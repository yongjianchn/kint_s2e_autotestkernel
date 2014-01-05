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

#=======================================================
#开始插入结果
for toInsertDir in net/ipv4 net/core kernel/events
	do 
		cd $toInsertDir
		pwd
		pintck
		insert
		cd $Kernel 
	done
#插入s2e.h
mkdir include/s2e
cp $Kernel/../config/s2e.h $Kernel/include/s2e/

#=========================================================
#rm 编译产生文件
cd $Kernel
make mrproper

#==========================================================
#删除kint产生的文件,要不要删除呢？现在没删除
llNum=0
llNum=`find ./ -name "*.ll"|wc -l`
if [ $llNum -gt 0 ]
then
	echo 'rm ll'
#	find ./ -name "*.ll" | xargs rm
fi

lloutNum=0
lloutNum=`find ./ -name "*.ll.out"|wc -l`
if [ $lloutNum -gt 0 ]
then
	echo 'rm ll.out'
#	find ./ -name "*.ll.out" | xargs rm
fi

pintckNum=0
pintckNum=`find ./ -name "pintck.txt"|wc -l`
if [ $pintckNum -gt 0 ]
then
	echo 'rm pintck'
#	find ./ -name "pintck.txt" | xargs rm
fi

#============================================================
#打包到../backup/s2ekernel.tar.gz, 为另一种方案做的准备，现在没用到
#if [ -f "${Kernel}/../../backup/s2ekernel.tar.gz" ]
#then
#	echo 'rm old ../backup/s2ekernel.tar.gz'
#	rm "${Kernel}/../backup/s2ekernel.tar.gz"
#fi
#
#tar czf "${Kernel}/../backup/s2ekernel.tar.gz" ${Kernel} \
#	--exclude *.ll \
#	--exclude *.ll.out \
#	--exclude pintck.txt
#echo "插桩后的内核:${Kernel}/../backup/s2ekernel.tar.gz"
#
#=========================================================
#编译插桩后的内核，目的是使用bzImage启动测试
make ARCH=i386 defconfig
make ARCH=i386 bzImage -j $cpuNum

#===============开始测试
find ../ -name startTest.sh | bash
