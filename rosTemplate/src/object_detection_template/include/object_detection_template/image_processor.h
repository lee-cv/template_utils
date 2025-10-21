#pragma once


#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <memory>
#include <mutex>
#include "detector_base.h"


namespace ros_target_detector {


class ImageProcessor {
public:
    ImageProcessor(ros::NodeHandle& nh, const std::string& topic_name, std::shared_ptr<DetectorBase> detector, const YAML::Node& cfg);

    ~ImageProcessor();

    void imageCallback(const sensor_msgs::ImageConstPtr& msg);
    void processFrame(const cv::Mat& frame);

private:
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    std::shared_ptr<DetectorBase> detector_;
    std::mutex proc_mutex_;

    // config
    bool enable_resize_ = true;
    int resize_w_ = 640;
    int resize_h_ = 480;
    bool debug_ = false;
    bool visualize_ = false;
};


} // namespace