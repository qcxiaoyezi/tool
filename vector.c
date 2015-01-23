#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if VEC_DEBUG_FLAG
#define VEC_DEBUG(fmt,args...) printf("DEBUG %s-%d: "fmt"\n",__FUNCTION__,__LINE__,## args);
#else
#define VEC_DEBUG(fmt,args...)
#endif

/********************************************************************************************/
/*******************************       静态函数声明     ***************************************/
/********************************************************************************************/

static int Vec_AppendItemCount(vector_t* pVec, int num);


/********************************************************************************************/
/*******************************       函数实现        ***************************************/
/********************************************************************************************/


/*constructs a empty vector*/
vector_t* Vec_CreateNew(int itemSize, int itemMaxCount)
{
    vector_t* pVec = 0;
    if(0>=itemSize || 0>=itemMaxCount) return 0;

    do{
        pVec = (vector_t*)malloc(sizeof(vector_t));
        if(0==pVec) break;
        memset(pVec,0,sizeof(vector_t));

        pVec->itemSize = itemSize;
        pVec->itemCount = 0;
        pVec->itemMaxCount = itemMaxCount;
        pVec->itemMaxCountReal = 65535;

        pVec->head = 0;
        pVec->tail = 0;

        pVec->pDataBuff = (char*)malloc(pVec->itemSize*pVec->itemMaxCount);
        if(0==pVec->pDataBuff) break;

        return pVec;
    }while(0);
    Vec_Delete(pVec);
    return 0;
}

/*delete a vector*/
void Vec_Delete(vector_t *pVec)
{
    if(0==pVec) return;
    VEC_FREE(pVec->pDataBuff);
    VEC_FREE(pVec);
}

/*remove all item from the vector*/
void Vec_Clear(vector_t *pVec)
{
    if(0==pVec) return;
    pVec->head = 0;
    pVec->tail = 0;
    pVec->itemCount = 0;
}

/*resize the pDataBuff size to contain more or less item*/
int Vec_Resize(vector_t *pVec, unsigned int count)
{
    if(0==pVec) return -1;
    if(pVec->itemMaxCount == count) return 0;
    if(pVec->itemMaxCountReal < count) return -1;

    Vec_Foreach_Variable;
    char  *pNewBuff   = 0;
    char  *pDst       = 0;
    char  *pSrc       = 0;

    do{
        pNewBuff = (char*)malloc(pVec->itemSize * count);
        if(0==pNewBuff) break;

        Vec_Foreach(pVec){
            if(Vec_Foreach_Index >= count) break;
            pSrc = Vec_Foreach_Value;
            pDst = Vec_Foreach_Index*pVec->itemSize + pNewBuff;
            memcpy(pDst,pSrc,pVec->itemSize);
        }

        pVec->itemCount = Vec_Foreach_Index;
        pVec->itemMaxCount = count;
        pVec->head = VEC_INDEX(pVec,Vec_Foreach_Index);
        pVec->tail = 0;

        VEC_FREE(pVec->pDataBuff);
        pVec->pDataBuff = pNewBuff;
        return 0;
    }while(0);
    VEC_FREE(pNewBuff);
    return -1;
}

/*inserts 'value' at index position 'index' in the vector*/
int Vec_Insert(vector_t *pVec, void *pValue, int index)
{
    return Vec_InsertEx(pVec, pValue, index, 1);
}

/*inserts 'pValue' at index position 'index' in the vector*/
int Vec_InsertEx(vector_t *pVec, void *pValue, unsigned int index, unsigned int num)
{
    if(0==pVec || 0==pValue) return -1;
    if(index >= pVec->itemCount) index = pVec->itemCount;

    Vec_Foreach_Variable;
    char          *pSrc = 0;
    char          *pDst = 0;
    int            rsl  = 0;
    unsigned int   i    = 0;

    do{
        /*resize*/
        rsl = Vec_AppendItemCount(pVec,num);
        if(0!=rsl) break;

        Vec_Foreach(pVec){
            if(index+Vec_Foreach_Index >= pVec->itemCount) {break;}
            pSrc = Vec_Foreach_Value_Reverse;
            pDst = Vec_Foreach_Value_Reverse_Ex(num);
            memcpy(pDst,pSrc,pVec->itemSize);
        }
        for(i=0;i<num;i++){
            pSrc = i*pVec->itemSize + (char*)pValue;
            pDst = VEC_INDEX_VALUE(pVec,index+i);
            memcpy(pDst,pSrc,pVec->itemSize);
        }

        pVec->itemCount += num;
        pVec->head = VEC_INDEX(pVec,pVec->head+num);
        return 0;
    }while(0);
    return -1;
}

/*Inserts 'pValue' at the beginning of the vector*/
int Vec_PushFront(vector_t *pVec, void *pValue)
{
    if(0==pVec || 0==pValue) return -1;

    int    rsl;
    char  *pDst=0;

    do{
        rsl = Vec_AppendItemCount(pVec,1);
        if(0!=rsl) break;

        pDst = VEC_INDEX_VALUE(pVec,pVec->tail-1);
        pVec->tail = VEC_INDEX(pVec,pVec->tail-1);
        memcpy(pDst,pValue,pVec->itemSize);

        pVec->itemCount++;
        return 0;
    }while(0);
    return -1;
}

/*inserts 'pValue' at the end of the vector*/
int Vec_PushBack(vector_t *pVec, void *pValue)
{
    if(0==pVec || 0==pValue) return -1;

    int    rsl;
    char  *pDst=0;
    do{
        /*resize*/
        rsl = Vec_AppendItemCount(pVec,1);
        if(0!=rsl) break;

        pDst = VEC_INDEX_VALUE(pVec,pVec->head);        
        pVec->head = VEC_INDEX(pVec,pVec->head+1);
        memcpy(pDst,pValue,pVec->itemSize);

        pVec->itemCount++;
        return 0;
    }while(0);
    return -1;
}

