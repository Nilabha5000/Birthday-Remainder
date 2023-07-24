#include "list.h"
#include <string.h>
#include <stdlib.h>
/*
    Linkedlist functions are :
    1. addfriendAtfront()
    2. addfriendAtLast()
    3. Remove_Friend_From_Front()
    4. Remove_Friend_From_Last()
    5. create()

    create() :- it is a primary function which creates a linkedlist node
    this function is not for user 
    
*/
// it creates a list node 
List *create(struct friend f){
    List *newinfo = (List*)malloc(sizeof(List));
   strcpy(newinfo->info.name ,f.name);
    newinfo->info.Dob.day = f.Dob.day;
    newinfo->info.Dob.month = f.Dob.month;
    newinfo->info.Dob.year = f.Dob.year;
    newinfo->next = NULL;
    return newinfo;
}
// function is use to add friend at first of list
List *addfriendAtfront(List *head,struct friend f){
     if(head == NULL){
        head = create(f);
        return head;
     }
    List *node = create(f);
    node->next = head;
    head = node;
    return head;
}
List*addfriendAtLast(List * head, struct friend f){
       // this is for when list is empty
        if(head == NULL){
        head = create(f);
        return head;
     }
     List *head1 = head;
     while(head1->next != NULL){
          head1 = head1->next;
     }
     head1->next = create(f);
     return head;
}
// this function is use to remove friend from the front of list
List *Remove_Friend_From_Front(List *head){
     List *head1 = head;
     if(head1 == NULL)
        return head1;
     head = head->next;
     free(head1);
     return head;
}
// this function removes friend infomation from back of the list
List *Remove_Friend_From_Last(List* head)
{
     List *start = head;
     List *prev = NULL;
     while(start->next != NULL)
     {
         prev = start;
         start = start->next;
     }
     prev->next = NULL;
     free(start);
     return head;
}
//this function is add at the between of the list
List* addFriendAtBetween(List *head , int pos, struct friend f)
{
     int i = 1;
     List *start = head;
     List *newFriend = create(f);
     while(i < pos-1){
          start = start->next;
          i++;
     }
     newFriend->next = start->next;
     start->next = newFriend;
     return head;
}

/*array functions*/

//this function sorts the contigious list on the basis of birthdays
void sortContigiousList(ContiguousList**list)
{
     int i,j,day;
     struct friend key;
     int n = (*list)->OccupiedIndex+1;
     for(i = 1; i < n; ++i)
     {
          key = (*list)->array[i];
          day = key.Dob.day;
          for(j = i-1; j >= 0 && day < (*list)->array[j].Dob.day; --j){
               (*list)->array[j+1] = (*list)->array[j];
          }
          (*list)->array[j+1] = key;
     }
}

//this functions insert an element in a sorted Contigious list
bool InsertInContigiousList(ContiguousList**list, struct friend f)
{
     if((*list)->OccupiedIndex == (*list)->bufferSize-1)
            return false;
     int day = f.Dob.day,i=(*list)->OccupiedIndex;
     
     for(i; i >= 0 && f.Dob.day < (*list)->array[i].Dob.day; --i){
          (*list)->array[i+1] = (*list)->array[i];
     }
     (*list)->array[i+1] = f;
     return true; 
}