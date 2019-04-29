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

add_library(${PROJECT_NAME}
  src/location_listener.cpp
  src/location_finder.cpp
)
add_executable(${PROJECT_NAME}_node src/location_finder_node.cpp)

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against

target_link_libraries(location_finder_node
   ${PROJECT_NAME}
   ${catkin_LIBRARIES}
 )

add_executable(service_caller_node src/service_caller.cpp)

add_dependencies(service_caller_node service_caller_generate_messages.cpp)

## Specify libraries to link a library or executable target against

target_link_libraries(service_caller_node
  ${catkin_LIBRARIES}
 )


 add_executable(location_publisher_node src/location_publisher.cpp)

 add_dependencies(location_publisher_node location_publisher_generate_messages.cpp)

 ## Specify libraries to link a library or executable target against

 target_link_libraries(location_publisher_node
   ${catkin_LIBRARIES}
  )


 ## Mark executables and/or libraries for installation
  install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

 ## Mark cpp header files for installation
  install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".svn" EXCLUDE
  )

 install(DIRECTORY launch
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
 )
