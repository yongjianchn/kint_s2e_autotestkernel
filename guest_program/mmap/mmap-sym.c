#include "s2e.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int ac,char **av)
{
	system("echo \"ABCDEFG123\" > demo.txt");

	char *filename = "demo.txt";
	int length = 10;
	int prot = PROT_READ;
	int flags = MAP_PRIVATE;
	int offset = 0;
	char *addr;

	int fd = open(filename, O_RDONLY);
	if (fd == -1)
		exit(0);

	s2e_enable_forking();
	s2e_make_symbolic(&(length),sizeof(length), "symbolic_var_length");
	s2e_make_symbolic(&(prot),sizeof(prot), "symbolic_var_prot");
	s2e_make_symbolic(&(flags),sizeof(flags), "symbolic_var_flags");
	s2e_make_symbolic(&(offset),sizeof(offset), "symbolic_var_offset");

	addr = mmap(NULL, length, prot, flags, fd, offset);
	write(STDOUT_FILENO, addr, length);

	s2e_disable_forking();
	close(fd);
	s2e_kill_state(0,"program done!");

	/*
	   s2e_enable_forking();
	   s2e_make_symbolic(&(x),sizeof(x), "symbolic_var_x");
	   s2e_disable_forking();
	   s2e_kill_state(0,"program done!");
	   return 0;
	*/
}
