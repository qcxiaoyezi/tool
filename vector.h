#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/*打开输出*/
#define VEC_DEBUG_FLAG 1

#define VEC_FREE(P) if(0!=P){free(P); P=0;}

typedef struct vector_t{
    unsigned int    itemSize;
    unsigned int    itemCount;
    unsigned int    itemMaxCount;
    unsigned int    itemMaxCountReal;

    unsigned int    head;
    unsigned int    tail;

    char*  pDataBuff;
}vector_t;

/*constructs a empty vector*/
vector_t* Vec_CreateNew(int itemSize, int itemMaxCount);
/*delete a vector*/
void Vec_Delete(vector_t *pVec);
/*remove all item from the vector*/
void Vec_Clear(vector_t *pVec);
/*resize the pDataBuff size to contain more or less item*/
int Vec_Resize(vector_t *pVec, unsigned int count);

/*inserts 'value' at index position 'index' in the vector*/
int Vec_Insert(vector_t *pVec, void *pValue, int index);
/*inserts multiple 'value' at index position 'index' in the vector*/
int Vec_InsertEx(vector_t *pVec, void *pValue, unsigned int index, unsigned int num);
/*Inserts 'value' at the beginning of the vector*/
int Vec_PushFront(vector_t *pVec, void *pValue);
/*inserts 'value' at the end of the vector*/
int Vec_PushBack(vector_t *pVec, void *pValue);


/*remvoe the item at 'index' position*/
int Vec_RemoveAt(vector_t *pVec, int index);
/*remvoe multiple item at 'index' position*/
int Vec_RemoveAtEx(vector_t *pVec, unsigned int index, unsigned int num);
/*remove the frist item in the vector*/
int Vec_RemoveFrist(vector_t *pVec);
/*remove the last item in the vector*/
int Vec_RemoveLast(vector_t *pVec);

/*remove the item at index position 'index' and return it*/
int Vec_TakeAt(vector_t *pVec, void *pBuff, unsigned int index);
/*remove multiple item at index position 'index' and return it*/
int Vec_TakeAtEx(vector_t *pVec, void *pBuff, unsigned int index, unsigned int *num);
/*remove the frist itme in the vector and return it*/
int Vec_PopFront(vector_t *pVec, void *pBuff);
/*remove the last item in the vector and return it*/
int Vec_PopBack(vector_t *pVec, void *pBuff);

/*return the item at 'index' position*/
void* Vec_At(vector_t *pVec,unsigned int index);
/*return the frist item in the vector*/
void* Vec_Frist(vector_t *pVec);
/*return the last item in the vector*/
void *Vec_Last(vector_t *pVec);


/*return the number of items in the vector*/
int Vec_Count(vector_t *pVec);

#define VEC_INDEX(pVec,index)           \
    (((index)+(pVec)->itemMaxCount)%(pVec)->itemMaxCount)
#define VEC_INDEX_VALUE(pVec,index)     \
    (VEC_INDEX(pVec,index)*(pVec)->itemSize + (pVec)->pDataBuff)


#define Vec_Foreach_Variable            \
    vector_t*     Vec_Foreach_pVec = 0;   \
    unsigned int  Vec_Foreach_index = 0;
#define Vec_Foreach(pVec)               \
    Vec_Foreach_pVec = (pVec);          \
    if(0!=Vec_Foreach_pVec)             \
    for(Vec_Foreach_index = 0; Vec_Foreach_index < Vec_Foreach_pVec->itemCount; Vec_Foreach_index++)

#define Vec_Foreach_Ex(pVec,head,tail)               \
    Vec_Foreach_pVec = (pVec);          \
    if(0!=Vec_Foreach_pVec)             \
    for(Vec_Foreach_index = (head); (Vec_Foreach_index<Vec_Foreach_pVec->itemCount) && (Vec_Foreach_index<(tail)); Vec_Foreach_index++)

#define Vec_Foreach_Value                       \
    VEC_INDEX_VALUE(Vec_Foreach_pVec,Vec_Foreach_pVec->tail+Vec_Foreach_index)
#define Vec_Foreach_Value_Ex(increment)         \
    VEC_INDEX_VALUE(Vec_Foreach_pVec,Vec_Foreach_pVec->tail+Vec_Foreach_index+(increment))
#define Vec_Foreach_Value_Reverse               \
    VEC_INDEX_VALUE(Vec_Foreach_pVec,Vec_Foreach_pVec->head-1-Vec_Foreach_index)
#define Vec_Foreach_Value_Reverse_Ex(increment) \
    VEC_INDEX_VALUE(Vec_Foreach_pVec,Vec_Foreach_pVec->head-1-Vec_Foreach_index+(increment))

#define Vec_Foreach_Index (Vec_Foreach_index)
#define Vec_Foreach_RmCurNode do{Vec_RemoveAt(Vec_Foreach_pVec,Vec_Foreach_index); Vec_Foreach_index--; }while(0);


#ifdef __cplusplus
}
#endif
#endif
