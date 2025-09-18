#!/bin/bash
#ssh-keygen 生成密室，密码一致不需要重新生成
#ssh-copy-id marvsmart@10.42.0.1 将密室拷贝远程

#git免账号密码输入，在git输入账号密码后，输入以下指令，后续不再需要输入密码
#git config --global credential.helper store 
#git config --global user.name "marvsmart"
#git config --global user.email "marvsmart@foxmail.com"
username=$(whoami)
echo -e "\033[31m 当前用户名为:$username \033[0m"

if [ ! -e "/home/$username/.ssh/id_rsa" ]; then

    ssh-keygen
    echo -e "\033[31m 文件已创建: $1 \033[0m" 
else
    # 文件已存在
    ssh-copy-id marvsmart@10.42.0.1
    echo -e "\033[31m 文件已存在: $1 \033[0m"
fi

local_time=$(date '+%Y-%m-%d %H:%M:%S')

remote_user="marvsmart"
remote_host="10.42.0.1"

ssh "${remote_user}@${remote_host}" "sudo date -s '${local_time}'"
ssh "${remote_user}@${remote_host}" "sudo hwclock --systohc --localtime"
ssh "${remote_user}@${remote_host}" "sudo hwclock --hctosys"

global=$(ssh "${remote_user}@${remote_host}" "sudo hwclock --show")
local=$(ssh "${remote_user}@${remote_host}" "sudo hwclock --localtime --show")

if [ "${global:0:16}" == "${local:0:16}" ]; then
    echo "远程主机时间已成功设置为: ${local_time}"
else
    echo "设置远程主机时间失败"
fi



