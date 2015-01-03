#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "fpcompare.h"
#include "fpcount.h"


int main(int ARGC, char *ARGV[]) {

	double cutoff, similarity;
	int bits, blocks;
	int files[4];
	struct stat stats[4];
	unsigned long long *fingerprints[2], *fingerprint_i, *fingerprint_j;
	unsigned short *counts[2], count_i, count_j;
	size_t sizes[2];
	int i, j;
	
	cutoff = atof(ARGV[1]);
	bits = atoi(ARGV[2]);
	blocks = bits/64;
	
	files[0] = open(ARGV[3], O_RDONLY);
	fstat(files[0], &(stats[0]));
	fingerprints[0] = (unsigned long long *)mmap(NULL, stats[0].st_size, PROT_READ, MAP_SHARED, files[0], 0);
	sizes[0] = stats[0].st_size*8/bits;
	
	files[1] = open(ARGV[4], O_RDONLY);
	fstat(files[1], &(stats[1]));
	fingerprints[1] = (unsigned long long *)mmap(NULL, stats[1].st_size, PROT_READ, MAP_SHARED, files[1], 0);
	sizes[1] = stats[1].st_size*8/bits;
		
	if (ARGC >= 6) {
		files[2] = open(ARGV[5], O_RDONLY);
		fstat(files[2], &(stats[2]));
		counts[0] = (unsigned short *)mmap(NULL, stats[2].st_size, PROT_READ, MAP_SHARED, files[2], 0);
	}
	else {
		counts[0] = (unsigned short *)malloc(sizes[0]*sizeof(unsigned short));
		for (i = 0; i < sizes[0]; ++i) {
			counts[0][i] = fpcount(fingerprints[0]+blocks*i, blocks);
		}
	}
	
	if (ARGC >= 7) {
		files[3] = open(ARGV[6], O_RDONLY);
		fstat(files[3], &(stats[3]));
		counts[1] = (unsigned short *)mmap(NULL, stats[3].st_size, PROT_READ, MAP_SHARED, files[3], 0);
	}
	else {
		counts[1] = (unsigned short *)malloc(sizes[1]*sizeof(unsigned short));
		for (i = 0; i < sizes[1]; ++i) {
			counts[1][i] = fpcount(fingerprints[1]+blocks*i, blocks);
		}
	}
	
	for (i = 0; i < sizes[0]; ++i) {
		fingerprint_i = fingerprints[0]+blocks*i;
		count_i = counts[0][i];
		for (j = 0; j < sizes[1]; ++j) {
			fingerprint_j = fingerprints[1]+blocks*j;
			count_j = counts[1][j];
			if (count_i >= count_j*cutoff && count_j >= count_i*cutoff) {
				similarity = fpcompare(fingerprint_i, fingerprint_j, count_i, count_j, blocks);
				if (similarity >= cutoff) {
					printf("%d\t%d\t%lf\n",  i, j,  similarity);
				}
			}
		}
	}

	munmap(fingerprints[0], stats[0].st_size);
	close(files[0]);

	munmap(fingerprints[1], stats[1].st_size);
	close(files[1]);

	if (ARGC >= 6) {
		munmap(counts[0], stats[2].st_size);
		close(files[2]);
	}
	else {
		free(counts[0]);
	}

	if (ARGC >= 7) {
		munmap(counts[1], stats[3].st_size);
		close(files[3]);
	}
	else {
		free(counts[1]);
	}

	return(0);

}
