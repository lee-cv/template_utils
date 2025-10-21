#include "ros_target_detector/utils.h"
#include <fstream>
#include <ros/ros.h>


namespace ros_target_detector {


bool loadYamlFile(const std::string& path, YAML::Node& out) {
    try {
        out = YAML::LoadFile(path);
        return true;
    } catch (const std::exception& e) {
        ROS_ERROR("Failed to load YAML file %s : %s", path.c_str(), e.what());
        return false;
    }
}


} // namespace