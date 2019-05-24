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

# ROS Wrapper for Intel&reg; RealSense&trade; Devices
These are packages for using Intel RealSense cameras (D400 series and the SR300) with ROS.

## Installation Instructions

The following instructions support ROS Indigo, on **Ubuntu 14.04**, and ROS Kinetic, on **Ubuntu 16.04**.

#### The simplest way to install on a clean machine is to follow the instructions on the [.travis.yml](https://github.com/intel-ros/realsense/blob/development/.travis.yml) file. It basically summerize the elaborate instructions in the following 3 steps:

### Step 1: Install the latest Intel&reg; RealSense&trade; SDK 2.0
- #### Install from [Debian Package](https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md#installing-the-packages) - In that case treat yourself as a developer. Make sure you follow the instructions to also install librealsense2-dev and librealsense-dkms packages.

#### OR
- #### Build from sources by downloading the latest [Intel&reg; RealSense&trade; SDK 2.0](https://github.com/IntelRealSense/librealsense/releases/tag/v2.19.2) and follow the instructions under [Linux Installation](https://github.com/IntelRealSense/librealsense/blob/master/doc/installation.md)

### Step 2: Install the ROS distribution
- #### Install [ROS Kinetic](http://wiki.ros.org/kinetic/Installation/Ubuntu), on Ubuntu 16.04

### Step 3: Install Intel&reg; RealSense&trade; ROS from Sources
- Create a [catkin](http://wiki.ros.org/catkin#Installing_catkin) workspace
```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src/
```
- Clone the latest Intel&reg; RealSense&trade; ROS from [here](https://github.com/intel-ros/realsense/releases) into 'catkin_ws/src/'

```bash
catkin_init_workspace
cd ..
catkin_make clean
catkin_make -DCATKIN_ENABLE_TESTING=False -DCMAKE_BUILD_TYPE=Release
catkin_make install
echo "source ~/catkin_ws/devel/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

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


