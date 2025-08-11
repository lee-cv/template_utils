#include <iostream>
#include <opencv2/opencv.hpp>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/visualization/pcl_visualizer.h>

int main() {
    // ===== 测试 OpenCV =====
    cv::Mat img(300, 300, CV_8UC3, cv::Scalar(50, 150, 250)); // 蓝色背景
    cv::circle(img, cv::Point(150, 150), 80, cv::Scalar(0, 255, 0), -1); // 绿色圆
    cv::putText(img, "OpenCV OK", cv::Point(40, 160),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);

    cv::imshow("OpenCV Test", img);
    cv::waitKey(500); // 显示 0.5 秒

    // ===== 测试 PCL 可视化 =====
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);

    // 创建一个简单立方体的点云
    for (float x = -0.5; x <= 0.5; x += 0.05f) {
        for (float y = -0.5; y <= 0.5; y += 0.05f) {
            for (float z = -0.5; z <= 0.5; z += 0.05f) {
                pcl::PointXYZRGB point;
                point.x = x;
                point.y = y;
                point.z = z;
                point.r = uint8_t((x + 0.5f) * 255);
                point.g = uint8_t((y + 0.5f) * 255);
                point.b = uint8_t((z + 0.5f) * 255);
                cloud->points.push_back(point);
            }
        }
    }
    cloud->width = cloud->points.size();
    cloud->height = 1;

    // 创建可视化器
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("PCL Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    viewer->addPointCloud<pcl::PointXYZRGB>(cloud, "sample cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");
    viewer->addCoordinateSystem(1.0);
    viewer->initCameraParameters();

    std::cout << "Close the PCL window to exit program." << std::endl;
    while (!viewer->wasStopped()) {
        viewer->spinOnce(100);
    }

    return 0;
}
