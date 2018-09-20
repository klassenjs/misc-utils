/*
 * mmap.c
 *
 * Test using mmap to create a large file-backed VM space.
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

#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>

void* mmap_malloc(const char* filename, size_t size, int *fd)
{
        void* map;

        if (fd == NULL) {
                return NULL;
        }

        /* Open the backing file */
        *fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
        if (*fd == -1) {
                return NULL;
        }

        /* Expand backing file to size */
        if (lseek(*fd, size-1, SEEK_SET) == -1) {
                close(*fd);
                return NULL;
        }

        if (write(*fd, "", 1) != 1) {
                close(*fd);
                return NULL;
        }

        /* map backing file */
        map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
        if (map == MAP_FAILED) {
                close(*fd);
                return NULL;
        }

        return map;
}

void mmap_free(void* map, size_t size, int fd)
{
        if (map != NULL) {
                munmap(map, size);
        }
        if (fd > 0) {
                close(fd);
        }
}

int main()
{
	const char* mmap_file="mmap.bin";
	const size_t mmap_size = 96LL * (1024*1024*1024);
	const size_t array_size = mmap_size / sizeof(int);
	int mmap_fd = 0;
	int* array;

	size_t i;

	array = mmap_malloc(mmap_file, mmap_size, &mmap_fd);

	#pragma omp parallel for
	for (i = 0; i < array_size; i++) {
		array[i] = (int)i;
	}

	printf("Final Value: %d\n", array[array_size - 1]);

	mmap_free(array, mmap_size, mmap_fd);

	printf("Done\n");
	return 0;
}
