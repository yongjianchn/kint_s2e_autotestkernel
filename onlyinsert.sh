
Kernel=$PWD
export PATH=$PATH:${Kernel}/../kint/kint/
export PATH=$PATH:${Kernel}/../kint/kint/kint/build/bin
for toInsertDir in net/ipv4 net/core kernel/events
	do 
		cd $toInsertDir
		pwd
		pintck
		insert
		cd - 
	done
#插入s2e.h
mkdir include/s2e
cp ../config/s2e.h include/s2e/
