/*
 *
 *   Copyright (c) International Business Machines  Corp., 2001
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
 */

/*
 * NAME
 *	open02.c
 *
 * DESCRIPTION
 *	Test if open without O_CREAT returns -1 if a file does not exist.
 *
 * ALGORITHM
 *	1. open a new file without O_CREAT, test for return value of -1
 *
 * USAGE:  <for command-line>
 *  open02 [-c n] [-e] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -e   : Turn on errno logging.
 *             -i n : Execute test n times.
 *             -I x : Execute test for x seconds.
 *             -P x : Pause for x seconds between iterations.
 *             -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *
 * RESTRICTIONS
 *	None
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/fcntl.h>
#include "test.h"
#include "usctest.h"

char *TCID = "open02";
int TST_TOTAL = 1;

static char pfilname[40] = "";

static int exp_enos[] = { ENOENT, 0 };

static void cleanup(void);
static void setup(void);

int main(int ac, char **av)
{
	int lc;
	char *msg;

	/*
	 * parse standard command line options
	 */
	msg = parse_opts(ac, av, NULL, NULL);
	if (msg != NULL)
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);

	setup();

	TEST_EXP_ENOS(exp_enos);

	/*
	 * check looping state if -i option given on the command line
	 */
	for (lc = 0; TEST_LOOPING(lc); lc++) {
		tst_count = 0;	/* reset tst_count while looping. */

		TEST(open(pfilname, O_RDWR, 0444));

		if (TEST_RETURN != -1) {
			tst_resm(TFAIL, "opened non-existent file");
			continue;
		}

		TEST_ERROR_LOG(TEST_ERRNO);

		if (TEST_ERRNO != ENOENT) {
			tst_resm(TFAIL, "open(2) set invalid errno: "
				 "expected ENOENT, got %d", TEST_ERRNO);
		} else {
			tst_resm(TPASS, "open returned ENOENT");
		}
	}

	cleanup();
	tst_exit();
}

static void setup(void)
{
	umask(0);

	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	TEST_PAUSE;

	tst_tmpdir();

	sprintf(pfilname, "./open3.%d", getpid());
}

static void cleanup(void)
{
	TEST_CLEANUP;
	tst_rmdir();
}
