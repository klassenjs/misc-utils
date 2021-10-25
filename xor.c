/*
 * xor.c
 *
 * Performs a bytewise XOR operation on two files
 * and writes the result to stdout.  Stops reads at the
 * first EOF.
 *
 * The general idea is to help spot small differences in
 * large binary files along with xxd and diff.
 *
 *
 * Copyright (c) 2021 James Klassen
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
#include<stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Usage: %s: file_a file_b\n", argv[0]);
		exit(1);
	}

	FILE* f1 = fopen(argv[1], "rb");
	FILE* f2 = fopen(argv[2], "rb");

	if (f1 == NULL) {
		printf("Could not open %s\n", argv[1]);
		exit(1);
	}
	if (f2 == NULL) {
		printf("Could not open %s\n", argv[2]);
		exit(1);
	}

	while(!feof(f1) && !feof(f2)) {
		int a,b;
		fread(&a, 4, 1, f1);
		fread(&b, 4, 1, f2);

		a = a ^ b;
		fwrite(&a, 4, 1, stdout);
	}
	fclose(f1);
	fclose(f2);

	return 0;
}
