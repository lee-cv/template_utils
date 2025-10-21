# ros_target_detector


通用且可扩展的 ROS1 C++ 目标检测模板。


## 特点
- YAML 配置读取（话题、模型路径、图像预处理参数）
- 抽象检测器接口 `DetectorBase`，方便扩展深度学习/传统检测
- 使用 `image_transport`，支持压缩订阅
- 简洁且高效：检测模块可在独立线程中实现


## 编译
```bash
cd ~/catkin_ws/src
git clone <this-repo>
cd ~/catkin_ws
catkin_make
source devel/setup.bash
roslaunch ros_target_detector detector.launch