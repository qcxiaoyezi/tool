#include "linklist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*constructs a empty list*/
List* List_CreateNew(int size)
{
    List *link = 0;

    do{
        link = (List*)malloc(sizeof(List));
        if(0==link) break;
        memset(link,0,sizeof(List));

        link->size = size;
        link->head = 0;
        link->tail = 0;
        link->count = 0;

        return link;
    }while(0);
    List_Delete(link);
    return 0;
}

/*delete a list*/
void List_Delete(List *list)
{
    if(0==list) return;
    List_Clear(list);
    List_FREE(list);
}

/*remove all item from the list*/
void List_Clear(List *list)
{
    List_Foreach_Variable;
    List_Foreach(list){
        List_Foreach_RmCurNode;
    }
}

/*create new list node*/
ListNode* List_CreateNewNode(List *list, void *value)
{
    ListNode* node = 0;

    if(0==list || 0==value || 0>list->size) return 0;

    do{
        node = (ListNode*)malloc(sizeof(ListNode));
        if(0==node) break;
        memset(node,0,sizeof(ListNode));

        if(0<list->size){
            node->data = malloc(list->size);
            if(0==node->data) break;
            memcpy(node->data,value,list->size);
        }else{
            node->data = value;
        }

        return node;
    }while(0);

    if(0!=node){
        List_FREE(node->data);
        List_FREE(node);
    }
    return 0;
}

/*delete list node*/
void List_DeleteNode(List *list,ListNode *node)
{
    if(0==list || 0==node) return;
    if(0<list->size){
        List_FREE(node->data);
    }
    List_FREE(node);
}

inline void List_AddNode(List* list, ListNode* pNode, ListNode* node)
{
    ListNode* nNode;
    if(0==list || 0==node) return;
    nNode = (0==pNode) ? list->head : pNode->next;
    node->previous = pNode;
    node->next = nNode;
    if(0==nNode){
        list->tail = node;
    }else{
        nNode->previous = node;
    }
    if(0==pNode){
        list->head = node;
    }else{
        pNode->next = node;
    }
    list->count ++;
}

inline void List_removeNode(List* list, ListNode* node)
{
    if(0==list || 0==node) return;
    if(0!=node->next){
        node->next->previous = node->previous;
    }
    if(0!=node->previous){
        node->previous->next = node->next;
    }
    if(list->head == node){
        list->head = node->next;
    }
    if(list->tail == node){
        list->tail = node->previous;
    }
    List_DeleteNode(list,node);
    list->count --;
}


/*inserts 'value' at index position 'index' in the list*/
int List_Insert(List *list, void *value ,int index)
{
    ListNode *node;
    ListNode *pNode;

    if(0==list || 0==value || 0>index) return -1;

    pNode = List_NodeAt(list,index-1);
    /*constructs a new node*/
    node = List_CreateNewNode(list,value);
    if(0==node) return -1;
    List_AddNode(list,pNode,node);

    return 0;
}

/*Inserts 'value' at the beginning of the list*/
int List_PushFront(List *list, void *value)
{
    return List_Insert(list,value,0);
}

/*inserts 'value' at the end of the list*/
int List_PushBack(List *list, void *value)
{
    ListNode *node;

    if(0==list || 0==value) return -1;
    /*constructs a new node*/
    node = List_CreateNewNode(list,value);
    if(0==node) return -1;

    List_AddNode(list,list->tail,node);
    return 0;
}

/*remvoe the item at 'index' position*/
int List_RemoveAt(List *list,int index)
{
    ListNode *node;
    if(0==list || 0==list->head || 0>index) return -1;

    node = List_NodeAt(list,index);
    List_removeNode(list,node);
    return 0;
}

/*remove the frist item in the list*/
int List_RemoveFrist(List *list)
{
    if(0==list) return -1;
    List_removeNode(list,list->head);
    return 0;
}

/*remove the last item in the list*/
int List_RemoveLast(List *list)
{
    if(0==list) return -1;
    List_removeNode(list,list->tail);
    return 0;
}


int List_TakeNode(List *list, ListNode *node, char *buff)
{
    if(0==list || 0==node || 0==buff) return -1;
    memcpy(buff,node->data,list->size);
    List_removeNode(list,node);
    return 0;
}

/*remove the item at index position 'index' and return it*/
int List_TakeAt(List *list, void *buff, int index)
{
    if(0==list || 0==buff || 0>index) return -1;
    return List_TakeNode(list,List_NodeAt(list,index),buff);
}

/*remove the frist itme in the list and return it*/
int List_PopFront(List *list, void *buff)
{
    if(0==list || 0==buff) return -1;
    return List_TakeNode(list,list->head,buff);
}

/*remove the last imte in the list and return it*/
int List_PopBack(List *list, void *buff)
{
    if(0==list || 0==buff) return -1;
    return List_TakeNode(list,list->tail,buff);
}

/*replaces the item at index position index with value*/
int List_Replace(List *list, void *value, int index)
{
    ListNode *pNode;
    if(0==list || 0==value || 0>index) return -1;

    pNode = List_NodeAt(list,index);
    if(0==pNode) return -1;

    memcpy(pNode->data,value,list->size);

    return 0;
}

inline ListNode *List_NodeAt(List *list, int index)
{
    int n;
    ListNode *node;
    if(0==list || 0>index) return 0;
    node = list->head;
    for(n=0;n<index;n++){
        if(0==node) break;
        node = node->next;
    }
    return node;
}

/*return the item at 'index' position index*/
void* List_At(List *list,int index)
{
    ListNode *node;
    node = List_NodeAt(list,index);
    return ((0==node)?0:node->data);
}

/*return the frist item in the list*/
void* List_Frist(List *list)
{
    return List_At(list,0);
}

/*return the last item in the list*/
void *List_Last(List *list)
{
    if(0==list) return 0;
    return ((0==list->tail)?0:list->tail->data);
}

/*return 1 is list is empty*/
int List_IsEmpty(List *list)
{
    if(0==list || 0==list->head) return 0;
    else return 1;
}

/*return the number of items in the list*/
int List_Count(List *list)
{
    return (0==list)?(-1):(list->count);
}

/*return the number of occurrences of 'value' in the list*/
int List_ValueCount(List *list, void *value)
{
    int num = 0;
    List_Foreach_Variable;
    if(0==list) return -1;
    List_Foreach(list){
        if(0==memcmp(value,List_Foreach_Value,List_Foreach_qList->size)) num++;
    }
    return num;
}

/*return the index position of the frist occurrence of 'value' in the list*/
int List_Indexof(List *list, void *value)
{
    int num;
    List_Foreach_Variable;
    if(0==list) return -1;

    num=0;
    List_Foreach(list){
        if(0==memcmp(value,List_Foreach_Value,List_Foreach_qList->size)) return num;
        num++;
    }
    return -1;
}

/*search Str*/
void* MListSearchStr(List *list, char *value ,int offset)
{
    ListNode *p;
    char *str;

    if(0==list || 0==list->head || 0==value || 0>offset) return 0;

    p = list->head;
    while(p){
        str = p->data;
        str += offset;
        str = *(char**)str;
        if(0==strcmp(str,value)) return p->data;
        p = p->next;
    }

    return 0;
}

/*search int*/
void* MListSearchInt(List *list, int value ,int offset)
{
    ListNode *p;
    char *cp;

    if(0==list || 0==list->head || 0==value || 0>offset) return 0;

    p = list->head;
    while(p){
        cp = p->data;
        cp += offset;

        if(*(int*)cp == value) return p->data;
        p = p->next;
    }

    return 0;
}
