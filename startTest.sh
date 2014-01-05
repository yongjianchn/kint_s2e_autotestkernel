#!/bin/bash

#判断当前目录是否是linux tree#
if [ -f "README" ]
then
	echo "check......"
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
	echo "IN Linux kernel..."
fi


nowPath=`pwd`
testsPath="${nowPath}/../tests"
readyPath="${testsPath}"
donePath="${testsPath}"

s2e='../s2e/build/qemu-release/i386-s2e-softmmu/qemu-system-i386'
img='../img/debian6.0.5.qcow2'
bzImage='arch/x86/boot/bzImage'
s2eConfig='../config/test.lua'

#判断bzImage是否存在
if [ -f $bzImage ]
then
	echo "bzImage   ready..."
else
	echo "$bzImage not exist, check it please!"
	exit
fi

#判断s2e是否存在
if [ -f $s2e ]
then
	echo "s2e       ready..."
else
	echo "$s2e not exist, check it please!"
	exit
fi

#判断s2eConfig是否存在
if [ -f $s2eConfig ]
then
	echo "s2econfig ready..."
else
	echo "$s2eConfig not exist, check it please!"
	exit
fi

#判断img是否存在
if [ -f $img ]
then
	echo "img       ready..."
else
	echo "$img not exist, check it please!"
	exit
fi

#判断testcases是否存在
if [ -d $testsPath ] && [ -d $readyPath ]
then
	echo "testcases ready..."
else
	echo "no tests"
	exit
fi

while [ 1 -eq 1 ]
do
	echo '==========>>>'
	haveTestsNum=`find $readyPath -type f -name 'test'| wc -l`
	echo "haveTestsNum: $haveTestsNum"
	if [ $haveTestsNum -gt 0 ]
	then
		nowTest=`find $readyPath -type f -name 'test' | head -n 1`
		echo "nowTest: $nowTest"
		#得到当前测试用例所在目录
		nowTestPath=`echo ${nowTest}hahaha | sed 's/testhahaha//'`
		echo "configuring TestPath..."
		sed -i "s#^.*baseDirs.*#\tbaseDirs = {\"${nowTestPath}\"},#" $s2eConfig
		echo testing......
		bash ../timer.sh & 
#		$s2e -kernel $bzImage -hda $img -append "root=/dev/sda1 text" -s2e-config-file $s2eConfig -vnc :7
		$s2e $img -loadvm runtest -s2e-config-file $s2eConfig -vnc :55
		sleep 2
		mv $nowTest "${nowTest}done"
	else
		echo "测试程序全部测试完成!"
		break
	fi
done

while [ 1 -eq 1 ]
do
	doneTestsNum=`find $donePath -type f -name 'testdone'| wc -l`
#	echo "doneTestsNum: $doneTestsNum"
	if [ $doneTestsNum -gt 0 ]
	then
		nowTest=`find $donePath -type f -name 'testdone' | head -n 1`
		mv $nowTest `echo ${nowTest}hahaha | sed 's/donehahaha//'` 
	else
		echo "测试程序全部复位!"
		break
	fi
done
