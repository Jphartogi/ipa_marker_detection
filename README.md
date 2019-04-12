# Location_finder
Aruco marker detection with intel realsense camera D435 with less delay on 2DOF pose estimation

This package use a standard aruco_marker detection which is based in http://wiki.ros.org/aruco_detect. This package is used to publish the TF of the markers in 2d space. And it also need a wrapper for the intel realsense camera which can be found here https://github.com/intel-ros/realsense. 

## Launching the file
We can easily launch the file with
```bash
roslaunch location_finder location_finder.launch
```bash
