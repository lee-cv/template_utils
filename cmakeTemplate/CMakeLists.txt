cmake_minimum_required(VERSION 3.10)
project(my_projects)

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 添加 utils 子目录
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/utils)

# OpenCV 配置
set(OpenCV_DIR /home/lee/Cpp/template_utils/cmakeTemplate/3rdparty/opencv-cuda)
find_package(OpenCV REQUIRED)
set(OpenCV_INCLUDE_DIRS ${OpenCV_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})


# 配置系统安装的 OpenCV
# find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui calib3d)
# include_directories(${OpenCV_INCLUDE_DIRS})
# link_directories(${OpenCV_LIBRARY_DIRS})

# PCL配置
find_package(PCL REQUIRED COMPONENTS common io filters segmentation surface visualization) 
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})

# 包含头文件目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/temp
    # ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/utils
)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/temp.cpp

)

# 或者对对单个源文件生成可执行文件
# add_executable(MyExecutable src/main.cpp)  # 创建一个可执行文件目标 MyExecutable

# 链接库
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${PCL_LIBRARIES}

    # 完整路径方式链接第三方静态/动态库
    # ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/utils/libutils.so
)

# if(CUDA_FOUND)
#     message(STATUS "CUDA found: ${CUDA_TOOLKIT_ROOT_DIR}")
# endif()

# 安装设置（可选）
# install(TARGETS ${PROJECT_NAME} DESTINATION .)
