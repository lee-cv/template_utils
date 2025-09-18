#!/bin/bash
# 杀掉旧的相机节点
rosnode kill /camera/camera
sleep 5

# 启动相机
roslaunch orbbec_camera gemini_330_series.launch
