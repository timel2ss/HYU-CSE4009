//2019054957
#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <math.h>

typedef struct CacheLine_ {
    int latest;
    int valid;
    unsigned tag;
    char* block;
}CacheLine;

typedef struct CacheSet_ {
    CacheLine* lines;
}CacheSet;

typedef struct Cache_ {
    CacheSet* sets;
}Cache;

Cache* createCache(void);
void freeCache(Cache* cache);    
void accessCache(Cache* cache, unsigned address, int size, int s, int b);
int checkHitOrMiss(CacheSet set, unsigned tag);
int checkEmptyLine(CacheSet set);
int checkEvictLine(CacheSet set);

int numOfSets = 0, numOfLines = 0, sizeOfBlock = 0;
int hitCount = 0, missCount = 0, evictionCount = 0;
int flag_v = 0;
char instruction;

int main(int argc, char* argv[])
{
    int c;
    int s, E, b;
    char* traceFileName;
    FILE* traceFile;
    unsigned address;
    int size;

    Cache* cache;

    while((c = getopt(argc, argv, "vs:E:b:t:")) != -1) {
        switch(c) {
            case 'v':
                flag_v = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                traceFileName = optarg;
                break;
            case '?':
                printf("Unknown flag : %c", optopt);
                exit(1);
        }
    }
    numOfSets = pow(2, s);
    numOfLines = E;
    sizeOfBlock = pow(2, b);

    cache = createCache();
    
    traceFile = fopen(traceFileName, "r");
    if(traceFile == NULL) {
        printf("Trace file cannot be found.");
        exit(1);
    }
    
    while(fscanf(traceFile, " %c %x, %d", &instruction, &address, &size) != EOF) {
        if(flag_v) {
            if(instruction != 'I') {
                printf("%c %x,%d ", instruction, address, size);
            }
        }

        switch(instruction) {
            case 'I':
                break;
            case 'L':
                accessCache(cache, address, size, s, b);
                break;
            case 'S':
                accessCache(cache, address, size, s, b);
                break;
            case 'M':
                accessCache(cache, address, size, s, b);
                accessCache(cache, address, size, s, b);
                break;
        }

        if(flag_v) {
            if(instruction != 'I') {
                printf("\n");
            }
        }
    }

    printSummary(hitCount, missCount, evictionCount);
    
    fclose(traceFile);
    freeCache(cache);
    return 0;
}

Cache* createCache(void) {
    Cache* cache;

    cache = (Cache*)malloc(sizeof(Cache));
    cache->sets = (CacheSet*)malloc(sizeof(CacheLine) * numOfSets);
    
    int i, j;
    for(i = 0; i < numOfSets; i++) {
        cache->sets[i].lines = (CacheLine*)malloc(sizeof(CacheLine) * numOfLines);
        for(j = 0; j < numOfLines; j++) {
            cache->sets[i].lines[j].latest = 0;
            cache->sets[i].lines[j].valid = 0;
            cache->sets[i].lines[j].tag = 0;
            cache->sets[i].lines[j].block = (char*)malloc(sizeOfBlock);
        }
    }
    return cache;
}

void freeCache(Cache* cache) { 
    int i, j;
    for(i = 0; i < numOfSets; i++) {
        for(j = 0; j < numOfLines; j++) {
            free(cache->sets[i].lines[j].block);
        }
        free(cache->sets[i].lines);
    }
    free(cache->sets);
    free(cache);
}

void accessCache(Cache* cache, unsigned address, int size, int s, int b) {
    int tagSize = 32 - (s + b);
    unsigned tag = address >> (s + b);
    unsigned setIndex = (address << tagSize) >> (tagSize + b);
    int index, i;

    CacheSet cacheSet = cache->sets[setIndex];
    int check = checkHitOrMiss(cacheSet, tag);
    if(check != -1) {
        hitCount++;
        cache->sets[setIndex].lines[check].latest = 0;
        if(flag_v) {
            if(instruction != 'I') {
                printf("hit ");
            }
        }
    }
    else {
        missCount++;
        if(flag_v) {
            if(instruction != 'I') {
                printf("miss ");
            }
        }
        
        index = checkEmptyLine(cacheSet);
        
        if(index != -1) {
            cache->sets[setIndex].lines[index].valid = 1;
            cache->sets[setIndex].lines[index].tag = tag;
            cache->sets[setIndex].lines[index].latest = 0;
        }
        else {
            index = checkEvictLine(cacheSet);
            cache->sets[setIndex].lines[index].tag = tag;
            cache->sets[setIndex].lines[index].latest = 0;
            if(flag_v) {
                if(instruction != 'I') {
                    printf("eviction ");
                }
            }
            evictionCount++;
        }
    }
    for(i = 0; i < numOfLines; i++) {
        if(cache->sets[setIndex].lines[i].valid) {
            cache->sets[setIndex].lines[i].latest++;
        }
    }

}

int checkHitOrMiss(CacheSet set, unsigned tag) {
    int i;
    for(i = 0; i < numOfLines; i++) {
        CacheLine tempLine = set.lines[i];
        if(tempLine.valid) {    
            if(tempLine.tag == tag) {
                return i;
            }
        }
    }
    return -1;
}

int checkEmptyLine(CacheSet set) {
    int i;
    for(i = 0; i < numOfLines; i++) {
        if(!set.lines[i].valid) {
            return i;
        }
    }
    return -1;
}

int checkEvictLine(CacheSet set) {
    int i;
    int LRU = set.lines[0].latest;
    int index = 0;
    for(i = 1; i < numOfLines; i++) {
        if(LRU < set.lines[i].latest) {
            LRU = set.lines[i].latest;
            index = i;
        }
    }
    return index;
}
