////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	      //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#include "Vector.h"
#include "BoundingBox.h"
#include "GeometryExporter.h"
#include "GeospatialBoundingBox.h"



#define DEFAULT_RES		1024

///BBox centroid repositioning offset. This repositions the data to its original position
Vector3f centroid_repos_offset;
///The bounding box of the entire dataset.
BoundingBox *bbox;
///The file's byte_length
long byte_length;
///The number of points in the dataset
long number_of_points;
///The data file name
std::string file_name;

///The geospatial bounding boxes
std::vector<GeospatialBoundingBox *> geo_bboxes;
///The resolution of the bounding boxes
float res_x, res_y, res_z;

///Measures the total bytes in a file
void byteLength(const char *file_name);
///Removes a geospatial bounding box i.e. in case it will be subdivided later
bool removeGeospatialBoundingBox(int index);
///Creates all the geospatial bounding boxes
void createGeospatialBoundingBoxes();
///Subdivides a geospatial bounding box into 8 smaller
void subdivideGeospatialBoundingBox(int index, int level);
///Inserts a point in the data
void insert(Vector3f const &point, int level=0);
///Exports the structure information file which contains spatial information about the dataset
void exportStructureInformationFile();
///Sorts the points in a file
void sort(const char *file_name);
///Computes a bounding box for the data in the file
void computeBoundingBox(const char *file_name);
///Structures the data into a number of geospatial bounding boxes
void structure(const char *file_name);
///Creates a single geospatial bounding box
int createGeospatialBoundingBox(Vector3f const &centroid, Vector3f const &resolution, std::string *_file_name);

void byteLength(const char *file_name)	{

	std::cout << "Computing size of data in bytes..." << std::endl;

	///Create the input file stream
	ifstream is;
	///Open the file for reading in text mode
	is.open(file_name);
	///Make sure the file was opened correctly
	if (!is.is_open()) return;

	///Compute the length of the file
	///First position the file pointer at the end
	is.seekg(0, ios::end);
	///Then get the byte number
	byte_length = is.tellg();

	///Close the file
	is.close();

	///Print out the length
	std::cout << _format("Byte length: %ld",byte_length) << std::endl;
	std::cout << "...done." << std::endl;

	return;
}

int createGeospatialBoundingBox(Vector3f const &centroid, Vector3f const &resolution, std::string *_file_name=0x00)	{

	///Create a geospatial bounding box for this bbox
	GeospatialBoundingBox *geo_bbox = new GeospatialBoundingBox(centroid,resolution,_file_name);
	///add it to the list
	geo_bboxes.push_back(geo_bbox);
	return geo_bboxes.size()-1;
}

bool removeGeospatialBoundingBox(int index)	{

	///check for boundary conditions
	if (index<0 || index>= geo_bboxes.size())	return false;
	///remove the geospatial box
	std::vector<GeospatialBoundingBox *>::iterator geo_itor = geo_bboxes.begin();
	geo_itor += index;
	std::string file_name = geo_bboxes[index]->getFileName();
	delete geo_bboxes[index];
	geo_bboxes.erase(geo_itor);
	///remove the geobox from the file system
	remove(file_name.c_str());
	return true;
}

void createGeospatialBoundingBoxes()	{

	Vector3f min_pt = bbox->getMinPt();
	Vector3f max_pt = bbox->getMaxPt();

	///Compute the number of geospatial bounding boxes for each dimension
	int sizex = max_pt(0) - min_pt(0);
	int sizey = max_pt(1) - min_pt(1);
	int sizez = max_pt(2) - min_pt(2);

	int numx = max(1.0f,ceil(float(sizex)/float(res_x)));
	int numy = max(1.0f,ceil(float(sizey)/float(res_y)));
	int numz = max(1.0f,ceil(float(sizez)/float(res_z)));


	std::cout << _format("Initial geospatial bounding boxes resolution: %.02fx%.02fx%.02f.",res_x,res_y,res_z) << std::endl;
	std::cout << _format("Initial number of geospatial bounding boxes: %dx%dx%d.",numx,numy,numz) << std::endl;

	Vector3f resolution = Vector3f(res_x,res_y,res_z);

	///Add the centroid as the first point to each geospatial bounding box.
	float z=min_pt(2) + float(res_z)/2.0f;
	for (unsigned int i=0;i<numz;i++)	{
		float y=min_pt(1) + float(res_y)/2.0f;
		for (unsigned int j=0;j<numy;j++)	{
			float x=min_pt(0) + float(res_x)/2.0f;
			for (unsigned int k=0;k<numx;k++)	{
				///Add the point to the tree
				createGeospatialBoundingBox(Vector3f(x,y,z),resolution);
				//std::cout << Vector3f(x,y,z) << std::endl;
				x += float(res_x);
			}
			y += float(res_y);
		}
		z += float(res_z);
	}

	return;
}