/*remvoe the item at 'index' position*/
int Vec_RemoveAt(vector_t *pVec,int index)
{
    return Vec_RemoveAtEx(pVec,index,1);
}

/*remvoe multiple item at 'index' position*/
int Vec_RemoveAtEx(vector_t *pVec, unsigned int index, unsigned int num)
{
    if(0==pVec) return -1;
    if(pVec->itemCount<=index) return -1;

    char        *pSrc = 0;
    char        *pDst = 0;
    unsigned int i = 0;

    do{
        if(num > pVec->itemCount-index){
            num = pVec->itemCount-index;
        }
        for(i=index;i<pVec->itemCount-num;i++){
            pSrc = VEC_INDEX_VALUE(pVec,pVec->tail+i+num);
            pDst = VEC_INDEX_VALUE(pVec,pVec->tail+i);
            memcpy(pDst,pSrc,pVec->itemSize);
        }
        pVec->itemCount -= num;
        pVec->head = VEC_INDEX(pVec,pVec->head-num);
        return 0;
    }while(0);
    return -1;
}

/*remove the frist item in the list*/
int Vec_RemoveFrist(vector_t *pVec)
{
    if(0==pVec) return -1;
    if(0>=pVec->itemCount) return -1;

    do{

        pVec->tail = VEC_INDEX(pVec,pVec->tail+1);
        pVec->itemCount --;
        return 0;
    }while(0);
    return -1;
}

/*remove the last item in the list*/
int Vec_RemoveLast(vector_t *pVec)
{
    if(0==pVec) return -1;
    if(0>=pVec->itemCount) return -1;

    do{
        pVec->tail = VEC_INDEX(pVec,pVec->tail+1);
        pVec->itemCount --;
        return 0;
    }while(0);
    return -1;
}

/*remove the item at index position 'index' and return it*/
int Vec_TakeAt(vector_t *pVec, void *pBuff, unsigned int index)
{
    unsigned int num = 1;
    return Vec_TakeAtEx(pVec,pBuff,index,&num);
}

/*remove multiple item at index position 'index' and return it*/
int Vec_TakeAtEx(vector_t *pVec, void *pBuff, unsigned int index, unsigned int *num)
{
    if(0==pVec) return -1;
    if(pVec->itemCount<=index) return -1;

    char        *pSrc = 0;
    char        *pDst = 0;
    unsigned int i = 0;

    *num  = (*num+index > pVec->itemCount) ? (pVec->itemCount-index) : (*num);
    do{
        for(i=0;i<*num;i++){
            pSrc = VEC_INDEX_VALUE(pVec,pVec->tail+index+i);
            pDst = i*pVec->itemSize+(char*)(pBuff);
            memcpy(pDst,pSrc,pVec->itemSize);
        }
        Vec_RemoveAtEx(pVec,index,*num);
        return 0;
    }while(0);
    return -1;
}

/*remove the frist itme in the vector and return it*/
int Vec_PopFront(vector_t *pVec, void *pBuff)
{
    if(0==pVec) return -1;
    if(0>=pVec->itemCount) return -1;

    char *pSrc = 0;
    char *pDst = 0;
    do{
        pVec->itemCount --;
        pSrc = VEC_INDEX_VALUE(pVec,pVec->tail);
        pVec->tail = VEC_INDEX(pVec,pVec->tail+1);

        pDst = (char*)pBuff;
        memcpy(pDst,pSrc,pVec->itemSize);

        return 0;
    }while(0);
    return -1;
}

/*remove the last item in the vector and return it*/
int Vec_PopBack(vector_t *pVec, void *pBuff)
{
    if(0==pVec) return -1;
    if(0>=pVec->itemCount) return -1;

    char *pSrc = 0;
    char *pDst = 0;
    do{
        pVec->itemCount --;
        pSrc = VEC_INDEX_VALUE(pVec,pVec->head-1);
        pVec->head = VEC_INDEX(pVec,pVec->head-1);

        pDst = (char*)pBuff;
        memcpy(pDst,pSrc,pVec->itemSize);
        return 0;
    }while(0);
    return -1;
}

/*return the item at 'index' position*/
void* Vec_At(vector_t *pVec,unsigned int index)
{
    if(0==pVec) return 0;
    if(pVec->itemCount<=index) return 0;
    return VEC_INDEX_VALUE(pVec,pVec->tail+index);
}

/*return the frist item in the vector*/
void* Vec_Frist(vector_t *pVec)
{
    return Vec_At(pVec,0);
}

/*return the last item in the vector*/
void *Vec_Last(vector_t *pVec)
{
    return Vec_At(pVec,pVec->itemCount-1);
}

/*return the number of items in the vector*/
int Vec_Count(vector_t *pVec)
{
    if(0==pVec) return -1;
    return pVec->itemCount;
}


/********************************************************************************************/
/*******************************       静态函数实现     ***************************************/
/********************************************************************************************/

static int Vec_AppendItemCount(vector_t* pVec, int addCount)
{
    if(0==pVec) return -1;
    if(0==addCount) return -1;

    int rsl;
    unsigned int minCount;
    unsigned int newCount;

    do{
        minCount = pVec->itemCount+addCount;
        if(minCount > pVec->itemMaxCountReal) break;

        if(minCount > pVec->itemMaxCount){
            newCount = pVec->itemMaxCount*2;
            if(newCount > pVec->itemMaxCountReal) newCount = pVec->itemMaxCountReal;
            if(newCount < minCount) newCount = minCount;

            rsl = Vec_Resize(pVec,newCount);
            if(0!=rsl) break;
        }
        return 0;
    }while(0);
    return -1;

}





