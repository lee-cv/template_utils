cmake_minimum_required(VERSION 3.0.2)
project(object_detection_template)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
  image_transport
  cv_bridge
)

find_package(OpenCV REQUIRED)
find_package(yaml-cpp REQUIRED)

catkin_package(
  INCLUDE_DIRS include
)


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${YAML_CPP_INCLUDE_DIRS}
)

add_executable(object_detection_node 
  src/object_detection_node.cpp 
  src/image_processor.cpp 
  src/utils.cpp 
  src/detectors/dummy_detector.cpp)

target_link_libraries(main_node
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
  yaml-cpp
)

install(TARGETS main_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch config include
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)