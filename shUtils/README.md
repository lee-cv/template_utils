# 对Ubuntu一些常见的问题指令和一些sh脚本进行收集
```sh

```

## ROS1的包转为ROS2的包
```sh 在python环境下运行：
rosbags-convert --src pvgrab1.bag --dst new_test
```


## 终端扫描IP
```sh
seq 1 254 | xargs -P 50 -I{} sh -c 'ping -c 1 -W 1 192.168.60.{} >/dev/null && echo 192.168.60.{}'
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

## 配置工控机热点
```sh
sudo nmcli device wifi hotspot ifname wlan0 con-name MyHostspot ssid semirobot password 12345678
nmcli dev wifi show-password
sudo nmcli connection down MyHostspot && sudo nmcli connection up MyHostspot
```

## 查看ssh的状态
```sh
sudo systemctl status ssh
sudo systemctl start ssh
sudo systemctl enable ssh

```

## 开机自启动配置
```sh
chmod +x auto_start_config.sh
sudo cp start_config.service /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable start_config.service
sudo systemctl disable start_config.service
sudo systemctl start start_config.service
sudo systemctl stop start_config.service
systemctl status start_config.service
```


