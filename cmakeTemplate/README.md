# semi_module_robot



## 1.1 工程简介

 **半自动铺装无人车-视觉系统** 是在Ubuntu_20.0 && arm64架构 && RK3588 环境下用于引导抓取光伏板的程序。它包括：
 + 图像目标识别功能
 + 目标物体位姿计算功能
 + 与PLC通讯实现电机控制功能
 
 ### 1.1.1 使用的硬件型号

- 深度相机：Orbbec_gemini_336L

- 开发板： RK3588 

## 1.2 运行说明

(1) 本工程感知识别模块名称为： semi_module_robot，生成的可执行程序名称：semi_module_robot。

```sh
cd /home/marvsmart/semi_module_robot/build 
sudo ./semi_module_robot
```


## 2 工作任务描述

### 2.1 光伏板像素位置识别 

 + 检测当前视野中货盘上的光伏板；


### 2.2 抓取姿态计算

 + 根据货盘上光伏板像素位置计算出其与相机的相对位姿（供端拾器使用）；


### 2.3.1 主程序逻辑

 + 有彩色图像和深度图像。

 + 有图像中的目标检测结果（带 box 和 boxMask）。

 + 有标定好的相机内参和端拾器到相机的外参。

 + 基于像素对应的深度值计算像素对应的三维坐标，进而去计算光伏板位姿。

### 2.3.2 平面拟合原理：

 + 输入：支架的点云

 + 计算：SAC_RANSAC

### 2.3.3 位姿求解原理：

 + 输入：支架的点云

 + 计算：点云的协方差矩阵


## 3 需要安装的包

```sh

sudo apt install libopencv-dev libpcl-dev libyaml-cpp-dev libmodbus-dev libdrm-dev

```

# vscode 配置

