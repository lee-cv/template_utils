#include "ros_target_detector/detectors/dummy_detector.h"
#include <ros/ros.h>


namespace ros_target_detector {

bool DummyDetector::initialize(const std::string& model_path) {
    ROS_INFO("DummyDetector initialized. model: %s", model_path.c_str());
    return true;
}

std::vector<DetectionResult> DummyDetector::detect(const cv::Mat& image) {
    std::vector<DetectionResult> out;
    if (image.empty()) return out;
    // 模拟：在图像中心输出一个框
    int w = image.cols, h = image.rows;
    int bw = w/6, bh = h/6;
    DetectionResult r;
    r.box = cv::Rect(w/2 - bw/2, h/2 - bh/2, bw, bh);
    r.label = "dummy";
    r.confidence = 0.9f;
    out.push_back(r);
    return out;
}


} // namespace