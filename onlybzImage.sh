#!/bin/bash
cpuNum=`cat /proc/cpuinfo |grep processor|wc -l`
Kernel=$PWD
echo ${Kernel}
export PATH=$PATH:${Kernel}/../kint/kint/
export PATH=$PATH:${Kernel}/../kint/kint/kint/build/bin

#=========================================================
#rm 编译产生文件
make mrproper

make ARCH=i386 defconfig
make ARCH=i386 bzImage -j $cpuNum
