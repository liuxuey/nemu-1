#include "common.h"
#include "memory/cache.h"
#include <stdlib.h>
#include "burst.h"

void ddr3_read_public(hwaddr_t addr, void *data);

void resetCache() {
	int i = 0;
	for(i = 0; i < CACHE_SET_SIZE * CACHE_WAY_SIZE; i++) cache[i].valid = false;
}

//return ID
int readCache(hwaddr_t addr) {
	uint32_t tag = addr >> (CACHE_BLOCK_SIZE_BIT + CACHE_SET_BIT);
	uint32_t set = (addr >> CACHE_BLOCK_SIZE_BIT) & (CACHE_SET_SIZE - 1);
	uint32_t block = (addr >> CACHE_BLOCK_SIZE_BIT) << CACHE_BLOCK_SIZE_BIT;
	//not offset
	int i = 0, j;
	for(i = CACHE_WAY_SIZE * set; i < CACHE_WAY_SIZE * (set + 1); i++) {
		if(cache[i].tag == tag && cache[i].valid) return i;
	}
	srand(i);
	i = CACHE_WAY_SIZE * set + rand() % CACHE_WAY_SIZE;//random
	for(j = 0; j < CACHE_BLOCK_SIZE / BURST_LEN; j++) {
		printf("%d %d %d\n",block + j * BURST_LEN,addr,(1 << (10 + 10 + 3 + (27 - 10 - 10 - 3))));
		ddr3_read_public(block + j * BURST_LEN , cache[i].data + j * BURST_LEN);
	}
	cache[i].valid = true;
	cache[i].tag = tag;
	return i;
}