#pragma once
#include <string>
#include <yaml-cpp/yaml.h>

namespace ros_target_detector {

bool loadYamlFile(const std::string& path, YAML::Node& out);


} // namespace