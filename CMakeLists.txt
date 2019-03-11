cmake_minimum_required(VERSION 2.8.3)
project(location_finder)

## Compile as C++11, supported in ROS Kinetic and newer
 add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED
	fiducial_msgs
	roscpp
	rospy
	geometry_msgs
	tf
	std_msgs
	message_generation
	actionlib
	)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


################################################
## Declare ROS messages, services and actions ##
################################################



# Generate messages in the 'msg' folder
add_message_files(
  FILES
 Position_publish.msg
)


## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   # fiducial_msgs
   # geometry_msgs
   std_msgs  # Or other packages containing msgs
 )


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES location_finder
 CATKIN_DEPENDS
 fiducial_msgs
 roscpp
 rospy
 geometry_msgs
 tf
 std_msgs
 message_runtime
 actionlib
#  DEPENDS system_lib
)



## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
include
${catkin_INCLUDE_DIRS}
)


add_executable(location_finder_node src/location_finder.cpp)

add_executable(location_listener_node src/location_listener.cpp)



# add_executable(publisher_node src/2DOF_publisher.cpp)


## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(location_finder_node location_finder_generate_messages.cpp)

add_dependencies(location_listener_node location_listener_generate_messages.cpp)



# add_dependencies(publisher_node publisher_generate_messages.cpp)

## Specify libraries to link a library or executable target against
 target_link_libraries(location_finder_node
   ${catkin_LIBRARIES}
 )

 target_link_libraries(location_listener_node
 	${catkin_LIBRARIES}
 )


 
# target_link_libraries(publisher_node
#  	${catkin_LIBRARIES}
#  )




