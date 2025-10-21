#pragma once
#include "sensor_manager.h"
#include "DetectedObjectArray.h"

class DetectionManager
{
public:
    DetectionManager(const std::string& config_file);
    DetectedObjectArray process(const SensorManager& sensor_manager);

private:
    void detectImage(const sensor_msgs::ImageConstPtr& img);
    void detectLidar(const sensor_msgs::PointCloud2ConstPtr& cloud);
    void fuseDetections();
};
