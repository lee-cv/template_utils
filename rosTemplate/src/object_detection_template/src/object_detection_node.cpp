#include <ros/ros.h>
#include <yaml-cpp/yaml.h>
#include <memory>


#include "ros_target_detector/utils.h"
#include "ros_target_detector/image_processor.h"
#include "ros_target_detector/detector_base.h"
#include "ros_target_detector/detectors/dummy_detector.h"


using namespace ros_target_detector;


int main(int argc, char** argv) {
    ros::init(argc, argv, "ros_target_detector");
    ros::NodeHandle nh("~");


    std::string config_path;
    nh.param<std::string>("config_path", config_path, std::string("config/config.yaml"));


    YAML::Node cfg;
    if (!loadYamlFile(config_path, cfg)) {
        ROS_ERROR("Failed to load config, exiting");
        return -1;
    }


    std::string topic = cfg["camera"]["image_topic"].as<std::string>("/camera/color/image_raw");
    std::string model_type = cfg["model"]["type"].as<std::string>("dummy");
    std::string model_path = cfg["model"]["path"].as<std::string>("");


    std::shared_ptr<DetectorBase> detector;
    if (model_type == "dummy") {
        detector = std::make_shared<DummyDetector>();
    } else {
        // TODO: add more detectors (YOLO/ONNX/TensorRT) and select here
        detector = std::make_shared<DummyDetector>();
    }


    if (!detector->initialize(model_path)) {
        ROS_ERROR("Detector initialization failed");
        return -1;
    }

    ImageProcessor proc(nh, topic, detector, cfg);

    ROS_INFO("ros_target_detector node started");
    ros::spin();
    return 0;
}