grep " __kmalloc" System.map | grep -v __kmalloc_|cut -d ' ' -f 1
