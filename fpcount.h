#ifndef __FPCOUNT_H__
#define __FPCOUNT_H__

#ifdef SSE4

unsigned long long fpcount(unsigned long long *fingerprint, int blocks) {
	
	unsigned long long count;
	int i;
	
	count = 0;
	i = 0;
	while (i < blocks) {
		count += __builtin_popcountll(fingerprint[i]);
		++i;
		count += __builtin_popcountll(fingerprint[i]);
		++i;
		count += __builtin_popcountll(fingerprint[i]);
		++i;
		count += __builtin_popcountll(fingerprint[i]);
		++i;
	}
	
	return count;
	
}
#else

unsigned long long fpcount(unsigned long long *fingerprint, int blocks) {
	
	unsigned long long block, count, vector;
	int i;
	
	count = 0;
	i = 0;
	while (i < blocks) {
		vector = 0;
		block = fingerprint[i];
		if (block) {
			block = block - ((block >> 1) & 0x5555555555555555ULL);
			block = (block & 0x3333333333333333ULL) + ((block >> 2) & 0x3333333333333333ULL);
			vector += (block & 0x0F0F0F0F0F0F0F0FULL) + ((block >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		}
		++i;
		block = fingerprint[i];
		if (block) {
			block = block - ((block >> 1) & 0x5555555555555555ULL);
			block = (block & 0x3333333333333333ULL) + ((block >> 2) & 0x3333333333333333ULL);
			vector += (block & 0x0F0F0F0F0F0F0F0FULL) + ((block >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		}
		++i;
		count += (vector * 0x0101010101010101ULL) >> 56;
		vector = 0;
		block = fingerprint[i];
		if (block) {
			block = block - ((block >> 1) & 0x5555555555555555ULL);
			block = (block & 0x3333333333333333ULL) + ((block >> 2) & 0x3333333333333333ULL);
			vector += (block & 0x0F0F0F0F0F0F0F0FULL) + ((block >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		}
		++i;
		block = fingerprint[i];
		if (block) {
			block = block - ((block >> 1) & 0x5555555555555555ULL);
			block = (block & 0x3333333333333333ULL) + ((block >> 2) & 0x3333333333333333ULL);
			vector += (block & 0x0F0F0F0F0F0F0F0FULL) + ((block >> 4) & 0x0F0F0F0F0F0F0F0FULL);
		}
		++i;
		count += (vector * 0x0101010101010101ULL) >> 56;
	}
	
	return(count);
	
}

#endif

#endif
