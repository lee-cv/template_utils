cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(testDemo)

# find opencv
find_package(OpenCV REQUIRED)
if(DEFINED OpenCV_LIBS AND DEFINED OpenCV_INCLUDE_DIRS)
  set(OpenCV_FOUND TRUE)
  include_directories(${OpenCV_INCLUDE_DIRS})
  link_directories(${OpenCV_LIBS})
else()
  set(OpenCV_FOUND FALSE)
endif()

# PCL
find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# Eigen
include_directories("/usr/include/eigen3")


# obsensor sdk
# set(OrbbecSDK_ROOT_DIR /home/lee/Orbbec/OrbbecSDK_OpenSource)
# set(OrbbecSDK_LIBRARY_DIRS ${OrbbecSDK_ROOT_DIR}/lib)
# set(OrbbecSDK_INCLUDE_DIR ${OrbbecSDK_ROOT_DIR}/include)
# include_directories(${OrbbecSDK_INCLUDE_DIR})
# link_directories(${OrbbecSDK_LIBRARY_DIRS})

# find_package(rs_driver REQUIRED)
# include_directories(${rs_driver_INCLUDE_DIRS})

# target_link_libraries(main ${rs_driver_LIBRARIES})

# include_directories(
#     ${PROJECT_SOURCE_DIR}/utils
# )

# add_executable(main main.cpp)
# target_link_libraries(main OrbbecSDK ${rs_driver_LIBRARIES} ${PCL_LIBRARIES} ${OpenCV_LIBS})

add_executable(plyToImg plyToImg.cpp)
target_link_libraries(plyToImg ${rs_driver_LIBRARIES} ${PCL_LIBRARIES} ${OpenCV_LIBS})
