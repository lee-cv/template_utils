# 对Ubuntu一些常见的问题指令和一些sh脚本进行收集

## 配置本地Rviz打开远程ssh的话题
```sh
sudo nano /etc/hosts : 
10.42.0.1	asrobot

bashrc:
export ROS_MASTER_URI=http://10.42.0.1:11311
export ROS_IP=10.42.0.36
```
