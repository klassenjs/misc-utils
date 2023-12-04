/* 
 * Counts the number of blocks in a file that are all zero's vs the
 * number of blocks that aren't all zeros.
 */

#include<fcntl.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

const size_t BLKSIZE=1024*1024*1024; // 1GB

int main(int argc, char** argv) {
	size_t zero_blocks = 0;
	size_t total_blocks = 0;
	ssize_t rec = 0;
	uint64_t *buffer;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <file/device>\n", argv[0]);
		return 2;
	}

	buffer = (uint64_t*)malloc(BLKSIZE);
	if (buffer == NULL) {
		fprintf(stderr, "cannot allocate buffer\n");
		return 1;
	}	

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "cannot open %s\n", argv[1]);
		return 1;
	}


	while((rec = read(fd, buffer, BLKSIZE)) == BLKSIZE) {
		size_t all_zero = 1;

		for(size_t i = 0; i < rec/sizeof(uint64_t); i++) {
			if (buffer[i] != 0) {
				all_zero = 0;
				break;
			}
		}
		zero_blocks += all_zero;
		total_blocks++;

		fprintf(stderr, "%ld / %ld\r", zero_blocks, total_blocks);
		
	}
	fprintf(stderr, "\n");
	close(fd);
	return 0;
}
