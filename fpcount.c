#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "fpcount.h"


int main(int ARGC, char *ARGV[]) {

	int bits, blocks, file, i;
	struct stat stat;
	size_t size;
	unsigned long long count, *fingerprints;
	
	bits = atoi(ARGV[1]);
	blocks = bits/64;

	// Map file to memory and count "ones".
	file = open(ARGV[2], O_RDONLY);
	fstat(file, &stat);
	fingerprints = (unsigned long long *)mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, file, 0);
	size = stat.st_size*8/bits;
	for (i = 0; i < size; ++i) {
		count = fpcount(fingerprints+blocks*i, blocks);
		fputc((int)(count & 0x00000000000000FFULL), stdout);
		fputc((int)((count >> 8) & 0x00000000000000FFULL), stdout);
	}
	
	// Free allocated memory and unmap files from memory.
	munmap(fingerprints, stat.st_size);
	close(file);

	return(0);

}
