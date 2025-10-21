#pragma once
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud2.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>

class SensorManager
{
public:
    SensorManager(ros::NodeHandle& nh, const std::string& config_file);
    void spinOnce();
    
    const sensor_msgs::ImageConstPtr& getLatestImage() const;
    const sensor_msgs::PointCloud2ConstPtr& getLatestPointCloud() const;

private:
    ros::NodeHandle nh_;
    message_filters::Subscriber<sensor_msgs::Image> camera_sub_;
    message_filters::Subscriber<sensor_msgs::PointCloud2> lidar_sub_;
    typedef message_filters::TimeSynchronizer<sensor_msgs::Image, sensor_msgs::PointCloud2> Sync;
    std::shared_ptr<Sync> sync_;

    sensor_msgs::ImageConstPtr latest_image_;
    sensor_msgs::PointCloud2ConstPtr latest_cloud_;

    void callback(const sensor_msgs::ImageConstPtr& img,
                  const sensor_msgs::PointCloud2ConstPtr& cloud);
};
