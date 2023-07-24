#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "list.h"
#include <time.h>
List *ListOfFriends = NULL; // for storing all friends information in linklist
ContiguousList *arraylist = NULL; //this array contains all the information of all the upcomming birthdays of current month
char const *Months[12] = {"January","February","March","April","May","June","July","August","September",
"October","November", "December"};
int numberOfRecords = 0;  // this keeps the track record of total records in file 
int numberOfPreviousRecords = 0; 
bool isChanged = false; 
#define MONTHNAME(n) (n > 12) ? "invalid Month" : Months[n-1] // this returns the name of month 
//this functions returns the current date
date current_Time(){
    date now;
    time_t ti = time(NULL);
    struct tm *Ti = localtime(&ti);
    now.day = Ti->tm_mday;
    now.month = Ti->tm_mon+1;
    now.year = Ti->tm_year+1900;
    return now;
}
// this function fetches all data from file 
void LoadData(){
    char NemberOfRecords[10];
    int n;
    struct friend f;
    FILE *BirthDayRecords = fopen("Record.dat","r");
    FILE *Nrecords = fopen("Nrecord.txt","r");
    if(BirthDayRecords == NULL || Nrecords == NULL){
          printf("There are some error data not found !\n");
          return;
    }
    fgets(NemberOfRecords,10,Nrecords);
    n = atoi(NemberOfRecords);
    numberOfRecords = numberOfPreviousRecords = n;
    for(int i = 0; i < n; ++i){
        fread(&f,sizeof(struct friend),1,BirthDayRecords);
        ListOfFriends = addfriendAtLast(ListOfFriends,f);
    }
    fclose(BirthDayRecords);
    fclose(Nrecords);
}
// this function put back all data to the file
void PutDataBack(){
    FILE *BirthDayRecords = NULL;
    FILE *Nrecords = NULL;
   int n = numberOfRecords;
   bool Isupdate = ((numberOfPreviousRecords == numberOfRecords && isChanged) || numberOfRecords != numberOfPreviousRecords);
   // if there is any update in list 
   if(Isupdate){
      BirthDayRecords = fopen("Record.dat","w");
      Nrecords = fopen("Nrecord.txt","w");
      for(int i = 0; i < n; ++i)
      {
        if(ListOfFriends == NULL){
            return;
        }
           fwrite(&ListOfFriends->info,sizeof(struct friend),1,BirthDayRecords);
          ListOfFriends = ListOfFriends->next;
       }
        fprintf(Nrecords,"%d",numberOfRecords);
        fclose(BirthDayRecords);
       fclose(Nrecords);
   }
    return;
}
// this function discards all the previous data from the list
// and creates a fresh new list
void createList(){
    int choise; // for choice y/n
    printf("1 is for yes\n0 is for no\n");
    printf("Are you sure you want to continue\nthis will erase all of your data \n 0/1  : ");
    scanf("%d",&choise);
    printf("\n");
    if(!choise){
        exit(0);
    }
    int n = 0;
    struct friend f;
    // removing all previous data from list
    while(n < numberOfRecords){
         ListOfFriends = Remove_Friend_From_Front(ListOfFriends);
         n++;
    }
    //deallocating the arrayList if it was allocated 
    if(arraylist != NULL)
    {
        if(arraylist->array != NULL)
        {
            free(arraylist->array);
        }
        free(arraylist);
        arraylist = NULL;
    }
    n = 0;
    printf("Enter how many records you want to store : ");
    scanf("%d",&n);
    for(int i = 0; i < n; ++i){
         printf("Name : ");
        scanf("%s",f.name);
         printf("day month year ");
         scanf("%d%d%d",&f.Dob.day,&f.Dob.month,&f.Dob.year);
         ListOfFriends = addfriendAtLast(ListOfFriends,f);
    }
    numberOfRecords = n;
}
// print all informations of upcomming birthdays in Current Month
void ShowAllBirthDaysOfCurrentMonth(List *list)
{
    int CurrentMonth = current_Time().month;
    int CurrentDay = current_Time().day;
    bool thereAreNoBirthdaysInCurrentMonth;
    List *start = NULL;
    if(arraylist == NULL)
    {
        arraylist = (ContiguousList*)malloc(sizeof(ContiguousList));
        arraylist->bufferSize = numberOfRecords; 
        arraylist->array = (struct friend*)malloc(arraylist->bufferSize*sizeof(struct friend));
        arraylist->OccupiedIndex = -1;
        start = list;
       while(start)
       {
            if(start->info.Dob.month == CurrentMonth && start->info.Dob.day >= CurrentDay)
            {
                arraylist->OccupiedIndex++;
                arraylist->array[arraylist->OccupiedIndex] = start->info;
            }
            start = start->next;
       }
       //check if there is no birthdays in current month
       thereAreNoBirthdaysInCurrentMonth = arraylist->OccupiedIndex == -1;
       if(thereAreNoBirthdaysInCurrentMonth)
       {
        printf("No birthdays are in month %s",MONTHNAME(CurrentMonth));
        free(arraylist->array);
        free(arraylist);
        arraylist = NULL;
        return;
       }
       sortContigiousList(&arraylist);
    }
        printf("Birthdays in %s\n",MONTHNAME(CurrentMonth));

        for (int i = 0; i < arraylist->OccupiedIndex+1; i++)
        {
           printf("%d. %s             %d %s\n",i+1,
           arraylist->array[i].name,
           arraylist->array[i].Dob.day,
           MONTHNAME(CurrentMonth)
           );
        }
}
void ShowAllBirthDaysOfPerticularMonth()
{
    List *start = ListOfFriends;
    int i = 0,MonthNum;
    int isvalid = 0;
    printf("Enter the month number : ");
    scanf("%d",&MonthNum);
    isvalid = (MonthNum <= 0 || MonthNum > 12);
    if(isvalid)
    {
        printf("Invalid month !");
        return;
    }
    while(start)
    {
       if(start->info.Dob.month == MonthNum)
       {
           printf("%d .%s\n",i+1,start->info.name);
           i++;
       }
       start = start->next; 
    }
    if(!i)
    {
        printf("No birthdays are in month %s\n",MONTHNAME(MonthNum));
    }
}
void ShowBirthDaysOfCurrentDay()
{
    int i,n;
    date currTime = current_Time();
    List *start = NULL;
    bool isBirthday = false;
    //allocating the arraylist if it was not allocated 
    if(arraylist == NULL)
    {
        arraylist = (ContiguousList*)malloc(sizeof(ContiguousList));
        arraylist->bufferSize = numberOfRecords; 
        arraylist->array = (struct friend*)malloc(arraylist->bufferSize*sizeof(struct friend));
        arraylist->OccupiedIndex = -1;
        start = ListOfFriends;
       while(start)
       {
            if(start->info.Dob.month == currTime.month && start->info.Dob.day >= currTime.day)
            {
                arraylist->OccupiedIndex++;
                arraylist->array[arraylist->OccupiedIndex] = start->info;
            }
            start = start->next;
       }
    }
    if(arraylist->OccupiedIndex == -1)
    {
        printf("Today there are no birthdays\n ");
        free(arraylist->array);
        free(arraylist);
        arraylist = NULL;
        return;
    }


    n = arraylist->OccupiedIndex+1;
    for(i = 0; i < n; ++i)
    {
        if(arraylist->array[i].Dob.day == currTime.day){
            printf("Today is %s's birthday \n",arraylist->array[i].name);
            isBirthday == false ? (isBirthday = true) : 0;
        }
    }
    if(!isBirthday)
       printf("Today there are no birthdays\n ");
}
//this function updates the list 
void update(){
     struct friend f;
     int position = -1;
     int select;
     printf("1. add friend at front of List \n");
     printf("2. add friend at back of List\n");
     printf("3. add friend at between of List\n\n");
     printf("4. Remove friend from front\n");
     printf("5. Remove friend from back of list\n");
     printf("select : ");
     scanf("%d",&select);
    switch (select)
    {
     case 1:
         printf("Name : ");
        scanf("%s",f.name);
         printf("day month year i.e DOB");
         scanf("%d%d%d",&f.Dob.day,&f.Dob.month,&f.Dob.year);
         ListOfFriends = addfriendAtfront(ListOfFriends,f);
         //check if the added friend birth month is equal to the current time month
         if((f.Dob.month == current_Time().month && f.Dob.day >= current_Time().day) && arraylist != NULL)
         {
            InsertInContigiousList(&arraylist,f);
         }
         isChanged = true;
         numberOfRecords++;
        break;
     case 2:
      printf("Name : ");
        scanf("%s",f.name);
         printf("day month year ");
         scanf("%d%d%d",&f.Dob.day,&f.Dob.month,&f.Dob.year);
         ListOfFriends = addfriendAtLast(ListOfFriends,f);
          //check if the added friend birth month is equal to the current time month
         if((f.Dob.month == current_Time().month && f.Dob.day >= current_Time().day) && arraylist != NULL)
         {
             InsertInContigiousList(&arraylist,f);
         }
         isChanged = true;
         numberOfRecords++;
         break;
     case 3:
         printf("Name : ");
         scanf("%s",f.name);
         printf("day month year ");
         scanf("%d%d%d",&f.Dob.day,&f.Dob.month,&f.Dob.year);
         printf("which position you want to add of list : ");
         scanf("%d",&position);
         ListOfFriends = addFriendAtBetween(ListOfFriends,position,f);
         //check if the added friend birth month is equal to the current time month
         if((f.Dob.month == current_Time().month && f.Dob.day >= current_Time().day) && arraylist != NULL)
         {
             InsertInContigiousList(&arraylist,f);
         }
         isChanged = true;
         numberOfRecords++;
         break;
     case 4:
            ListOfFriends = Remove_Friend_From_Front(ListOfFriends);
            isChanged = true;
            numberOfRecords--;
            break;
     case 5:
           ListOfFriends = Remove_Friend_From_Last(ListOfFriends);
           isChanged = true;
           numberOfRecords--;
           break;
     default:
        printf("No functions are added at this selection \n");
        break;
     }
}
//this function calculates the age
int getAge(date dob)
{
      date currentTime = current_Time();
      int currentYear = currentTime.year;
      int age = currentYear - dob.year;
      if(currentTime.month < dob.month || (currentTime.month == dob.month && currentTime.day < dob.day))
            age = age - 1;
      return age;
}
void showAllFriendsBirthDays(){
    List *head = ListOfFriends;
    int serialNo = 1;
    if(head != NULL){
         printf("S.no   Name                               DOB          age\n");
          while(head){
           printf("%d.  %s                   ",serialNo,head->info.name);
         printf("%d-%d-%d           %d\n\n",
         head->info.Dob.day,
         head->info.Dob.month,
         head->info.Dob.year,
         getAge(head->info.Dob));
         head  = head->next;
         serialNo++;
        }
    }
    else
       printf("There are no records \n");
}
int main(int argc, char const *argv[])
{
    int choice;
    int IsNotRunning  = 0;
    LoadData();
    while(!IsNotRunning){
       printf("\t\tBIRTHDAY REMINDER \t \t \n");
       printf("1.Show friend Birthdays \n");
       printf("2. Edit list \n");
       printf("3.Show Birthdays of current Month \n");
       printf("4.show Birthdays of Current day \n");
       printf("5.Show all birthDays of perticular month \n");
       printf("6.Create new records \n");
       printf("7.Exit \n");
       printf("Choice : ");
       scanf("%i",&choice);
    switch (choice)
    {
    case 1:
        showAllFriendsBirthDays();
        break;
    case 2:
      update();
    break;
    case 3:
       ShowAllBirthDaysOfCurrentMonth(ListOfFriends);
    break;
    case 4:
     ShowBirthDaysOfCurrentDay();
    break;
    case 5:
        ShowAllBirthDaysOfPerticularMonth();
    break;
    case 6:
      createList();
      break;
    case 7:
      IsNotRunning = 1;
      break;
    default:
        break;
    }
       getch();
       system("cls");
    }
   PutDataBack();
   printf("\t\tBIRTHDAY REMINDER \t \t \n");
   printf("\t\t Closed \n"); 
   getch(); // holding the execution until any key is not pressed
    return 0;
}