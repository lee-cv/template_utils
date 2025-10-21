#include <ros/ros.h>
#include "sensor_manager.h"
#include "detection_manager.h"
#include "result_publisher.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "fusion_detection_node");
    ros::NodeHandle nh;
    ros::NodeHandle pnh("~");

    // 加载配置
    std::string config_file;
    pnh.param<std::string>("config_file", config_file, "fusion_config.yaml");

    SensorManager sensor_manager(nh, config_file);
    DetectionManager detection_manager(config_file);
    ResultPublisher result_pub(nh, config_file);

    // 主循环
    ros::Rate loop_rate(30); // 30Hz
    while (ros::ok())
    {
        sensor_manager.spinOnce();              // 更新传感器数据
        auto fused_objects = detection_manager.process(sensor_manager); // 运行检测 + 融合
        result_pub.publish(fused_objects);     // 发布结果
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
