#pragma once
#include "ros_target_detector/detector_base.h"


namespace ros_target_detector {


class DummyDetector : public DetectorBase {
public:
    DummyDetector() = default;
    ~DummyDetector() override = default;

    bool initialize(const std::string& model_path) override;
    std::vector<DetectionResult> detect(const cv::Mat& image) override;
};


} // namespace