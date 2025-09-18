#!/bin/bash
# 切换到工作空间
cd /home/marvsmart/semi_module_robot_ws

# 加载 ROS 环境
source /opt/ros/noetic/setup.bash
source /home/marvsmart/semi_module_robot_ws/devel/setup.bash

export ROS_MASTER_URI=http://localhost:11311
export ROS_HOSTNAME=localhost

# 启动四个节点（后台运行）
roslaunch orbbec_camera gemini_330_series.launch &
sleep 5
roslaunch semirobot_pv_detector pv_detector.launch &
sleep 3
roslaunch semirobot_modbus modbus_server.launch &
sleep 1
roslaunch asrobot_ahrs_imu_driver asrobot_ahrs_imu_driver.launch &
sleep 1
roslaunch semirobot_modbus modbus_client.launch &
sleep 2
roslaunch semirobot_result_recorder recorder.launch &
sleep 2
roslaunch semirobot_camera_monitor camera_monitor.launch &
# 防止脚本退出
wait
