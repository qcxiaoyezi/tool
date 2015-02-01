#ifndef RINGBUFF_H
#define RINGBUFF_H

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ringblock_t{
    int     offset;
    int     size;
    int     duration;
}ringblock_t;

typedef struct ring_t{
    vector_t         *pIndex_vec;
    char             *pDataBuff;
    int               dataBuffSize;
}ring_t;

ring_t* Ring_CreateNew(int buffSize);
void Ring_delete(ring_t* pRing);
int  Ring_FrameCount(ring_t* pRing);
void Ring_Clean(ring_t* pRing);

int  Ring_GetFreeBlock(ring_t* pRing, ringblock_t*);
int  Ring_CommitBlock(ring_t* pRing, ringblock_t*);
int  Ring_GetLastBlock(ring_t* pRing, ringblock_t* , int *dration);
void Ring_FreeLastBlock(ring_t* pRing);

#ifdef __cplusplus
}
#endif
#endif
