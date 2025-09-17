#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <Eigen/Dense>

// 生成一些测试点云
std::vector<pcl::PointXYZ> generateTestPoints() {
    std::vector<pcl::PointXYZ> points;
    points.emplace_back(0.1790, -4.2291, 2.1771);    // 正前方
    points.emplace_back(-0.0865, -4.2178, 2.1699);    // 右上方
    points.emplace_back(0.0903, -4.2284, 2.1754);  // 左下方
    points.emplace_back(0.3183, -4.2454, 2.1468);    // 更远
    points.emplace_back(0.4448, -4.2584, 2.1160);    // 很近
    return points;
}

// 点云从雷达坐标系 -> 相机坐标系
pcl::PointXYZ transformPoint(const pcl::PointXYZ& pt, const Eigen::Matrix4f& T) {
    Eigen::Vector4f p_lidar(pt.x, pt.y, pt.z, 1.0f);
    Eigen::Vector4f p_cam = T * p_lidar;
    return pcl::PointXYZ(p_cam[0], p_cam[1], p_cam[2]);
}

// 相机坐标 -> 图像像素
cv::Point2f projectToPixel(const pcl::PointXYZ& pt_cam, const cv::Mat& K) {
    double fx = K.at<double>(0, 0);
    double fy = K.at<double>(1, 1);
    double cx = K.at<double>(0, 2);
    double cy = K.at<double>(1, 2);

    return cv::Point2f(
        fx * pt_cam.x / pt_cam.z + cx,
        fy * pt_cam.y / pt_cam.z + cy
    );
}

int main() {
    // 1. 读取原始图像
    std::string img_path = "000000.png"; // <-- 修改成你的图像路径
    cv::Mat img = cv::imread(img_path);
    if (img.empty()) {
        std::cerr << "Failed to read image at " << img_path << std::endl;
        return -1;
    }

    // 2. 相机内参
    cv::Mat K = (cv::Mat_<double>(3, 3) <<
        610.515869, 0, 640.636475,
        0, 610.739746, 354.311493,
        0, 0, 1);

    // 3. 外参 T_Lidar2Cam
    Eigen::Matrix4f T_Lidar2Cam;
    T_Lidar2Cam << -1.0000, 0.0052, 0.0035, 0.0187,
                    -0.0036, -0.0155, -0.9999, -0.1172,
                    -0.0051, -0.9999, 0.0155, 0.1041,
                    0, 0, 0, 1;

    // 4. 测试点云
    auto points_lidar = generateTestPoints();

    // 5. 点云变换与投影
    for (size_t i = 0; i < points_lidar.size(); ++i) {
        auto pt_cam = transformPoint(points_lidar[i], T_Lidar2Cam);

        // 只投影Z>0的点（防止负深度）
        if (pt_cam.z <= 0.1) {
            std::cout << "Point " << i << " is behind camera, skipped." << std::endl;
            continue;
        }

        auto pixel = projectToPixel(pt_cam, K);

        // 画点
        cv::Scalar color(0, 0, 255); // 红色
        cv::circle(img, pixel, 5, color, -1);

        std::cout << "Point " << i 
                  << " | LidarXYZ: (" << points_lidar[i].x << "," << points_lidar[i].y << "," << points_lidar[i].z << ")"
                  << " -> CamXYZ: (" << pt_cam.x << "," << pt_cam.y << "," << pt_cam.z << ")"
                  << " -> Pixel: (" << pixel.x << "," << pixel.y << ")"
                  << std::endl;
    }

    // 6. 显示
    cv::imshow("Projection Result", img);
    cv::imwrite("projection_result.jpg", img); // 也可以保存结果
    cv::waitKey(0);

    return 0;
}
