// 原有 include 和 namespace 保持不变
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

const float kMoveStep = 0.005;   // 5毫米平移步长
const float kRotateStep = 0.5 * M_PI / 180.0; // 0.5度旋转步长

// 保存矩阵到文件
void saveMatrix(const Eigen::Matrix4f& T, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        std::cerr << "Cannot open file to save: " << filename << std::endl;
        return;
    }
    ofs << T << std::endl;
    std::cout << "Saved T_Lidar2Cam to " << filename << std::endl;
}

// 加载ASCII格式的PLY文件
pcl::PointCloud<pcl::PointXYZI>::Ptr loadPLYFile(const std::string& filename) {
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return cloud;
    }
    std::string line;
    bool headerEnded = false;
    while (std::getline(file, line)) {
        if (line == "end_header") {
            headerEnded = true;
            break;
        }
    }
    if (!headerEnded) {
        std::cerr << "PLY header not properly terminated." << std::endl;
        return cloud;
    }
    float x, y, z, intensity;
    while (file >> x >> y >> z >> intensity) {
        pcl::PointXYZI pt;
        pt.x = x;
        pt.y = y;
        pt.z = z;
        pt.intensity = intensity;
        cloud->points.push_back(pt);
    }
    std::cout << "Loaded " << cloud->points.size() << " points from PLY." << std::endl;
    return cloud;
}

// 点变换：雷达系到相机系
pcl::PointXYZI transformPoint(const pcl::PointXYZI& pt, const Eigen::Matrix4f& T) {
    Eigen::Vector4f pt_lidar(pt.x, pt.y, pt.z, 1.0f);
    Eigen::Vector4f pt_cam = T * pt_lidar;
    pcl::PointXYZI pt_out;
    pt_out.x = pt_cam.x();
    pt_out.y = pt_cam.y();
    pt_out.z = pt_cam.z();
    pt_out.intensity = pt.intensity;
    return pt_out;
}

// 点投影到图像像素
cv::Point2f projectToPixel(const pcl::PointXYZI& pt_cam, const cv::Mat& K) {
    double fx = K.at<double>(0,0);
    double fy = K.at<double>(1,1);
    double cx = K.at<double>(0,2);
    double cy = K.at<double>(1,2);

    float u = static_cast<float>(fx * pt_cam.x / pt_cam.z + cx);
    float v = static_cast<float>(fy * pt_cam.y / pt_cam.z + cy);
    return cv::Point2f(u, v);
}

// 绘制点云到图像
void renderPointCloud(
    const pcl::PointCloud<pcl::PointXYZI>::Ptr& cloud,
    const Eigen::Matrix4f& T_Lidar2Cam,
    const cv::Mat& K,
    const cv::Mat& original_img,
    cv::Mat& output_img
) {
    int img_width = original_img.cols;
    int img_height = original_img.rows;
    output_img = original_img.clone();
    for (size_t i = 0; i < cloud->size(); ++i) {
        pcl::PointXYZI pt_cam = transformPoint(cloud->points[i], T_Lidar2Cam);
        if (pt_cam.z <= 0.1) continue;
        cv::Point2f pixel = projectToPixel(pt_cam, K);
        if (pixel.x >= 0 && pixel.x < img_width && pixel.y >= 0 && pixel.y < img_height) {
            cv::circle(output_img, pixel, 1, cv::Scalar(0, 0, 255), -1);
        }
    }
}

