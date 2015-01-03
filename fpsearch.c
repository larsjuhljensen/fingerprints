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
	
	int algorithms = 4;
	int bits = 4096;
	int blocks = bits/64;
	double weights[] = {0.4, 0.3, 0.2, 0.1};
	
	double cutoff, similarity;
	int files[4][algorithms];
	struct stat stats[4][algorithms];
	unsigned long long *fingerprints[2][algorithms], *fingerprint_i, *fingerprint_j;
	unsigned short *counts[2][algorithms], count_i, count_j;
	size_t sizes[2];
	int a, d, i, j;
	
	cutoff = atof(ARGV[1]);
	
	for (a = 0; a < algorithms; ++a) {
		filename = ARGV[3];
		files[0][a] = open(filename, O_RDONLY);
		fstat(files[0][a], &(stats[0][a]));
		fingerprints[0][a] = (unsigned long long *)mmap(NULL, stats[0][a].st_size, PROT_READ, MAP_SHARED, files[0][a], 0);
		filename = ;
		files[2][a] = open(filename, O_RDONLY);
		fstat(files[2][a], &(stats[2][a]));
		counts[0][a] = (unsigned short *)mmap(NULL, stats[2][a].st_size, PROT_READ, MAP_SHARED, files[2][a], 0);
	}
	sizes[0] = stats[0][0].st_size*8/bits;
	
	for (d = 3; d < ARGC; ++d) {
		for (a = 0; a < algorithms; ++a) {
			filename = ARGV[d];
			files[1][a] = open(filename, O_RDONLY);
			fstat(files[1][a], &(stats[1][a]));
			fingerprints[1][a] = (unsigned long long *)mmap(NULL, stats[1][a].st_size, PROT_READ, MAP_SHARED, files[1][a], 0);
			filename = ;
			files[3][a] = open(filename, O_RDONLY);
			fstat(files[3][a], &(stats[3][a]));
			counts[1][a] = (unsigned short *)mmap(NULL, stats[3][a].st_size, PROT_READ, MAP_SHARED, files[3][a], 0);
		}
		sizes[1] = stats[1][0].st_size*8/bits;
		for (i = 0; i < sizes[0]; ++i) {
			for (j = 0; j < sizes[1]; ++j) {
				similarity = 0;
				for (a = 0; a < algorithms; ++a) {
					fingerprint_i = fingerprints[0][a]+blocks*i;
					count_i = counts[0][a][i];
					fingerprint_j = fingerprints[1][a]+blocks*j;
					count_j = counts[1][a][j];
					if (count_i >= count_j*cutoff && count_j >= count_i*cutoff) {
						similarity += weights[a]*fpcompare(fingerprint_i, fingerprint_j, count_i, count_j, blocks);
					}
				}
				if (similarity >= cutoff) {
					printf("%d\t%d\t%lf\n",  i, j,  similarity);
				}
			}
		}
		for (a = 0; a < algorithms; ++a) {
			munmap(fingerprints[1][a], stats[1][a].st_size);
			close(files[1][a]);
			munmap(counts[1], stats[3][a].st_size);
			close(files[3][a]);
		}
	}

	for (a = 0; a < algorithms; ++a) {
		munmap(fingerprints[0], stats[0].st_size);
		close(files[0][a]);
		munmap(counts[0][a], stats[2][a].st_size);
		close(files[2][a]);
	}

	return(0);

}
