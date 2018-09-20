/*
 * big_vm.c
 *
 * Program to allocate a huge VM space to test how allocated memory affects
 * core dump size.
 *
 * This program is meant to be run in gdb with breakpoints on various lines
 * where the user can then force a core dump and see what it contains.
 *
 * The idea is looking at core dump size as memory is allocated, touched, 
 * and then freed.
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
 */

#include<stdio.h>
#include<stdlib.h>

int main()
{
	size_t a_size = 4LL*1024*1024*1024;
	int *array = (int*)malloc(sizeof(int)*a_size);

	#pragma omp parallel for
	for(size_t i = 0; i < a_size; i++) {
		array[i] = (int)i;
	}

	free(array);

	printf("break here\n");

	return 0;
}
