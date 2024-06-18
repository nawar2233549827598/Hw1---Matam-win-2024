#include "RLEList.h"
#include <stdlib.h>
#include <assert.h>
#define BACK_SLASH_ZERO '\0'
#define TEN 10
#define NEW_LINE '\n'

struct RLEList_t {
    char val;
    int count;
    struct RLEList_t* next;

    struct RLEList_t* end;
    struct RLEList_t* first;

};

//implement the functions here
// first node is always dumy node
RLEList RLEListCreate(){

    RLEList list = malloc(sizeof(*list));
    if(list == NULL) {
        return NULL;
    }
    list->next= NULL;
    list->first=list;
    list->end=list;
    list->val = '\0';
    list->count =0;

    return list;

}



void RLEListDestroy(RLEList list){
    if(list == NULL) {
        return;
    }
    RLEList current = list;
    RLEList next;

    while (current != NULL) {
        next = current->next;  // Update next before freeing current
        free(current);
        current = next;
    }
}



RLEList getLast(RLEList list){
    // this function go the last node in the last
    RLEList temp=list;
    if(!temp&&temp->next==NULL)
        return NULL;
    while (temp ->next){
        temp=temp->next;
    }
    return temp;
}



RLEListResult RLEListAppend(RLEList list, char value) {
    if (list == NULL) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    RLEList temp = getLast(list);

    if (temp->val == value) {
        temp->count++;
        // No new node created, so no memory to free
        return RLE_LIST_SUCCESS;
    }

    RLEList newNode = malloc(sizeof(*newNode));
    if (!newNode) {
        return RLE_LIST_OUT_OF_MEMORY;
    }

    newNode->count = 1;
    newNode->val = value;
    newNode->next = NULL;
    temp->next = newNode;
    return RLE_LIST_SUCCESS;
}


int RLEListSize(RLEList list){

    if(list == NULL){
        return -1;
    }
    int size = 0;
    //size +=list->count;
    RLEList current = list->next;

    while (current){

        size +=current->count;
        current = current->next;
    }
    return size;
}

RLEListResult RLEListRemove(RLEList list, int index) {
    if (list == NULL) {
        return RLE_LIST_NULL_ARGUMENT;
    }

    int size = RLEListSize(list);

    if (index < 0 || index >= size) {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }

    RLEList previous = list;
    RLEList current = list->next;
    int currentIndex = 0;

    while (currentIndex+current->count <= index) {
        currentIndex+=current->count;

        previous = current;
        current = current->next;
        if(0) {
            int countChar = 0;
            while (currentIndex < index && countChar < current->count) {
                currentIndex++;
                countChar++;
            }

            if (currentIndex >= index && countChar < current->count) {
                break;
            }
            previous = current;
            current = current->next;
        }

    }
    assert(current!=NULL);
    if(0) {
        if (current == NULL) {
            // Handle the case where the index is out of bounds or there's an issue with the list
            return RLE_LIST_NULL_ARGUMENT;
        }
    }
    if(current->count>1){
        current->count--;
        return RLE_LIST_SUCCESS;
    }
    RLEList nextNode = current->next;
    previous->next=nextNode;
    current->next=NULL;
    RLEListDestroy(current);
    // merge previous and nextNode if possible
    if(nextNode!=NULL && previous->val==nextNode->val){
        RLEList nextNextNode = nextNode->next;
        previous->count+=nextNode->count;
        nextNode->next=NULL;
        RLEListDestroy(nextNode);

        previous->next=nextNextNode;
        return RLE_LIST_SUCCESS;
    }
//    if (current->count == 1) {
//        if (current->next != NULL && current->next->val == previous->val) {
//            previous->count += current->next->count;
//            RLEList tmp = current->next;
//            previous->next = current->next->next;
//            free(tmp);
//        } else {
//            RLEList toDelete = current;
//            previous->next = current->next;
//            free(toDelete);
//        }
//    } else {
//        current->count -= 1;
//    }

    return RLE_LIST_SUCCESS;
}



char RLEListGet(RLEList list, int index, RLEListResult *result) {
    if (!list) {
        if (result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return 0;
    }
    if (index < 0 || index >= RLEListSize(list)) {
        if (result) {
            *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        }
        return 0;

    }

    RLEList current = list->next;

    while (current && index >= current->count) {
        index -= current->count;
        current = current->next;
    }

    if (result) {
        *result = RLE_LIST_SUCCESS;
    }

    return current->val;
}


int num_counting(int num){

    // if i have 100 i will return 3
    int count = 0;
    int temp =num;
    while (temp != 0) {
        temp /= 10;
        count++;
    }
    return count;




}

void saveResult( RLEListResult* result, RLEListResult value){
    if(result){
        *result=value;
    }
}
char* RLEListExportToString(RLEList list, RLEListResult* result) {
    if (list==NULL){
        saveResult(result,RLE_LIST_NULL_ARGUMENT);
        return NULL;
    }

    int size =0;

    RLEList current = list->next;
    while (current) {
        size += num_counting(current->count) + 2;
        current = current->next;
    }



    char* string= malloc((sizeof(char )*size+1));
    if (string==NULL){
        saveResult(result,RLE_LIST_OUT_OF_MEMORY);
        return NULL;
    }

    int i=0;
    for (RLEList current=list->next;current!=NULL;current=current->next){
        string[i]=current->val;
        i++;
        int items = num_counting((current->count));
        int temp=current->count;
        for (int j = items; j>0; --j) {
            string[i + j - 1] = (temp % 10) + '0';
            temp /= 10;
        }
        i+=items;
        string[i]='\n';
        i++;
        string[i] = '\0';
    }
//    string[i] = '\0';
    saveResult(result,RLE_LIST_SUCCESS);
    return string;

}

void printList(RLEList list){
    if(list==NULL){
        fprintf(stderr,"null\n");
    }
    for(RLEList current=list->next;current!=NULL;current=current->next){
        fprintf(stderr,"'%c':%d\n",current->val,current->count);
    }
}







RLEListResult RLEListMap(RLEList list, MapFunction map_function){

    if(list == NULL || map_function == NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEList current = list->next;
    while (current){

        current->val=map_function(current->val);
        current = current->next;
    }
    RLEList previous = list;
    RLEList ptr = list->next;
    while (ptr)
    {
        if(previous->val == ptr->val){

            RLEList temp = ptr;
            previous->count +=ptr->count;
            previous->next=ptr->next;
            ptr=ptr->next;
            free(temp);
            continue;

        }
        previous=ptr;
        ptr=ptr->next;
    }

    return RLE_LIST_SUCCESS;



}