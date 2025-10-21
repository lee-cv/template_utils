#pragma once
#include <ros/ros.h>
#include "DetectedObjectArray.h"

class ResultPublisher
{
public:
    ResultPublisher(ros::NodeHandle& nh, const std::string& config_file);
    void publish(const DetectedObjectArray& objects);

private:
    ros::Publisher pub_;
};
