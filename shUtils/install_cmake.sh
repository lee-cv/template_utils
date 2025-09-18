#!/bin/bash
set -e

CMAKE_VERSION=3.28.0
INSTALL_PREFIX=/usr/local

echo "=== Step 1: 清理旧的 cmake ==="
sudo rm -f $INSTALL_PREFIX/bin/cmake
sudo rm -f $INSTALL_PREFIX/bin/ccmake
sudo rm -f $INSTALL_PREFIX/bin/cmake-gui
sudo rm -f $INSTALL_PREFIX/bin/cpack
sudo rm -f $INSTALL_PREFIX/bin/ctest
sudo rm -rf $INSTALL_PREFIX/share/cmake-3.*

echo "=== Step 2: 下载 CMake $CMAKE_VERSION 源码包 ==="
cd /tmp
wget -q https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION.tar.gz -O cmake-$CMAKE_VERSION.tar.gz
tar -xf cmake-$CMAKE_VERSION.tar.gz
cd cmake-$CMAKE_VERSION

echo "=== Step 3: 配置编译 ==="
./bootstrap --prefix=$INSTALL_PREFIX

echo "=== Step 4: 编译并安装 ==="
make -j$(nproc)
sudo make install

echo "=== Step 5: 验证安装 ==="
which cmake
cmake --version

