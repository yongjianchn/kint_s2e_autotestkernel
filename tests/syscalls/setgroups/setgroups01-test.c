/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 *
 * http://www.sgi.com
 *
 * For further information regarding this notice, see:
 *
 * http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */
/* $Id: setgroups01.c,v 1.7 2009/11/02 13:57:18 subrata_modak Exp $ */
/**********************************************************
 *
 *    OS Test - Silicon Graphics, Inc.
 *
 *    TEST IDENTIFIER	: setgroups01
 *
 *    EXECUTED BY	: root
 *
 *    TEST TITLE	: Basic test for setgroups(2)
 *
 *    PARENT DOCUMENT	: usctpl01
 *
 *    TEST CASE TOTAL	: 1
 *
 *    WALL CLOCK TIME	: 1
 *
 *    CPU TYPES		: ALL
 *
 *    AUTHOR		: William Roske
 *
 *    CO-PILOT		: Dave Fenner
 *
 *    DATE STARTED	: 03/30/92
 *
 *    INITIAL RELEASE	: UNICOS 7.0
 *
 *    TEST CASES
 *
 * 	1.) setgroups(2) returns...(See Description)
 *
 *    INPUT SPECIFICATIONS
 * 	The standard options for system call tests are accepted.
 *	(See the parse_opts(3) man page).
 *
 *    OUTPUT SPECIFICATIONS
 *$
 *    DURATION
 * 	Terminates - with frequency and infinite modes.
 *
 *    SIGNALS
 * 	Uses SIGUSR1 to pause before test if option set.
 * 	(See the parse_opts(3) man page).
 *
 *    RESOURCES
 * 	None
 *
 *    ENVIRONMENTAL NEEDS
 *      No run-time environmental needs.
 *
 *    SPECIAL PROCEDURAL REQUIREMENTS
 * 	None
 *
 *    INTERCASE DEPENDENCIES
 * 	None
 *
 *    DETAILED DESCRIPTION
 *	This is a Phase I test for the setgroups(2) system call.  It is intended
 *	to provide a limited exposure of the system call, for now.  It
 *	should/will be extended when full functional tests are written for
 *	setgroups(2).
 *
 * 	Setup:
 * 	  Setup signal handling.
 *	  Pause for SIGUSR1 if option specified.
 *
 * 	Test:
 *	 Loop if the proper options are given.
 * 	  Execute system call
 *	  Check return code, if system call failed (return=-1)
 *		Log the errno and Issue a FAIL message.
 *	  Otherwise, Issue a PASS message.
 *
 * 	Cleanup:
 * 	  Print errno log and/or timing stats if options given
 *
 *
 *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#**/

#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/param.h>
#include <unistd.h>
#include <grp.h>

#include "test.h"
#include "usctest.h"
#include "s2e.h"
#include "compat_16.h"

void setup();
void cleanup();

TCID_DEFINE(setgroups01);
int TST_TOTAL = 1;

int len = NGROUPS, ngrps = 0;
GID_T list[NGROUPS];

int main(int ac, char **av)
{
	int lc;
	char *msg;

    /***************************************************************
     * parse standard options
     ***************************************************************/
	if ((msg = parse_opts(ac, av, NULL, NULL)) != NULL) {
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);

	}

    /***************************************************************
     * perform global setup for test
     ***************************************************************/
	setup();

    /***************************************************************
     * check looping state if -c option given
     ***************************************************************/
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		tst_count = 0;

		/*
		 * Call setgroups(2)
		 */
		s2e_enable_forking();
		s2e_make_symbolic(&ngrps,sizeof(ngrps),"ngrps");
		TEST(SETGROUPS(ngrps, list));
		s2e_disable_forking();
		s2e_kill_state(0,"program done!");

		/* check return code */
		if (TEST_RETURN == -1) {
			TEST_ERROR_LOG(TEST_ERRNO);
			tst_resm(TFAIL,
				 "setgroups(%d, list) Failed, errno=%d : %s",
				 len, TEST_ERRNO, strerror(TEST_ERRNO));
		} else {
	    /***************************************************************
	     * only perform functional verification if flag set (-f not given)
	     ***************************************************************/
			if (STD_FUNCTIONAL_TEST) {
				/* No Verification test, yet... */
				tst_resm(TPASS,
					 "setgroups(%d, list) returned %ld",
					 len, TEST_RETURN);
			}
		}

	}

    /***************************************************************
     * cleanup and exit
     ***************************************************************/
	cleanup();
	tst_exit();
	tst_exit();

}

/***************************************************************
 * setup() - performs all ONE TIME setup for this test.
 ***************************************************************/
void setup()
{

	tst_sig(NOFORK, DEF_HANDLER, cleanup);

	if (geteuid() != 0)
		tst_brkm(TBROK, cleanup, "Must be ROOT to run this test.");

	TEST_PAUSE;

	if ((ngrps = GETGROUPS(len, list)) == -1) {
		tst_brkm(TBROK, cleanup,
			 "getgroups(%d, list) Failure. errno=%d : %s",
			 len, errno, strerror(errno));
	}
}

/***************************************************************
 * cleanup() - performs all ONE TIME cleanup for this test at
 *		completion or premature exit.
 ***************************************************************/
void cleanup()
{
	/*
	 * print timing stats if that option was specified.
	 * print errno log if that option was specified.
	 */
	TEST_CLEANUP;

}
