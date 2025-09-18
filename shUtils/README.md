# 对Ubuntu一些常见的问题指令和一些sh脚本进行收集
```sh

```

## 配置本地Rviz打开远程ssh的话题
```sh
sudo nano /etc/hosts : 
10.42.0.1	asrobot

bashrc:
export ROS_MASTER_URI=http://10.42.0.1:11311
export ROS_IP=10.42.0.36
```

## 配置生成SSH-Key和git推送
```sh
ssh-keygen -t ed25519 -C "lijianxin231@gmail.com"

git config --global user.name "lee-cv"
git config --global user.email "lijianxin231@gmail.com"
git config --global --list
```

## 用venv配置python环境
```sh
sudo apt update
sudo apt install python3-venv
python3 -m venv venv_name
source venv_name/bin/activate
```
