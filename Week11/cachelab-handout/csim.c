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
    int tag;
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
void accessCache(Cache* cache, int address, int size, int s, int b);
int checkHitOrMiss(Cache* cache);
int checkEmptyLine(Cache* cache);
int checkEvictLine(Cache* cache);

int numOfSets = 0, numOfLines = 0, sizeOfBlock = 0;
int hitCount = 0, missCount = 0, evictionCount = 0;
int flag_v = 0;

int main(int argc, char* argv[])
{
    int c;
    int s, E, b;
    char* traceFileName;
    File* traceFile;
    char instruction;
    int address, size;

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
                memcpy(traceFileName, optarg, strlen(optarg));
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
            printf("%c %x,%d ", instruction, address, size);
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
            printf("\n");
        }
    }

    printSummary(hitCount, missCount, evictionCount);

    fclose(traceFile);
    freeCache(cache, numOfSets, E);
    return 0;
}

Cache* createCache(void) {
    Cache* cache;

    cache = (Cache*)malloc(sizeof(Cache));
    cache->sets = (CacheSet*)malloc(sizeof(CacheLine) * numOfSets);
    
    CacheSet tempSet;
    CacheLine tempLine;

    int i, j;
    for(i = 0; i < numOfSets; i++) {
        tempSet.lines = (CacheLine*)malloc(sizeof(CacheLine) * numOfLines);
        for(j = 0; j < numOfLines; j++) {
            tempLine.latest = 0;
            tempLine.valid = 0;
            tempLine.tag = 0;
            tempLine.block = (char*)malloc(sizeOfBlock);
            tempSet.lines[i] = tempLine;
        }
        cache->sets[i] = tempSet;
    }
    return cache;
}

void freeCache(Cache* cache) { 
    int i, j;
    for(i = 0; i < numOfSets; i++) {
        CacheSet* tempSet = cache->sets[i];
        if(tempSet->lines != NULL) {
            for(j = 0; j < numOfLines; j++) {
                CacheLine* tempLine = tempSet->lines[i];
                if(tempLine != NULL) {
                    free(tempLine->block);
                }
            }
            free(tempSet->lines);
        }
        if(tempSet != NULL) {
            free(tempSet);
        }
    }
    free(cache);
}

void accessCache(Cache* cache, int address, int size, int s, int b) {
    int tagSize = 32 - (s + b);
    int tag = address >> (s + b);
    int setIndex = (address << tagSize) >> (tagSize + b);

    CacheSet cacheSet = cache->sets[setIndex];
    int check = checkHitOrMiss(cacheSet, tag);
    if(check == -1) {
        missCount++;
        if(flag_v) {
            printf("miss ");
        }
    }

}

int checkHitOrMiss(CacheSet* set, int tag) {
    int i;
    for(int i = 0; i < numOfLines; i++) {
        CacheLine tempLine = set->lines[i];
        if(tempLine.valid == 1) {
            if(tempLine.tag == tag) {
                hitCount++;
                if(flag_v) {
                    printf("hit ");
                }
                return i;
            }
        }
    }
    return -1;
}

int checkEmptyLine(Cache* cache) {

}

int checkEvictLine(Cache* cache) {

}
