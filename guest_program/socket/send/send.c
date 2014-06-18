#include "s2e.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void main()
{
	s2e_enable_forking();
	int s;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;//?
//	s2e_make_symbolic(&(addr.sin_family), sizeof(addr.sin_family), "sin_family");
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port=htons(8080);

	int socket1 = PF_INET;//?
	int sockettype = SOCK_DGRAM;//?
	int socket3 = 0;//?
//	s2e_make_symbolic(&socket1, sizeof(socket1), "socket1");
//	s2e_make_symbolic(&sockettype, sizeof(sockettype), "sockettype");
//	s2e_make_symbolic(&socket3, sizeof(socket3), "socket3");
	s = socket(socket1, sockettype, socket3);//---socket

	int level = SOL_SOCKET;//?
	int options = SO_REUSEADDR;//?
	char* val = malloc(4);
	memset(val, 't', sizeof(val));
	int setlen = sizeof(val);//?
//	s2e_make_symbolic(&level, sizeof(level), "set_level");
//	s2e_make_symbolic(&options, sizeof(options), "set_options");
//	s2e_make_symbolic(&setlen, sizeof(setlen), "set_len");
	setsockopt(s, level, options, &val, setlen);//---setsockopt

	int alen = sizeof(addr);//?
//	s2e_make_symbolic(&alen, sizeof(alen), "bind_len");
	bind(s, &addr, alen);//---bind

	int addr_size = sizeof(addr);//?
//	s2e_make_symbolic(&addr_size, sizeof(addr_size), "connet_addr_size");
	connect(s, &addr, addr_size);//---connect

	int qlen = sizeof(addr);//?
//	s2e_make_symbolic(&qlen, sizeof(qlen), "listen_len");
	listen(s, qlen);//---listen

	int addr_size2 = sizeof(addr);//?
//	s2e_make_symbolic(&addr_size2, sizeof(int), "accept_addr_size");
	accept(s, &addr, addr_size2);//---accept

	char* buf = malloc(4);
	memset(buf,'v',sizeof(buf));
	int nbytes = sizeof(buf);//?
	int flags = MSG_DONTROUTE;//?
	s2e_make_symbolic(&nbytes, sizeof(int), "send_len");
	s2e_make_symbolic(&flags, sizeof(int), "send_flags");
	send(s, buf, nbytes, flags);///---send
/*
	int clen = sizeof(addr);//?
	s2e_make_symbolic(&clen, sizeof(int), "sendto_addr_len");
	sendto(s, buf, nbytes, flags, &addr, clen);//sendto

	char* buf_recv = malloc(10);
	int recv_len = sizeof(buf_recv);//?
	int recv_flag = MSG_OOB;//?
	s2e_make_symbolic(&recv_len, sizeof(int), "recv_len");
	s2e_make_symbolic(&recv_flag, sizeof(int), "recv_flag");
	recv(s, buf_recv, recv_len, recv_flag);//---recv
*/
	close(s);
	s2e_disable_forking();
	s2e_kill_state(0, "program done!");
}
