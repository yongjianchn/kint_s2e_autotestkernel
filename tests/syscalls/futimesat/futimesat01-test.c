/******************************************************************************
 *
 *   Copyright (c) International Business Machines  Corp., 2006
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * NAME
 *      futimesat01.c
 *
 * DESCRIPTION
 *	This test case will verify basic function of futimesat
 *	added by kernel 2.6.16 or up.
 *
 * USAGE:  <for command-line>
 * futimesat01 [-c n] [-e] [-i n] [-I x] [-P x] [-t] [-p]
 * where:
 *      -c n : Run n copies simultaneously.
 *      -e   : Turn on errno logging.
 *      -i n : Execute test n times.
 *      -I x : Execute test for x seconds.
 *      -p   : Pause for SIGUSR1 before starting
 *      -P x : Pause for x seconds between iterations.
 *      -t   : Turn on syscall timing.
 *
 * Author
 *	Yi Yang <yyangcdl@cn.ibm.com>
 *
 * History
 *      08/24/2006      Created first by Yi Yang <yyangcdl@cn.ibm.com>
 *
 *****************************************************************************/

#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <error.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include "test.h"
#include "usctest.h"
#include "linux_syscall_numbers.h"
#include "s2e.h"

#define TEST_CASES 5
#ifndef AT_FDCWD
#define AT_FDCWD -100
#endif
void setup();
void cleanup();
void setup_every_copy();

char *TCID = "futimesat01";
int TST_TOTAL = TEST_CASES;
char pathname[256];
char testfile[256];
char testfile2[256];
char testfile3[256];
int dirfd, fd, ret;
int fds[TEST_CASES];
char *filenames[TEST_CASES];
int expected_errno[TEST_CASES] = { 0, 0, ENOTDIR, EBADF, 0 };

struct timeval times[2];

int myfutimesat(int dirfd, const char *filename, struct timeval *times)
{
	return ltp_syscall(__NR_futimesat, dirfd, filename, times);
}

int main(int ac, char **av)
{
	int lc;
	char *msg;
	int i;

	/* Disable test if the version of the kernel is less than 2.6.16 */
	if ((tst_kvercmp(2, 6, 16)) < 0) {
		tst_resm(TWARN, "This test can only run on kernels that are ");
		tst_resm(TWARN, "2.6.16 and higher");
		exit(0);
	}

	/***************************************************************
	 * parse standard options
	 ***************************************************************/
	if ((msg = parse_opts(ac, av, NULL, NULL)) != NULL)
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);

	/***************************************************************
	 * perform global setup for test
	 ***************************************************************/
	setup();

	/***************************************************************
	 * check looping state if -c option given
	 ***************************************************************/
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		setup_every_copy();

		tst_count = 0;

		/*
		 * Call futimesat
		 */
		for (i = 0; i < TST_TOTAL; i++) {
			gettimeofday(&times[0], NULL);
			gettimeofday(&times[1], NULL);
			s2e_enable_forking();
			s2e_make_symbolic(&fds[i], sizeof(int), "fds[i]");
			s2e_make_symbolic(filenames[i], 10, "filenames");
			s2e_make_symbolic(times, sizeof(struct timeval) * 2, "times");
			TEST(myfutimesat(fds[i], filenames[i], times));
			s2e_disable_forking();
			s2e_kill_state(0, "program done");

			/* check return code */
			if (TEST_ERRNO == expected_errno[i]) {

			/***************************************************************
			 * only perform functional verification if flag set (-f not given)
			 ***************************************************************/
				if (STD_FUNCTIONAL_TEST) {
					/* No Verification test, yet... */
					tst_resm(TPASS,
						 "futimesat() returned the expected  errno %d: %s",
						 TEST_ERRNO,
						 strerror(TEST_ERRNO));
				}
			} else {
				TEST_ERROR_LOG(TEST_ERRNO);
				tst_resm(TFAIL,
					 "futimesat() Failed, errno=%d : %s",
					 TEST_ERRNO, strerror(TEST_ERRNO));
			}
		}

	}

	/***************************************************************
	 * cleanup and exit
	 ***************************************************************/
	cleanup();

	return (0);
}

void setup_every_copy()
{
	/* Initialize test dir and file names */
	sprintf(pathname, "futimesattestdir%d", getpid());
	sprintf(testfile, "futimesattestfile%d.txt", getpid());
	sprintf(testfile2, "futimesattestdir%d/futimesattestfile%d.txt",
		getpid(), getpid());
	sprintf(testfile3, "/tmp/futimesattestfile%d.txt", getpid());

	ret = mkdir(pathname, 0700);
	if (ret < 0) {
		perror("mkdir: ");
		exit(-1);
	}

	dirfd = open(pathname, O_DIRECTORY);
	if (dirfd < 0) {
		perror("open: ");
		exit(-1);
	}

	fd = open(testfile, O_CREAT | O_RDWR, 0600);
	if (fd < 0) {
		perror("open: ");
		exit(-1);
	}

	fd = open(testfile2, O_CREAT | O_RDWR, 0600);
	if (fd < 0) {
		perror("open: ");
		exit(-1);
	}

	fd = open(testfile3, O_CREAT | O_RDWR, 0600);
	if (fd < 0) {
		perror("open: ");
		exit(-1);
	}

	fds[0] = fds[1] = dirfd;
	fds[2] = fd;
	fds[3] = 100;
	fds[4] = AT_FDCWD;

	filenames[0] = filenames[2] = filenames[3] = filenames[4] = testfile;
	filenames[1] = testfile3;
}

/***************************************************************
 * setup() - performs all ONE TIME setup for this test.
 ***************************************************************/
void setup()
{

	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	TEST_PAUSE;
}

/***************************************************************
 * cleanup() - performs all ONE TIME cleanup for this test at
 *		completion or premature exit.
 ***************************************************************/
void cleanup()
{
	/* Remove them */
	unlink(testfile2);
	unlink(testfile3);
	unlink(testfile);
	rmdir(pathname);

	/*
	 * print timing stats if that option was specified.
	 * print errno log if that option was specified.
	 */
	TEST_CLEANUP;

}