int main() {
    // 配置路径
    std::string ply_file = "000000.ply";    
    std::string image_file = "000000.png";   

    // 加载数据
    auto cloud = loadPLYFile(ply_file);
    if (cloud->empty()) return -1;
    cv::Mat original_img = cv::imread(image_file);
    if (original_img.empty()) return -1;

    // 相机内参
    cv::Mat K = (cv::Mat_<double>(3, 3) << 
        610.515869, 0, 640.636475,
        0, 610.739746, 354.311493,
        0, 0, 1);

    // 外参初始化
    Eigen::Matrix4f T_Lidar2Cam;
    T_Lidar2Cam << 
                -0.9979,  0.0542,  0.0353,  0.2356,
                -0.0234,  0.2076, -0.9779,  0.0153,
                -0.0603, -0.9767, -0.2059,  0.0102,
                 0, 0, 0, 1;

    cv::Mat display_img;
    renderPointCloud(cloud, T_Lidar2Cam, K, original_img, display_img);
    cv::imshow("Projected", display_img);

    std::cout << "Use keys to adjust:\n"
                 " Arrow keys - move (X/Y)\n"
                 " Z/C - move (Z)\n"
                 " A/D - rotate around X\n"
                 " W/S - rotate around Y\n"
                 " Q/E - rotate around Z\n"
                 " P - save matrix\n"
                 " ESC - exit\n";

    while (true) {
        int key = cv::waitKey(0);
        bool updated = false;

        // 平移
        if (key == 82) { // up arrow
            T_Lidar2Cam(1,3) -= kMoveStep;
            updated = true;
        } else if (key == 84) { // down arrow
            T_Lidar2Cam(1,3) += kMoveStep;
            updated = true;
        } else if (key == 81) { // left arrow
            T_Lidar2Cam(0,3) -= kMoveStep;
            updated = true;
        } else if (key == 83) { // right arrow
            T_Lidar2Cam(0,3) += kMoveStep;
            updated = true;
        } else if (key == 'z' || key == 'Z') { // Z
            T_Lidar2Cam(2,3) += kMoveStep;
            updated = true;
        } else if (key == 'c' || key == 'C') { // C
            T_Lidar2Cam(2,3) -= kMoveStep;
            updated = true;
        }

        // 旋转
        else if (key == 'a' || key == 'A') {
            Eigen::Matrix3f R = Eigen::AngleAxisf(kRotateStep, Eigen::Vector3f::UnitX()).toRotationMatrix();
            T_Lidar2Cam.block<3,3>(0,0) = R * T_Lidar2Cam.block<3,3>(0,0);
            updated = true;
        } else if (key == 'd' || key == 'D') {
            Eigen::Matrix3f R = Eigen::AngleAxisf(-kRotateStep, Eigen::Vector3f::UnitX()).toRotationMatrix();
            T_Lidar2Cam.block<3,3>(0,0) = R * T_Lidar2Cam.block<3,3>(0,0);
            updated = true;
        } else if (key == 'w' || key == 'W') {
            Eigen::Matrix3f R = Eigen::AngleAxisf(kRotateStep, Eigen::Vector3f::UnitY()).toRotationMatrix();
            T_Lidar2Cam.block<3,3>(0,0) = R * T_Lidar2Cam.block<3,3>(0,0);
            updated = true;
        } else if (key == 's' || key == 'S') {
            Eigen::Matrix3f R = Eigen::AngleAxisf(-kRotateStep, Eigen::Vector3f::UnitY()).toRotationMatrix();
            T_Lidar2Cam.block<3,3>(0,0) = R * T_Lidar2Cam.block<3,3>(0,0);
            updated = true;
        } else if (key == 'q' || key == 'Q') {
            Eigen::Matrix3f R = Eigen::AngleAxisf(kRotateStep, Eigen::Vector3f::UnitZ()).toRotationMatrix();
            T_Lidar2Cam.block<3,3>(0,0) = R * T_Lidar2Cam.block<3,3>(0,0);
            updated = true;
        } else if (key == 'e' || key == 'E') {
            Eigen::Matrix3f R = Eigen::AngleAxisf(-kRotateStep, Eigen::Vector3f::UnitZ()).toRotationMatrix();
            T_Lidar2Cam.block<3,3>(0,0) = R * T_Lidar2Cam.block<3,3>(0,0);
            updated = true;
        }

        else if (key == 'p' || key == 'P') { // 保存外参
            saveMatrix(T_Lidar2Cam, "T_Lidar2Cam_saved.txt");
        }

        else if (key == 27) { // ESC
            break;
        }

        if (updated) {
            renderPointCloud(cloud, T_Lidar2Cam, K, original_img, display_img);
            cv::imshow("Projected", display_img);

            std::cout << "Updated T_Lidar2Cam:\n" << T_Lidar2Cam << "\n";
        }
    }

    std::cout << "\nFinal T_Lidar2Cam:\n" << T_Lidar2Cam << "\n";

    return 0;
}
