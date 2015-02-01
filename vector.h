#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct vector_t{
    unsigned int    itemSize;                  /*item 大小*/
    unsigned int    itemCount;                 /*item 数量*/
    unsigned int    itemCapacity;              /*item 容量*/
    unsigned int    itemMaxCapacity;           /*item 最大容量*/

    unsigned int    head;                      /*头下标*/
    unsigned int    tail;                      /*尾下标*/

    char*  pDataBuff;
}vector_t;

/*构建一个新的向量*/
vector_t* Vec_CreateNew(int itemSize, int itemMaxCount);
/*销毁一个向量*/
void Vec_Delete(vector_t *pVec);
/*清空一个向量*/
void Vec_Clear(vector_t *pVec);
/*改变一个向量的容量*/
int Vec_Resize(vector_t *pVec, unsigned int count);

/*在 index 位置插入一个新的 item*/
int Vec_Insert(vector_t *pVec, void *pValue, int index);
/*在 index 位置插入一个或多个新的 item*/
int Vec_InsertEx(vector_t *pVec, void *pValue, unsigned int index, unsigned int num);
/*在头部插入一个新的 item*/
int Vec_PushFront(vector_t *pVec, void *pValue);
/*在尾部插入一个新的 item*/
int Vec_PushBack(vector_t *pVec, void *pValue);


/*删除 index 位置的 item*/
int Vec_RemoveAt(vector_t *pVec, int index);
/*删除从 index 位置开始的一个或多个 item*/
int Vec_RemoveAtEx(vector_t *pVec, unsigned int index, unsigned int num);
/*删除第一个item*/
int Vec_RemoveFrist(vector_t *pVec);
/*删除最后一个item*/
int Vec_RemoveLast(vector_t *pVec);

/*取出 index 位置 item 的内容并移除它*/
int Vec_TakeAt(vector_t *pVec, void *pBuff, unsigned int index);
/*取出从 index 位置开始的一个或多个 item 的内容并移除它们*/
int Vec_TakeAtEx(vector_t *pVec, void *pBuff, unsigned int index, unsigned int *num);
/*取出第一个 item 的内容并移除它*/
int Vec_PopFront(vector_t *pVec, void *pBuff);
/*取出最后一个 item 的内容并移除它*/
int Vec_PopBack(vector_t *pVec, void *pBuff);

/*返回 index 位置 item 的内容指针*/
void* Vec_At(vector_t *pVec,unsigned int index);
/*返回第一个 item 的内容指针*/
void* Vec_Frist(vector_t *pVec);
/*返回最后一个 item 的内容指针*/
void *Vec_Last(vector_t *pVec);

/*返回一个向量当前 item 个数*/
int Vec_Count(vector_t *pVec);

#define VEC_INDEX(pVec,index)           \
    (((index)+(pVec)->itemCapacity)%(pVec)->itemCapacity)
#define VEC_INDEX_VALUE(pVec,index)     \
    (VEC_INDEX(pVec,index)*(pVec)->itemSize + (pVec)->pDataBuff)


#define VEC_FOREACH_VARIABLE            \
    vector_t*     Vec_Foreach_pVec = 0;   \
    unsigned int  Vec_Foreach_Index = 0;
#define VEC_FOREACH(pVec)               \
    Vec_Foreach_pVec = (pVec);          \
    if(0!=Vec_Foreach_pVec)             \
    for(Vec_Foreach_Index = 0; Vec_Foreach_Index < Vec_Foreach_pVec->itemCount; Vec_Foreach_Index++)

#define VEC_FOREACH_EX(pVec,head,tail)               \
    Vec_Foreach_pVec = (pVec);          \
    if(0!=Vec_Foreach_pVec)             \
    for(Vec_Foreach_Index = (head); (Vec_Foreach_Index<Vec_Foreach_pVec->itemCount) && (Vec_Foreach_Index<(tail)); Vec_Foreach_Index++)

#define VEC_FOREACH_VALUE                       \
    VEC_INDEX_VALUE(Vec_Foreach_pVec,Vec_Foreach_pVec->tail+Vec_Foreach_Index)
#define VEC_FOREACH_VALUE_Ex(increment)         \
    VEC_INDEX_VALUE(Vec_Foreach_pVec,Vec_Foreach_pVec->tail+Vec_Foreach_Index+(increment))
#define VEC_FOREACH_VALUE_REVERSE                \
    VEC_INDEX_VALUE(Vec_Foreach_pVec,Vec_Foreach_pVec->head-1-Vec_Foreach_Index)
#define VEC_FOREACH_VALUE_REVERSE_EX(increment) \
    VEC_INDEX_VALUE(Vec_Foreach_pVec,Vec_Foreach_pVec->head-1-Vec_Foreach_Index+(increment))

#define VEC_FOREACH_INDEX (Vec_Foreach_Index)

#ifdef __cplusplus
}
#endif
#endif
