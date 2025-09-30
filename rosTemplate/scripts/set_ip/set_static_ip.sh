#!/bin/bash

# 固定 IP 参数
IFACE="eth0"            # 网口名称
IPADDR="192.168.3.5"  # 需要设置的 IP
NETMASK="24"            # 子网掩码（24 表示 255.255.255.0）
GATEWAY="192.168.3.1"   # 网关
DNS="8.8.8.8"           # DNS 服务器

# Netplan 配置文件路径（一般在 /etc/netplan/ 下）
NETPLAN_FILE="/etc/netplan/01-network-manager-all.yaml"

# 生成 netplan 配置
cat <<EOF | sudo tee $NETPLAN_FILE
network:
  version: 2
  ethernets:
    $IFACE:
      dhcp4: no
      addresses: [$IPADDR/$NETMASK]
      gateway4: $GATEWAY
      nameservers:
        addresses: [$DNS]
EOF

# 应用配置
sudo netplan apply
echo "固定 IP 已设置：$IPADDR"
