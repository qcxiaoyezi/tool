#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define VEC_DEBUG_FLAG 1    /*打开输出*/
/********************************************************************************************/
/*******************************           宏         ***************************************/
/********************************************************************************************/

#if VEC_DEBUG_FLAG
#define VEC_DEBUG(fmt,args...) printf("DEBUG %s-%d: "fmt"\n",__FUNCTION__,__LINE__,## args);
#else
#define VEC_DEBUG(fmt,args...)
#endif

#define VEC_FREE(ptr) do{if(0==(ptr))break; free(ptr); ptr=0; }while(0)

/********************************************************************************************/
/*******************************       静态函数声明     ***************************************/
/********************************************************************************************/

static int Vec_AppendItemCount(vector_t* pVec, int num);


/********************************************************************************************/
/*******************************         函数实现      ***************************************/
/********************************************************************************************/


/*构建一个新的向量*/
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
        pVec->itemCapacity = itemMaxCount;
        pVec->itemMaxCapacity = 65535;

        pVec->head = 0;
        pVec->tail = 0;

        pVec->pDataBuff = (char*)malloc(pVec->itemSize*pVec->itemCapacity);
        if(0==pVec->pDataBuff) break;

        return pVec;
    }while(0);
    Vec_Delete(pVec);
    return 0;
}

/*销毁一个向量*/
void Vec_Delete(vector_t *pVec)
{
    if(0==pVec) return;
    VEC_FREE(pVec->pDataBuff);
    VEC_FREE(pVec);
}

/*清空一个向量*/
void Vec_Clear(vector_t *pVec)
{
    if(0==pVec) return;
    pVec->head = 0;
    pVec->tail = 0;
    pVec->itemCount = 0;
}

/*改变一个向量的容量*/
int Vec_Resize(vector_t *pVec, unsigned int count)
{
    if(0==pVec) return -1;
    if(pVec->itemCapacity == count) return 0;
    if(pVec->itemMaxCapacity < count) return -1;

    VEC_FOREACH_VARIABLE;
    char  *pNewBuff   = 0;
    char  *pDst       = 0;
    char  *pSrc       = 0;

    do{
        pNewBuff = (char*)malloc(pVec->itemSize * count);
        if(0==pNewBuff) break;

        VEC_FOREACH(pVec){
            if(VEC_FOREACH_INDEX >= count) break;
            pSrc = VEC_FOREACH_VALUE;
            pDst = VEC_FOREACH_INDEX*pVec->itemSize + pNewBuff;
            memcpy(pDst,pSrc,pVec->itemSize);
        }

        pVec->itemCount = VEC_FOREACH_INDEX;
        pVec->itemCapacity = count;
        pVec->head = VEC_INDEX(pVec,VEC_FOREACH_INDEX);
        pVec->tail = 0;

        VEC_FREE(pVec->pDataBuff);
        pVec->pDataBuff = pNewBuff;
        return 0;
    }while(0);
    VEC_FREE(pNewBuff);
    return -1;
}

/*在 index 位置插入一个新的 item*/
int Vec_Insert(vector_t *pVec, void *pValue, int index)
{
    return Vec_InsertEx(pVec, pValue, index, 1);
}

/*在 index 位置插入一个或多个新的 item*/
int Vec_InsertEx(vector_t *pVec, void *pValue, unsigned int index, unsigned int num)
{
    if(0==pVec || 0==pValue) return -1;
    if(index >= pVec->itemCount) index = pVec->itemCount;

    VEC_FOREACH_VARIABLE;
    char          *pSrc = 0;
    char          *pDst = 0;
    int            rsl  = 0;
    unsigned int   i    = 0;

    do{
        /*resize*/
        rsl = Vec_AppendItemCount(pVec,num);
        if(0!=rsl) break;

        VEC_FOREACH(pVec){
            if(index+VEC_FOREACH_INDEX >= pVec->itemCount) {break;}
            pSrc = VEC_FOREACH_VALUE_REVERSE;
            pDst = VEC_FOREACH_VALUE_REVERSE_EX(num);
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

/*在头部插入一个新的 item*/
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

/*在尾部插入一个新的 item*/
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

/*删除 index 位置的 item*/
int Vec_RemoveAt(vector_t *pVec,int index)
{
    return Vec_RemoveAtEx(pVec,index,1);
}

/*删除从 index 位置开始的一个或多个 item*/
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

/*删除第一个item*/
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

/**删除最后一个item*/
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

/*取出 index 位置 item 的内容并移除它*/
int Vec_TakeAt(vector_t *pVec, void *pBuff, unsigned int index)
{
    unsigned int num = 1;
    return Vec_TakeAtEx(pVec,pBuff,index,&num);
}

/*取出从 index 位置开始的一个或多个 item 的内容并移除它们*/
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

/*取出第一个 item 的内容并移除它*/
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

/*取出最后一个 item 的内容并移除它*/
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

/*返回 index 位置 item 的内容指针*/
void* Vec_At(vector_t *pVec,unsigned int index)
{
    if(0==pVec) return 0;
    if(pVec->itemCount<=index) return 0;
    return VEC_INDEX_VALUE(pVec,pVec->tail+index);
}

/*返回第一个 item 的内容指针*/
void* Vec_Frist(vector_t *pVec)
{
    return Vec_At(pVec,0);
}

/*返回最后一个 item 的内容指针*/
void *Vec_Last(vector_t *pVec)
{
    return Vec_At(pVec,pVec->itemCount-1);
}

/*返回一个向量当前 item 个数*/
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
        if(minCount > pVec->itemMaxCapacity) break;

        if(minCount > pVec->itemCapacity){
            newCount = pVec->itemCapacity*2;
            if(newCount > pVec->itemMaxCapacity) newCount = pVec->itemMaxCapacity;
            if(newCount < minCount) newCount = minCount;

            rsl = Vec_Resize(pVec,newCount);
            if(0!=rsl) break;
        }
        return 0;
    }while(0);
    return -1;

}