void subdivideGeospatialBoundingBox(int index, int level)	{

	///subdivide the geo box into 8 geoboxes
	for (int i=0;i<level;i++)	std::cout << "\t";
	std::cout << _format("Performing subdivision at level %d ...", level) << std::endl;
	///first get the centroid of the parent geobox
	Vector3f centroid = geo_bboxes[index]->getCentroid();

	///get the resolution of the parent geobox
	Vector3f resolution = geo_bboxes[index]->getResolution();

	///get the min and max points of the geo box
	Vector3f geo_min_pt = geo_bboxes[index]->getMinPt();
	Vector3f geo_max_pt = geo_bboxes[index]->getMaxPt();

	///get the name of the parent geobox
	std::string parent_file_name = geo_bboxes[index]->getFileName();
	///get only the number of the file and and not the extention
	char geo_box_parent_id[256];
	sscanf(parent_file_name.c_str(), "../output/geo_bbox_%[^xyz].xyz",&geo_box_parent_id);
	///get all the points in the parent geo box
	std::vector<Vector3f> points = geo_bboxes[index]->getPoints();
	///remove the parent geobox
	if (!removeGeospatialBoundingBox(index))	{
		std::cout << "Error removing parent geospatial bounding box" << std::endl;
	}

	///Create 8 octands, 2 on each axis
	///Add the centroid as the first point to each geospatial bounding box.
	///The resolution of the sub geo boxes will be half the parent's resolution
	///Provide buffer zone
	Vector3f half_resolution = Vector3f(0.5f,0.5f,0.5f) + resolution/2.0f;
	//std::cout << geo_min_pt << " " << geo_max_pt << "||" << "from: " << resolution << " to: " << half_resolution << std::endl;
	int new_ids = 0;
	std::string new_file_name;
	float z=geo_min_pt(2) + half_resolution(2)/2.0f;
	for (int i=0;i<2;i++)	{
		float y=geo_min_pt(1) + half_resolution(1)/2.0f;
		for (int j=0;j<2;j++)	{
			float x=geo_min_pt(0) + half_resolution(0)/2.0f;
			for (int k=0;k<2;k++)	{
				///Form the filename for the first geobox
				new_file_name = _format("../output/geo_bbox_%s%d.xyz",geo_box_parent_id,new_ids);
				new_ids++;
				///Create a geospatial bounding box for this bbox
				int new_index = createGeospatialBoundingBox(Vector3f(x,y,z),half_resolution,&new_file_name);
				//std::cout << Vector3f(x,y,z) << std::endl;
				x += float(half_resolution(0));
			}
			y += half_resolution(1);
		}
		z += half_resolution(2);
	}

	///sort the points which were part of the parent geo box
	for (int i=0;i<points.size();i++)	{
		insert(points[i], level+1);
	}

	for (int i=0;i<level;i++)	std::cout << "\t";
	std::cout << "done." << std::endl;
	return;
}


