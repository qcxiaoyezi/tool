#include <stdio.h>
#include "linklist.h"
#include "vector.h"
#include "ringbuff.h"


int main(void)
{
    ring_t *pRing;
    int rsl;


    do{
        pRing = Ring_CreateNew(1024);
        if(0==pRing) break;



        printf("ring success!\n");
        return 0;
    }while(0);
    printf("2333333333333333333\n");
    return -1;
}









#if 0
int main(void)
{
    LIST_FOREACH_VARIABLE;
    list_t *pList;
    int     i;
    int     c;

    do{
        pList = List_CreateNew(1);
        if(0==pList) break;
        for(i=0;i<20;i++){
            c = i+'a';
            List_PushBack(pList,&c);
        }
        c = '-';
        List_PushBack(pList,&c);
        List_PushFront(pList,&c);
        c = '+';
        List_Insert(pList,&c,3);

        List_RemoveFrist(pList);
        List_RemoveLast(pList);

        List_TakeAt(pList,&c,5);
        printf("%c\n",c);

        List_PopBack(pList,&c);
        printf("%c\n",c);

        List_PopFront(pList,&c);
        printf("%c\n",c);

        LIST_FOREACH(pList){
            c = *(char*)LIST_FOREACH_VALUE;
            printf("%2d:c = %c\n",LIST_FOREACH_INDEX,c);
        }




        printf("pList successs!\n");

        return 0;
    }while(0);
    printf("233333333333...\n");
    return 0;
}
#endif



