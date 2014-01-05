#!/bin/bash

#判断当前目录是否是linux tree#
if [ -f "README" ]
then
	echo "========== File 'README' exist"
else
	echo "========== Not in kernel"
	exit
fi
 
iskernel=`grep "Linux kernel release" README | wc -l`
if [ $iskernel -eq 0 ]
then
	echo "========== Not in kernel"
	exit
else
	echo "========== IN Linux kernel"
fi

Inserted=`find ./ -name s2e.h|wc -l`
if [ $Inserted -gt 0 ]
then
	echo 'this kernel have inserted'
	echo '直接开始测试...'
	find ../ -name startTest.sh | bash
	exit
fi

cpuNum=`cat /proc/cpuinfo |grep processor|wc -l`
Kernel=$PWD
echo ${Kernel}
export PATH=$PATH:${Kernel}/../kint/kint/
export PATH=$PATH:${Kernel}/../kint/kint/kint/build/bin

#======================================================
#修正linux kernel错误
grep -v 'BUILD_BUG_ON(!__builtin_constant_p(offset))' include/linux/rcupdate.h > include/linux/rcupdate.h.new 
mv include/linux/rcupdate.h.new include/linux/rcupdate.h
sed -i "s/long tmp/int tmp/g" arch/x86/include/asm/bitops.h

#=====================================================
#使用kint 编译
make allyesconfig
kint-build make -j $cpuNum

