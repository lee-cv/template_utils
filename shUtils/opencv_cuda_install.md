## 一.安装CUDA
```sh

```

### 0.安装Ubuntu内核和显卡驱动

### 1.先看看系统的情况
```sh
sudo apt update
sudo apt upgrade
sudo apt install linux-headers-$(uname -r)

/etc/modprobe.d/blacklist-nouveau.conf:
blacklist nouveau
options nouveau modeset=0

sudo update-initramfs -u

lspci | grep -i nvidia
nvidia-smi

```

### 2.下载cuda文件(https://developer.nvidia.com/cuda-downloads),deb或者run文件都可以
```sh
wget https://developer.download.nvidia.com/compute/cuda/11.8.0/local_installers/cuda_11.8.0_520.61.05_linux.run
sudo sh cuda_11.8.0_520.61.05_linux.run --toolkit --silent --override

# NVIDIA CUDA Toolkit
export CUDA_HOME=/usr/local/cuda-11.8
export LD_LIBRARY_PATH=${CUDA_HOME}/lib64:$LD_LIBRARY_PATH
export PATH=${CUDA_HOME}/bin:${PATH}

source ~/.bashrc
nvcc -V

git clone https://github.com/NVIDIA/cuda-samples.git
cd cuda-samples/Samples/1_Utilities/deviceQuery/
make
./deviceQuery
```

## 2. 安装cuDnn(https://developer.nvidia.com/cudnn)
```sh
sudo apt install zlib1g
sudo dpkg -i cudnn-local-repo-ubuntu2004-8.6.0.163_1.0-1_amd64.deb

```



### 查找显卡的架构版本(https://developer.nvidia.com/cuda-gpus)
