#include "ringbuff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define RING_DEBUG_FLAG 1    /*打开输出*/
/********************************************************************************************/
/*******************************           宏         ***************************************/
/********************************************************************************************/

#if RING_DEBUG_FLAG
#define RING_DEBUG(fmt,args...) printf("DEBUG %s-%d: "fmt"\n",__FUNCTION__,__LINE__,## args);
#else
#define RING_DEBUG(fmt,args...)
#endif

#define RING_FREE(ptr) do{if(0==(ptr))break; free(ptr); (ptr)=0; }while(0)
/********************************************************************************************/
/*******************************       静态函数声明     ***************************************/
/********************************************************************************************/



/********************************************************************************************/
/*******************************         函数实现      ***************************************/
/********************************************************************************************/


ring_t* Ring_CreateNew(int buffSize)
{
    ring_t *pRing = 0;
    do{
        pRing = (ring_t*)malloc(sizeof(ring_t));
        if(0==pRing) break;
        memset(pRing,0,sizeof(ring_t));

        pRing->pIndex_vec = Vec_CreateNew(sizeof(ringblock_t),1024);
        if(0==pRing->pIndex_vec) break;
        pRing->pDataBuff = (char*)malloc(buffSize);
        if(0==pRing->pDataBuff) break;

        pRing->dataBuffSize = buffSize;


        return pRing;
    }while(0);

    Ring_delete(pRing);
    return 0;
}

void Ring_delete(ring_t* pRing)
{
    if(0!=pRing){
        Vec_Delete(pRing->pIndex_vec);
        RING_FREE(pRing->pDataBuff);
        RING_FREE(pRing);
    }
}

int Ring_GetFreeBlock(ring_t* pRing, ringblock_t* pBlock)
{
    if(0==pRing) return -1;
    if(0==pBlock || 0>=pBlock->size || pRing->dataBuffSize<pBlock->size) return -1;

    ringblock_t *pHeadBlock=0;
    ringblock_t *pLastBlock=0;

    do{

        pLastBlock = Vec_Last(pRing->pIndex_vec);
        if(0!=pLastBlock){
            pBlock->offset = pLastBlock->offset+pLastBlock->size;
        }else{
            pBlock->offset = 0;
        }
        pHeadBlock = Vec_Frist(pRing->pIndex_vec);
        if(0!=pHeadBlock){
            if(pBlock->offset+pBlock->size <= pRing->dataBuffSize){
                if(pHeadBlock->offset >= pBlock->offset){
                    if(pBlock->offset+pBlock->size>pHeadBlock->offset) break;
                }
            }else{
                if(pHeadBlock->offset >= pBlock->offset) break;
                pBlock->offset = 0;
                if(pBlock->offset+pBlock->size > pHeadBlock->offset) break;
            }
        }
        return 0;
    }while(0);
    pBlock->offset = -1;
    return -1;
}

int  Ring_CommitBlock(ring_t* pRing, ringblock_t* pBlock)
{
    if(0==pRing) return -1;
    if(0==pBlock || 0>=pBlock->size || pRing->dataBuffSize<pBlock->size) return -1;

    int rsl;
    rsl = Vec_PushBack(pRing->pIndex_vec,pBlock);
    return rsl;
}

int  Ring_GetLastBlock(ring_t* pRing, ringblock_t* pBlock, int *dration)
{
    if(0==pRing) return -1;
    if(0==pBlock ) return -1;
    if(1>=Vec_Count(pRing->pIndex_vec)) return -1;

    ringblock_t *pFristBlock = 0;
    ringblock_t *pSecondBlock = 0;

    do{

        pFristBlock = Vec_Frist(pRing->pIndex_vec);
        pSecondBlock = Vec_At(pRing->pIndex_vec,1);

        *dration = pSecondBlock->duration - pFristBlock->duration;

        pBlock->offset = pFristBlock->offset;
        pBlock->size = pFristBlock->size;

        return 0;
    }while(0);
    return -1;
}

void Ring_FreeLastBlock(ring_t* pRing)
{
    if(0==pRing) return;
    Vec_RemoveFrist(pRing->pIndex_vec);
}


int Ring_FrameCount(ring_t* pRing)
{
    if(0==pRing) return -1;
    return Vec_Count(pRing->pIndex_vec);
}

void Ring_Clean(ring_t *pRing)
{
    if(0==pRing) return -1;
    Vec_Clear(pRing->pIndex_vec);
}





