#ifndef __FPCOMPARE_H__
#define __FPCOMPARE_H__

#ifdef SSE4

double fpcompare(unsigned long long *fingerprint_i, unsigned long long *fingerprint_j, unsigned short count_i, unsigned short count_j, int blocks) {

	unsigned long long count;
	int k;
	
	count = 0;
	k = 0;
	while (k < blocks) {
		count += __builtin_popcountll(fingerprint_i[k] & fingerprint_j[k]);
		++k;
		count += __builtin_popcountll(fingerprint_i[k] & fingerprint_j[k]);
		++k;
		count += __builtin_popcountll(fingerprint_i[k] & fingerprint_j[k]);
		++k;
		count += __builtin_popcountll(fingerprint_i[k] & fingerprint_j[k]);
		++k;
	}
	return((double)count/(count_i+count_j-count));
	
}

#else

double fpcompare(unsigned long long *fingerprint_i, unsigned long long *fingerprint_j, unsigned short count_i, unsigned short count_j, int blocks) {

	unsigned long long block, count, vector;
	int k;
	
	count = 0;
	k = 0;
	while (k < blocks) {
		vector = 0;
		block = fingerprint_i[k] & fingerprint_j[k];
		if (block) {
			block = block - ((block >> 1) & 0x5555555555555555ULL);
			block = (block & 0x3333333333333333ULL) + ((block >> 2) & 0x3333333333333333ULL);
			vector += (block & 0x0F0F0F0F0F0F0F0FULL) + ((block >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		}
		++k;
		block = fingerprint_i[k] & fingerprint_j[k];
		if (block) {
			block = block - ((block >> 1) & 0x5555555555555555ULL);
			block = (block & 0x3333333333333333ULL) + ((block >> 2) & 0x3333333333333333ULL);
			vector += (block & 0x0F0F0F0F0F0F0F0FULL) + ((block >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		}
		++k;
		count += (vector * 0x0101010101010101ULL) >> 56;
		vector = 0;
		block = fingerprint_i[k] & fingerprint_j[k];
		if (block) {
			block = block - ((block >> 1) & 0x5555555555555555ULL);
			block = (block & 0x3333333333333333ULL) + ((block >> 2) & 0x3333333333333333ULL);
			vector = (block & 0x0F0F0F0F0F0F0F0FULL) + ((block >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		}
		++k;
		block = fingerprint_i[k] & fingerprint_j[k];
		if (block) {
			block = block - ((block >> 1) & 0x5555555555555555ULL);
			block = (block & 0x3333333333333333ULL) + ((block >> 2) & 0x3333333333333333ULL);
			vector += (block & 0x0F0F0F0F0F0F0F0FULL) + ((block >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		}
		++k;
		count += (vector * 0x0101010101010101ULL) >> 56;
	}
	return((double)count/(count_i+count_j-count));
	
}

#endif

#endif