void insert(Vector3f const &point, int level)	{

	///go through the bboxes' centroids and check if the point lies inside any of them
	int index=-1;
	for (int i=0;i<geo_bboxes.size();i++)	{
		if (geo_bboxes[i]->liesInside(point))	{
			///save the index of the geospatial bounding box
			index = i;
			break;
		}
	}
	///if there is a valid index
	if (index!=-1)	{
		///the point is inside the cube so add it
		///add the point to the corresponding geospatial bounding box
		geo_bboxes[index]->insert(point);
		///check whether the geospatial bounding box has reached it's maximum capacity
		if (geo_bboxes[index]->maxedOut())	{
			///subdivide into 8
			subdivideGeospatialBoundingBox(index,level);
		}
	}
	else	{
		std::cout << "level: " << level << " index: " << index << " point: " << point << " min: " << bbox->getMinPt() << " max: " << bbox->getMaxPt() << std::endl;
		std::cout << geo_bboxes.size() << std::endl;
		for (unsigned int i=0;i<geo_bboxes.size();i++)	{
			std::cout << geo_bboxes[i]->getMinPt() << " " << geo_bboxes[i]->getMaxPt() << std::endl;
		}
		std::cout <<"Probably this problem is caused by the last line of the XYZ file which is read as 0,0,0 and the centroid is subtracted from it. Removing the line will fix the problem." << std::endl;
		exit(-1);
	}

	return;
}


void exportStructureInformationFile()	{

	FILE *file_ptr = fopen("structure_file.info","w");
	fprintf(file_ptr, "%d\n",geo_bboxes.size());
	fprintf(file_ptr, "centroid offset: %f %f %f\n", centroid_repos_offset(0), centroid_repos_offset(1), centroid_repos_offset(2));
	fprintf(file_ptr, "res: %f %f %f\n", res_x, res_y, res_z);
	fprintf(file_ptr, "points: %d\n", number_of_points);
	fprintf(file_ptr, "filename: %s\n", file_name.c_str());
	fclose(file_ptr);
	return;
}

void sort(const char *file_name)	{

	///Read the data once and distribute each point to the right cell in the kd-tree. At the same
	///time create geospatial bounding boxes which store the points in each cell to the disk.
	///Destroy the kd-tree at the end.

	std::cout << "Sorting points..." << std::endl;

	///Create the input file stream
	ifstream is;
	///Open the file for reading in text mode
	is.open(file_name);
	///Make sure the file was opened correctly
	if (!is.is_open()) return;

	///Read all points and insert them to the kd-tree
	std::string line;
	Vector3f current_pt;
	while (!is.eof())	{
		///Get the current line
		getline (is, line);
		if (!line.size()) continue;
		///Process the string
		sscanf(line.c_str(), "%f %f %f\n", &current_pt(0),&current_pt(1),&current_pt(2));
		///Flip the Y - only applies for the CHP dataset
		//current_pt(1) = -current_pt(1);
		///Offset the point
		current_pt -= centroid_repos_offset;
		///insert the point to the right place
		insert(current_pt);
	}

	///Close the file
	is.close();

	///Go through the geospatial bounding boxes and remove any of them without any points in them
	int variable_size = geo_bboxes.size();
	for (int i=0;i<variable_size;i++)	{
		if (geo_bboxes[i]->getNumberOfPoints() == 0)	{
			removeGeospatialBoundingBox(i);
			i--;
			variable_size--;
		}
	}
	std::cout << "...done." << std::endl;

	return;
}

