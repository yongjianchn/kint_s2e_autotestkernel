all:
	for tododir in creat dup\&dup2 fcntl flock open open\&flock pread\&pwrite readv\&writev read\&write truncate\&ftruncate; \
		do \
		cd $$tododir; make; cd ..; \
		done

