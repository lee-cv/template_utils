#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>


namespace ros_target_detector {


struct DetectionResult {
    cv::Rect box;
    std::string label;
    float confidence = 0.0f;
};


class DetectorBase {
public:
    virtual ~DetectorBase() = default;
    // 初始化模型，返回是否成功
    virtual bool initialize(const std::string& model_path) = 0;
    // 同步检测接口：给定一帧 BGR 图像，返回检测结果
    virtual std::vector<DetectionResult> detect(const cv::Mat& image) = 0;
};


} // namespace