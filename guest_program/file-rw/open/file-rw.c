#include <fcntl.h>
#include <sys/uio.h>
#include <sys/file.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "s2e.h"

#define FILENAME "test.FILENAME"
#define WRITE_LEN 10

int main()
{

	char *s;
	s = malloc(WRITE_LEN);
	memset(s,'*',WRITE_LEN);

	char *new;
	new = malloc(1024);
	memset(new,'*',1024);


s2e_enable_forking();
	//creat
	int creat_fd;
	int creat_perm = 0777;
//s2e_make_symbolic(&creat_perm, sizeof(creat_perm), "creat_perm");
	creat_fd = creat(FILENAME, creat_perm);//-1
	//dup
	int dup_fd;
	dup_fd = dup(creat_fd);//-2.1
	//dup2
	int dup2_fd;
//s2e_make_symbolic(&dup2_fd, sizeof(dup2_fd), "dup2_fd");
	dup2(creat_fd,dup2_fd);//-2.2
	close(creat_fd);//-end-1
	close(dup_fd);//-end-2.1
	close(dup2_fd);//-end-2.2
	//open
	int open_fd;
	int open_flag = O_RDWR;
	int open_mode = 0700;
s2e_make_symbolic(&open_flag, sizeof(open_flag), "open_flag");
s2e_make_symbolic(&open_mode, sizeof(open_mode), "open_mode");
	open_fd = open(FILENAME, open_flag, open_mode);//-3
	close(open_fd);//-end-3
	open_fd = open(FILENAME, open_flag);//-4
	close(open_fd);//-end-4
	//fcntl
	int fcntl_cmd;
	long fcntl_arg;
	struct flock fcntl_lock;
	/*
	truct flock
	    {
	      short int l_type;
	      short int l_whence;
	#ifndef __USE_FILE_OFFSET64
	      __off_t l_start;
	      __off_t l_len;
	#else
	      __off64_t l_start;
	      __off64_t l_len;
	#endif
	      __pid_t l_pid;
	    };
	*/
//s2e_make_symbolic(&fcntl_cmd, sizeof(fcntl_cmd), "fcntl_cmd");
//s2e_make_symbolic(&fcntl_arg, sizeof(fcntl_arg), "fcntl_arg");
//s2e_make_symbolic(&(fcntl_lock.l_type), sizeof(fcntl_lock.l_type), "fcntl_lock.l_type");
//s2e_make_symbolic(&(fcntl_lock.l_whence), sizeof(fcntl_lock.l_whence), "fcntl_lock.l_whence");
//s2e_make_symbolic(&(fcntl_lock.l_start), sizeof(fcntl_lock.l_start), "fcntl_lock.l_start");
//s2e_make_symbolic(&(fcntl_lock.l_len), sizeof(fcntl_lock.l_len), "fcntl_lock.l_len");
	fcntl(open_fd, fcntl_cmd); 
	fcntl(open_fd, fcntl_cmd, fcntl_arg); 
	fcntl(open_fd, fcntl_cmd, &fcntl_lock); 

	//write
	int write_len = WRITE_LEN;
//s2e_make_symbolic(&write_len, sizeof(write_len), "write_len");
	open_fd = open(FILENAME, O_RDWR);//-5
	write(open_fd,s,write_len);
	close(open_fd);//-end-5

	//read
	int read_len = WRITE_LEN;
//s2e_make_symbolic(&read_len, sizeof(read_len), "read_len");
	open_fd = open(FILENAME, O_RDWR);//-6
	read(open_fd, s, read_len);
	close(open_fd);//-end-6

	//writev
	char writev_buf[30];
	int writev_iov_len = 20;
	struct iovec writev_iovec = {writev_buf, writev_iov_len};
	int writev_cont = 10;
//s2e_make_symbolic(&writev_iov_len, sizeof(writev_iov_len), "writev_iov_len");
//s2e_make_symbolic(&writev_cont, sizeof(writev_cont), "writev_cont");
	open_fd = open(FILENAME, O_RDWR);//-7
	writev(open_fd, &writev_iovec, writev_cont);
	close(open_fd);//-end-7

	//readv
	char readv_buf[30];
	int readv_iov_len = 20;
	struct iovec readv_iovec = {readv_buf, readv_iov_len};
	int readv_cont = 10;
//s2e_make_symbolic(&readv_iov_len, sizeof(readv_iov_len), "readv_iov_len");
//s2e_make_symbolic(&readv_cont, sizeof(readv_cont), "writev_cont");
	open_fd = open(FILENAME, O_RDWR);//-8
	readv(open_fd, &readv_iovec, readv_cont);
	close(open_fd);//-end-8

	//pwrite
	char pwrite_buf[30];
	int pwrite_count = 10;
	int pwrite_offset = 20;
	open_fd = open(FILENAME, O_RDWR);//-9
//s2e_make_symbolic(&pwrite_count, sizeof(pwrite_count), "pwrite_count");
//s2e_make_symbolic(&pwrite_offset, sizeof(pwrite_offset), "pwrite_offset");
	pwrite(open_fd, pwrite_buf, pwrite_count, pwrite_offset);
	close(open_fd);//-end-9
	//pread
	char pread_buf[30];
	int pread_count = 10;
	int pread_offset = 20;
	open_fd = open(FILENAME, O_RDWR);//-10
//s2e_make_symbolic(&pread_count, sizeof(pread_count), "pread_count");
//s2e_make_symbolic(&pread_offset, sizeof(pread_offset), "pread_offset");
	pread(open_fd, pread_buf, pread_count, pread_offset);
	close(open_fd);//-end-10

	//ftruncate & truncate
	int truncate_offset = 100;
	open_fd = open(FILENAME, O_RDWR);//-11
	write(open_fd, new, 1024);
//s2e_make_symbolic(&truncate_offset, sizeof(truncate_offset), "truncate_offset");
	ftruncate(open_fd, truncate_offset);
	close(open_fd);//-end-11
	open_fd = open(FILENAME, O_RDWR);//-12
	write(open_fd, new, 1024);
	close(open_fd);//-end-12
	truncate(FILENAME, truncate_offset);

	//flock
	int flock_op;
	open_fd = open(FILENAME, O_RDWR);//-13
//s2e_make_symbolic(&flock_op, sizeof(flock_op), "flock_op");
	flock(open_fd, flock_op);
	close(open_fd);//-end-13
	
	unlink(FILENAME);//删除测试产生的文件

s2e_disable_forking();
s2e_kill_state(0, "file-rw test done!");
}
