////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////

#ifndef __FILE_UTILITIES_H__
#define __FILE_UTILITIES_H__

#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

///Enumarated type for file types
typedef enum {
	UNSUPPORTED_FORMAT = -1,
	JPG,
	PNG,
	TGA,
	BMP,
	HDR,
	PFM,
	XYZ,
	XYZI,
	XYZRGB,
	OBJ,
	LAS,
	TIF
} FILE_TYPE;


///Separates the file name from the extension
static bool separate(std::string const &file_name, std::string &name, FILE_TYPE &type)	{

	///The position of the "."
	int pos = file_name.rfind('.');

	///The name of the file
	name = file_name.substr(0,pos-1);

	///The extension of the file
	std::string file_extension = &file_name.c_str()[pos+1];
	if (strcasecmp(file_extension.c_str(),"jpg")==0 || strcasecmp(file_extension.c_str(),"jpeg")==0)	{
		type = JPG;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"png")==0)	{
		type = PNG;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"tga")==0)	{
		type = TGA;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"hdr")==0)	{
		type = HDR;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"pfm")==0)	{
		type = PFM;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"xyz")==0)	{
		type = XYZ;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"xyzi")==0)	{
		type = XYZI;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"xyzrgb")==0)	{
		type = XYZRGB;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"obj")==0)	{
		type = OBJ;
		return true;
	}
	if (strcasecmp(file_extension.c_str(),"tiff")==0)	{
		type = TIF;
		return true;
	}
	type = UNSUPPORTED_FORMAT;
	return false;
}


static int getDirectoryContents(std::string const &dir, std::vector<std::string> &file_names)	{
    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        file_names.push_back(std::string(dirp->d_name));
    }
    closedir(dp);

    return 0;
}
#endif
