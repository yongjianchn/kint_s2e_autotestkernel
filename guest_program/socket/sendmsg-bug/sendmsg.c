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

int main(int ac,char **av)
{
struct msghdr msghdr;
struct iovec iovector[10];
int i,s;
struct sockaddr_in sockad; 
char msg[128];
struct cmsghdr *cmsg,*cm2; 
char opts[4];

s=socket(PF_INET, /*SOCK_STREAM*/ SOCK_DGRAM, 0);
sockad.sin_family = AF_INET;
sockad.sin_addr.s_addr=inet_addr("127.0.0.1");
sockad.sin_port=htons(8080); 

connect(s,(struct sockaddr *) &sockad, sizeof(sockad)); 

memset(msg,'v',sizeof(msg));
memset(opts,0,sizeof(opts));
#define VV 1024*1024
cmsg = malloc(VV);
memset(cmsg,0,VV);

cmsg->cmsg_len = sizeof(struct cmsghdr) + sizeof(opts);
cmsg->cmsg_level = SOL_IP;
cmsg->cmsg_type = IP_RETOPTS;
memcpy(CMSG_DATA(cmsg), opts, sizeof(opts));
/*
cm2= (struct cmsghdr *) (long) ((char *)CMSG_DATA(cmsg)+sizeof(opts));
cm2->cmsg_len = sizeof(struct cmsghdr); 
cm2->cmsg_level = SOL_IP;
cm2->cmsg_type = IP_RETOPTS;
*/
msghdr.msg_name = &sockad;
msghdr.msg_namelen = sizeof(sockad);

msghdr.msg_control=cmsg;
msghdr.msg_controllen= cmsg->cmsg_len + 14; 
msghdr.msg_iov = iovector;

msghdr.msg_iovlen = 1;
iovector[0].iov_base = msg;
iovector[0].iov_len = sizeof(msg);
system("sync");
s2e_enable_forking();
//s2e_make_symbolic(&(msghdr.msg_controllen),4,"ctrlen");
//s2e_make_symbolic(cmsg,24, "all-24");
//s2e_make_symbolic(cmsg, 12, "12-1");
s2e_make_symbolic((char *)cmsg , 100, "12-2");
//s2e_make_symbolic(cmsg, 16, "16");
if ((i = sendmsg(s, &msghdr, 0)) < 0)
perror("sendmsg");
s2e_disable_forking();
s2e_kill_state(0,"done\n");
return 42;
}
