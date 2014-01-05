waitfor=1200
while [ $waitfor -gt 0 ]
do
	sleep 1
	let waitfor=${waitfor}-1
	let left=${waitfor}%10
	if [ 0 -eq $left ]
	then
		echo "${waitfor}S left..."
	fi
	stillrunning=`ps x|grep qemu|grep "../img/debian6.0.5.qcow2"|wc -l`
	if [ 0 -eq $stillrunning ]
	then
		echo "OK, this test done!..."
		exit
	fi
done

kill -9 `ps x|grep qemu|grep "../img/debian6.0.5.qcow2"|awk '{print $1}'`
