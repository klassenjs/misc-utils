/*
 * ext4-find-superblocks.c
 *
 * A quick and dirty program to find potential ext4 superblocks
 *
 * Copyright (c) 2019 James Klassen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

unsigned char buf[4096];

int main(int argc, char** argv)
{
	size_t block = 0;
	int fd = open(argv[1], O_RDONLY);

	while(1) { 
		lseek(fd, block*4096, SEEK_SET);
		if (read(fd, buf, 4096) != 4096) {
			printf("Short read\n");
			return(0);
		}

		if (buf[0x38] == 0x53 && buf[0x39] == 0xEF &&
				buf[0x3B] == 0x00) {
			printf("Match at block %lu %lu\n", block, block * 4);
		}

		block++;
	}
	return 0;
}
