/*
 * S2E Selective Symbolic Execution Framework
 *
 * Copyright (c) 2010, Dependable Systems Laboratory, EPFL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Dependable Systems Laboratory, EPFL nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE DEPENDABLE SYSTEMS LABORATORY, EPFL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Currently maintained by:
 *    Volodymyr Kuznetsov <vova.kuznetsov@epfl.ch>
 *    Vitaly Chipounov <vitaly.chipounov@epfl.ch>
 *
 * All contributors are listed in S2E-AUTHORS file.
 *
 */

//#include <stdio.h>
//#include <string.h>
//#include <inttypes.h>

/** Forces the read of every byte of the specified string.
  * This makes sure the memory pages occupied by the string are paged in
  * before passing them to S2E, which can't page in memory by itself. */

#ifndef S2E_H_
#define S2E_H_

static inline void __s2e_touch_string(volatile const char *string)
{
    while(*string) {
        ++string;
    }
}

static inline void __s2e_touch_buffer(volatile void *buffer, unsigned size)
{
    unsigned i;
    for (i=0; i<size; ++i) {
        volatile const char *b = (volatile const char *)buffer;
        *b;
    }
}

//fwl new opcode
static inline void s2e_detect_int(void* buf1, void* buf2, int size, int flag)
{
	//__s2e_touch_string(flag);
    __s2e_touch_buffer(buf1, size);
    __s2e_touch_buffer(buf2, size);
    __asm__ __volatile__(
        "pushl %%ebx\n"
        //"movl %%edx, %%ebx\n"
        ".byte 0x0f, 0x3f\n"
        ".byte 0x00, 0x80, 0x00, 0x00\n"
        ".byte 0x00, 0x00, 0x00, 0x00\n"
        "popl %%ebx\n"
        : : "a" (buf1), "b" (buf2),  "d" (size), "c" (flag) : "memory"
    );
}

/** Print message to the S2E log. */
static inline void s2e_message(const char* message)
{
    __s2e_touch_string(message);
    __asm__ __volatile__(
        ".byte 0x0f, 0x3f\n"
        ".byte 0x00, 0x10, 0x00, 0x00\n"
        ".byte 0x00, 0x00, 0x00, 0x00\n"
        : : "a" (message)
    );
}

#endif


