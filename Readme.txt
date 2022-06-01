The preprocessing module in IEEE PAMI 2013: A Framework for Automatic Modeling from Point Cloud Data


Structures the unstructured LiDAR data captured using an airborne scanner, into memory-manageable components which can be further processed in parallel. 

The size of the "tiles" or "geospatial bounding boxes" can be adjusted by changing the preprocessor flags: RES_x, RES_Y, RES_Z.

The copyright for the included bilateral filtering code belongs to Sylvain Paris and Frédo Durand.

References:
1. IEEE PAMI 2013: A Framework for Automatic Modeling from Point Cloud Data
2. IEEE CVPR 2009: Automatic reconstruction of cities from remote sensor data

More information about this work: www.poullis.org

Technical details:
- The project file is provided for Code::Blocks IDE. 
- It requires the libraries Image Magick and fftw3.
- A small sample file is provided in the data folder.

*IMPORTANT: To use this software, please consider citing the following in any resulting publication:*

@article{poullis2013framework,
  title={A Framework for Automatic Modeling from Point Cloud Data},
  author={Poullis, Charalambos},
  journal={Pattern Analysis and Machine Intelligence, IEEE Transactions on},
  volume={35},
  number={11},
  pages={2563--2575},
  year={2013},
  publisher={IEEE}
}

@inproceedings{poullis2009automatic,
  title={Automatic reconstruction of cities from remote sensor data},
  author={Poullis, Charalambos and You, Suya},
  booktitle={Computer Vision and Pattern Recognition, 2009. CVPR 2009. IEEE Conference on},
  pages={2775--2782},
  year={2009},
  organization={IEEE}
}
