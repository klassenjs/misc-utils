/*
 * wait-for-hr.c
 *
 * Simple utility to sleep until the zero minute of the next hour. This is
 * meant to use instead of sleep(1) in a shell script to avoid accumulating
 * drift between executions in a delayed loop.
 * 
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
 * Building:
 *     gcc -o wait-for-hr wait-for-hr.c -pedantic -Wall -Os -s
 *
 * Running:
 *    ./wait-for-hr [-v]
 *        With -v will show number seconds it will wait.
 *
 */


#include<stdio.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

int main(int argc, char** argv)
{
	int verbose = 0;
	struct tm *tt;
	time_t t;
	time_t wait_sec;

	/* Check for verbose flag */
	if ((argc > 1) && (0==strcmp("-v", argv[1]))) {
		fprintf(stderr, "%s started\n", argv[0]);
		verbose = 1;
	}

	t = time(NULL);
	if (t == (time_t)-1) {
		perror("Couldn't get time");
		goto error;
	}

	tt = localtime(&t);
	if (tt == NULL) {
		perror("Couldn't parse time");
		goto error;
	}

	wait_sec = 3600 - (tt->tm_min * 60 + tt->tm_sec);

	if (verbose)
		fprintf(stderr, "Waiting %ld seconds\n", wait_sec);

	while(wait_sec) {
		wait_sec = sleep(wait_sec);
	}

	return 0;
error:
	return 1;
}
