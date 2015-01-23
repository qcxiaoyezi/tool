#ifndef LINKList_H
#define LINKList_H

#ifdef __cplusplus
extern "C" {
#endif

#define List_FREE(P) if(0!=P){free(P); P=0;}

typedef struct ListNode{
    struct ListNode *previous;
    struct ListNode *next;
    void *data;
}ListNode;

typedef struct List{
    int size;                              /*list node size*/
    int count;                             /*list node count*/
    ListNode *head;
    ListNode *tail;
}List;

/*constructs a empty list*/
List* List_CreateNew(int size);
/*delete a list*/
void List_Delete(List *list);
/*remove all item from the list*/
void List_Clear(List *list);
/*create new listnode*/
ListNode* List_CreateNewNode();
/*delete a list node*/
void List_DeleteNode(List *list,ListNode *node);
inline void List_AddNode(List* list, ListNode* pNode, ListNode* node);
inline void List_removeNode(List* list, ListNode* node);

/*inserts 'value' at index position 'index' in the list*/
int List_Insert(List *list, void *value ,int index);
/*Inserts 'value' at the beginning of the list*/
int List_PushFront(List *list, void *value);
/*inserts 'value' at the end of the list*/
int List_PushBack(List *list, void *value);


/*remvoe the item at 'index' position*/
int List_RemoveAt(List *list,int index);
/*remove the frist item in the list*/
int List_RemoveFrist(List *list);
/*remove the last item in the list*/
int List_RemoveLast(List *list);


int List_TakeNode(List *list, ListNode *node, char *buff);
/*remove the item at index position 'index' and return it*/
int List_TakeAt(List *list, void *buff, int index);
/*remove the frist itme in the list and return it*/
int List_PopFront(List *list, void *buff);
/*remove the last item in the list and return it*/
int List_PopBack(List *list, void *buff);


/*replaces the item at index position 'index' with 'value'*/
int List_Replace(List *list, void *value, int index);

/*return the node at index postion 'index'*/
inline ListNode *List_NodeAt(List *list, int index);
/*return the item at 'index' position*/
void* List_At(List *list,int index);
/*return the frist item in the list*/
void* List_Frist(List *list);
/*return the last item in the list*/
void *List_Last(List *list);

/*return 1 is list is empty*/
int List_IsEmpty(List *list);
/*return the number of items in the list*/
int List_Count(List *list);
/*return the number of occurrences of 'value' in the list*/
int List_ValueCount(List *list, void *value);
/*return the index position of the frist occurrence of 'value' in the list*/
int List_Indexof(List *list, void *value);

/*search Str*/
void* MListSearchStr(List *list, char *value ,int offset);
/*search Int*/
void* MListSearchInt(List *list, int value ,int offset);

#define List_Foreach_Variable \
    List*     List_Foreach_qList = 0; \
    ListNode* List_Foreach_pNode = 0; \
    ListNode* List_Foreach_qNode = 0;

#define List_Foreach(list) \
    List_Foreach_qList = list; \
    List_Foreach_pNode = (0==list)?(0):(List_Foreach_qList->head); \
    while((0==List_Foreach_pNode)?(0):(List_Foreach_qNode = List_Foreach_pNode,List_Foreach_pNode=List_Foreach_pNode->next,1))
#define List_Foreach_Value (List_Foreach_qNode->data)

#define List_Foreach_RmCurNode List_removeNode(List_Foreach_qList,List_Foreach_qNode);
/*watch value*/
#define WatchInt(x) printf("int %s's value is %d\n",#x,x)
#define WatchFloat(x) printf("float %s's value is %lf\n",#x,x)
#define WatchString(x) printf("str %s's value is %s\n",#x,x)
#define WatchPointer(x) printf("pointer %s's value is %p\n",#x,x)
#define Mark(x) printf("\nmark: %s\n",#x);

#ifdef __cplusplus
}
#endif

#endif
