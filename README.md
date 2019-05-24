# Location_finder
Aruco marker detection with intel realsense camera D435 with less delay on 2DOF pose estimation

This package use a standard aruco_marker detection which is based in http://wiki.ros.org/aruco_detect. This package is used to publish the TF of the markers in 2d space ( In form of x y and also yaw ). And it also need a wrapper for the intel realsense camera which can be found here https://github.com/intel-ros/realsense. 

## Prerequisites
First to launch this file, we need to clone the aruco detect package.
```bash
cd
cd catkin_ws/src
git clone https://github.com/UbiquityRobotics/fiducials.git
cd ..
catkin_make
```
Next we need to install realsense wrapper for ROS

https://github.com/IntelRealSense/realsense-ros

## Launching the file

!! for MobiKa, launch the location_finder.launch in robot, and the camera.launch in the NUC. So that the TF publisher will not suffer from time synchronization problem.

To launch the camera and aruco detection
```bash
roslaunch location_finder camera.launch
```
To launch the TF publisher for the marker and the transformed marker
```bash
roslaunch location_finder location_finder.launch
```


