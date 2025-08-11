cmake_minimum_required(VERSION 3.10)
project(my_projects)

set(CMAKE_CXX_STANDARD 11)

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 添加 utils 子目录
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/utils)

# OpenCV 配置
# set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/opencv/opencv-linux-aarch64/share/OpenCV)
# find_package(OpenCV REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui calib3d)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

# PCL配置（这个需要下载PCL库）：sudo apt install libpcl-dev
find_package(PCL REQUIRED COMPONENTS common io filters segmentation surface visualization) 
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})

# 查找libmodbus库
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMODBUS REQUIRED libmodbus)

# obsensor sdk
set(OrbbecSDK_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/OrbbecSDK_arm64)
set(OrbbecSDK_LIBRARY_DIRS ${OrbbecSDK_ROOT_DIR}/lib)
set(OrbbecSDK_INCLUDE_DIR ${OrbbecSDK_ROOT_DIR}/include)
include_directories(${OrbbecSDK_INCLUDE_DIR})
link_directories(${OrbbecSDK_LIBRARY_DIRS})

# 包含头文件目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/semi_module_robot
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rknpu2/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/OrbbecSDK_arm64/include
    # ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/opencv/opencv-linux-aarch64/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libsndfile/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/librga/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/stb_image
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/jpeg_turbo/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/allocator/dma
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/allocator/drm
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/timer
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/utils
)

find_package(Threads REQUIRED)
find_package(yaml-cpp REQUIRED)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/yolov8_seg.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/postprocess.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ob_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/obstream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/depth_processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/detprocess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/modbus_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp

)

# 链接库
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME}
    OrbbecSDK
    fileutils
    imageutils
    imagedrawing
    yaml-cpp
    Threads::Threads
    ${OpenCV_LIBS}
    ${PCL_LIBRARIES}
    ${LIBMODBUS_LIBRARIES}

    # 完整路径方式链接第三方静态/动态库
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rknpu2/Linux/aarch64/librknnrt.so
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/librga/Linux/aarch64/librga.so
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/librga/Linux/aarch64/librga.a
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libsndfile/Linux/aarch64/libsndfile.a
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/jpeg_turbo/Linux/aarch64/libturbojpeg.a
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/kaldi_native_fbank/Linux/aarch64/libkaldi-native-fbank-core.a
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fftw/Linux/aarch64/libfftw3f.a
)

# 安装设置（可选）
install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rknpu2/Linux/aarch64/librknnrt.so DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/model DESTINATION .)