void computeBoundingBox(const char *file_name)	{

	///Read the data once and compute the Bounding Box (min and max points)
	///Using the min and max points reposition the dataset around the origine ( subtract by the centroid)

	std::cout << "Recentering..." << std::endl;

	///Create the input file stream
	ifstream is;
	///Open the file for reading in text mode
	is.open(file_name);
	///Make sure the file was opened correctly
	if (!is.is_open()) return;

	///Min/Max points for BBox
	Vector3f min_pt = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3f max_pt = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	///Read all points and compute the min/max points for the BBox
	std::string line;
	float x, y, z;
	number_of_points = 0;
	while (!is.eof())	{
		///Get the current line
		getline (is, line);
		if (!line.size()) continue;
		sscanf(line.c_str(),"%f %f %f\n",&x,&y,&z);

		///Flip the Y. This is only needed for the CHP dataset
		//y = -y;

		if (min_pt(0) > x)	min_pt(0) = x;
		if (max_pt(0) < x)	max_pt(0) = x;

		if (min_pt(1) > y)	min_pt(1) = y;
		if (max_pt(1) < y)	max_pt(1) = y;

		if (min_pt(2) > z)	min_pt(2) = z;
		if (max_pt(2) < z)	max_pt(2) = z;

		number_of_points++;
	}

	///Print the BBox min/max
	std::cout << _format("Min pt: %f %f %f", min_pt(0), min_pt(1), min_pt(2)).c_str() << std::endl;
	std::cout << _format("Max pt: %f %f %f", max_pt(0), max_pt(1), max_pt(2)).c_str() << std::endl;

	///Compute centroid
	centroid_repos_offset = (min_pt + max_pt)/2.0f;
	///Print the centroid
	std::cout << _format("Centroid repositioning offset: %f %f %f", centroid_repos_offset(0),centroid_repos_offset(1),centroid_repos_offset(2)) << std::endl;

	///Recenter the min and max points.
	min_pt -= centroid_repos_offset;
	max_pt -= centroid_repos_offset;

	///Create the bounding box of the entire dataset
	if (bbox) delete bbox;
	bbox = new BoundingBox(min_pt, max_pt);


	///Print the recentered BBox min/max
	std::cout << _format("Recentered Min pt: %f %f %f", min_pt(0), min_pt(1), min_pt(2)) << std::endl;
	std::cout << _format("Recentered Max pt: %f %f %f", max_pt(0), max_pt(1), max_pt(2)) << std::endl;

	///Print the total number of points in the dataset
	std:: cout << _format("Total number of points in dataset: %d", number_of_points) << std::endl;

	///Close the file
	is.close();

	std::cout << "...done." << std::endl;

	return;
}



void structure(const char *file_name)	{

	std::cout << _format("Starting structuring %s.", timestamp().c_str()).c_str() << std::endl;

	///Compute the bounding box of the data
	computeBoundingBox(file_name);

	///Create the geo spatial boxes
	createGeospatialBoundingBoxes();

	///Sort the data to the appropriate geospatial bounding box
	sort(file_name);

	std::cout << _format("Final number of geospatial bounding boxes: %d",geo_bboxes.size()) << std::endl;

	///Export the information file
	exportStructureInformationFile();

	std::cout << _format("Finished structuring %s.", timestamp().c_str()).c_str() << std::endl;

	return;
}


int main(int argc, char *argv[])    {

	res_x = float(DEFAULT_RES);
	res_y = float(DEFAULT_RES);
	res_z = float(DEFAULT_RES);

	centroid_repos_offset = Vector3f(0.0f,0.0f,0.0f);
	bbox = 0x00;

	byte_length = 0;

	number_of_points = 0;

    if (argc != 2)  {
        std::cout << "Number of arguments should be 2. Quiting..." << std::endl;
        exit(-1);
        //file_name = std::string("pointcloud.xyz");
    }
    else    {
        file_name = std::string(argv[1]);
    }

    ///Compute the byte length of the file
    byteLength(file_name.c_str());

    ///Structure the data
    structure(file_name.c_str());

    ///Create a geometry exporter object
    GeometryExporter *ge = new GeometryExporter();

    ///Export the information files for each geobox
    for (int i=0;i<geo_bboxes.size();i++)	{
        geo_bboxes[i]->exportInformationFile();

        if (geo_bboxes[i]->getNumberOfPoints()>0)	{
            ///Initialize the data required for further processing
            geo_bboxes[i]->initialize();

            ///Export the entire mesh object

            ///Get the triangulated object corresponding to this geospatial bounding box
            GeometricObject *object = geo_bboxes[i]->getObject();
            object->translate(Vector3f(geo_bboxes[i]->getCentroid()));
            ge->exportToOBJ(geo_bboxes[i]->getFileName().c_str(),object);
            std::cout << _format("Exported geospatial bounding box: %s", _format("%s.obj",geo_bboxes[i]->getFileName().c_str()).c_str()) << std::endl;
            delete object;
        }
    }

    delete ge;


    return 0;
}



