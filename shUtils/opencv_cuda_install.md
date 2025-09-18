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
nvidia-smi -L

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

### 3. 安装cuDnn(https://developer.nvidia.com/cudnn)
```sh
sudo apt install zlib1g
sudo dpkg -i cudnn-local-repo-ubuntu2004-8.6.0.163_1.0-1_amd64.deb
sudo cp /var/cudnn-local-repo-ubuntu2004-8.6.0.163/cudnn-local-B0FE0A41-keyring.gpg /usr/share/keyrings/
sudo apt-key add /var/cudnn-local-repo-ubuntu2004-8.6.0.163/B0FE0A41.pub

sudo apt-get update
sudo apt-get install libcudnn8=8.6.0.163-1+cuda11.8
sudo apt-get install libcudnn8-dev=8.6.0.163-1+cuda11.8
sudo apt-get install libcudnn8-samples=8.6.0.163-1+cuda11.8

git clone https://github.com/johnpzh/cudnn_samples_v8.git
cd cudnn_samples_v8/mnistCUDNN/
make clean && make
```

### 4.安装其他
```sh
sudo apt install libavcodec-dev libavformat-dev libswscale-dev
sudo apt install libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev
sudo apt install libgtk-3-dev
sudo apt install libpng-dev libjpeg-dev libopenexr-dev libtiff-dev libwebp-dev
```

### 5.下载编译OpenCV(https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
```sh
mkdir OpenCV && cd OpenCV
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.x.zip
unzip opencv.zip
unzip opencv_contrib.zip
sudo rm -rf ./*zip

mkdir -p build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=../opencv_cuda -D WITH_CUDA=ON -D WITH_CUDNN=ON -D WITH_CUBLAS=ON -D WITH_TBB=ON -D OPENCV_DNN_CUDA=ON -D OPENCV_ENABLE_NONFREE=ON -D CUDA_ARCH_BIN=8.9 -D BUILD_EXAMPLES=OFF -D HAVE_opencv_python3=OFF -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.x/modules  ../opencv-4.x

make -j$(nproc)
make install

```

### 查找显卡的架构版本(https://developer.nvidia.com/cuda-gpus)

### 扩展：安装tensorrt
```sh
sudo dpkg -i nv-tensorrt-local-repo-ubuntu2004-8.6.1-cuda-11.8_1.0-1_amd64.deb
sudo cp /var/nv-tensorrt-local-repo-ubuntu2004-8.6.1-cuda-11.8/nv-tensorrt-local-D7BB1B18-keyring.gpg /usr/share/keyrings/
sudo apt-get update

wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/cuda-ubuntu2004.pin
sudo mv cuda-ubuntu2004.pin /etc/apt/preferences.d/cuda-repository-pin-600
wget https://developer.download.nvidia.com/compute/cuda/11.8.0/local_installers/cuda-repo-ubuntu2004-11-8-local_11.8.0-520.61.05-1_amd64.deb
sudo dpkg -i cuda-repo-ubuntu2004-11-8-local_11.8.0-520.61.05-1_amd64.deb
sudo cp /var/cuda-repo-ubuntu2004-11-8-local/cuda-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update

sudo apt-get install tensorrt
```



