#include "ros_target_detector/image_processor.h"


namespace ros_target_detector {


ImageProcessor::ImageProcessor(ros::NodeHandle& nh, const std::string& topic_name, std::shared_ptr<DetectorBase> detector, const YAML::Node& cfg)
                                    : it_(nh), detector_(std::move(detector)) {


    if (cfg) {
        if (cfg["image"]["enable_resize"]) enable_resize_ = cfg["image"]["enable_resize"].as<bool>();
        resize_w_ = cfg["image"]["resize_width"].as<int>(resize_w_);
        resize_h_ = cfg["image"]["resize_height"].as<int>(resize_h_);
        debug_ = cfg["system"]["enable_debug"].as<bool>(false);
        visualize_ = cfg["system"]["visualize_result"].as<bool>(false);
    }

    image_sub_ = it_.subscribe(topic_name, 1, &ImageProcessor::imageCallback, this);
    ROS_INFO("ImageProcessor subscribed to %s", topic_name.c_str());
}


ImageProcessor::~ImageProcessor() {
    if (visualize_) cv::destroyAllWindows();
}


void ImageProcessor::imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    try {
        cv::Mat frame = cv_bridge::toCvShare(msg, "bgr8")->image.clone();
        processFrame(frame);
    } catch (const cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }
}


void ImageProcessor::processFrame(const cv::Mat& frame) {
    if (!detector_) return;
    std::lock_guard<std::mutex> lock(proc_mutex_);


    cv::Mat input = frame;
    if (enable_resize_) {
        cv::resize(frame, input, cv::Size(resize_w_, resize_h_));
    }


    auto t0 = std::chrono::steady_clock::now();
    auto results = detector_->detect(input);
    auto t1 = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();


    ROS_INFO("Detect: %lu boxes, time: %.2f ms", results.size(), ms);


    if (visualize_) {
        cv::Mat vis = input.clone();
        for (auto &r : results) {
            cv::rectangle(vis, r.box, cv::Scalar(0,255,0), 2);
            cv::putText(vis, r.label + " " + std::to_string(r.confidence),
            r.box.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255,0,0), 2);
        }
        cv::imshow("detector_vis", vis);
        cv::waitKey(1);
    }
}


} // namespace