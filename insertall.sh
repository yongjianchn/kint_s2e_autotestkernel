
Kernel=$PWD
export PATH=$PATH:${Kernel}/../kint/kint/
export PATH=$PATH:${Kernel}/../kint/kint/kint/build/bin
		pintck
		insert
#插入s2e.h
mkdir include/s2e
cp ../config/s2e.h include/s2e/
