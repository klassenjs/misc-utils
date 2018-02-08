/*
 * strerror.c
 *
 * Simple utility to display system message associated with an errno.
 *
 * Copyright (c) 2017 James Klassen
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
 *
 * Running:
 *    ./strerror errno
 *
 */
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
	char* psEnd;
	int errnum;

	if (argc != 2) {
		printf("%s: errno (int)\n", argv[0]);
		return 1;
	}
	
	errno = 0;
	errnum = strtol(argv[1], &psEnd, 10);

	if (errno != 0 || psEnd == argv[1]) {
		fprintf(stderr, "%s: cannot parse errno\n", argv[0]);
		return 1;
	}

	printf("%d: %s\n", errnum, strerror(errnum));
	return 0;
}

