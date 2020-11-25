//김하운, Assignment 3, customer_manager1.c
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define UNIT_ARRAY_SIZE 64

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
};

/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  /*새로운 d의 메모리를 할당하고 사용자 정보 공간인 pArray 공간을 할당한다
  메모리 할당에 성공한 경우 할당된 d 주소를 반환하고
  메모리 공간이 부족한 경우 stderr로 에러 메세지를 보내고 NULL을 반환*/
  DB_T d;

  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 64 elements
  d->pArray = (struct UserInfo *)calloc(d->curArrSize,
               sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);
    free(d);
    return NULL;
  }
  return d;
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  /*d 안에 할당되어있던 모든 메모리를 해제한다.*/
  if(d == NULL)
    return;
  for(int i = 0; i < d->curArrSize; i++){
    if((d->pArray + i)->name != NULL){
      free((d->pArray + i) ->name);
      free((d->pArray + i) ->id);
    }
  }
  free(d->pArray);
  free(d);
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /*d 의 사용자 목록인 pArray에 새로운 사용자 정보(id, name, purchase)를
  비어있는 배열에 저장한다. 비어있는 공간이 없으면 pArray 크기를 확장한 후
  저장한다.
  성공하면 0을 반환하고 d, id, name이 NULL 인경우, purchase<0 인 경우,
  이미 사용자 정보가 존재하는 경우 -1을 반환하고 메모리 공간이 부족하면
  stderr 로 에러 메세지를 보낸다.*/
  if(d == NULL || id == NULL || name == NULL)
    return (-1);
  //exception
  if(purchase <= 0)
    return (-1);
  //check existence
  for(int i = 0; i < d->curArrSize; i++){
    if((d->pArray+i)->name != NULL)
      if(strcmp((d->pArray+i)->id, id) == 0 ||
          strcmp((d->pArray+i)->name, name)==0)
          return (-1);
  }
  //register
  int j;
  //printf("%d\n", d->pArray == NULL);
  for(j = 0; j < d->curArrSize; j++){
    if((d->pArray + j)->name == NULL){
      (d->pArray + j)->name = strdup(name);
      (d->pArray + j)->id = strdup(id);
      (d->pArray + j)->purchase = purchase;
      break;
    }
  }
  //expand and register
  if(j == d->curArrSize){
    //expand
    d->curArrSize += UNIT_ARRAY_SIZE;
    d->pArray = (struct UserInfo *)realloc(d->pArray, 
                                sizeof(struct UserInfo)*d->curArrSize);
    if (d->pArray == NULL) {
      fprintf(stderr, "Can't allocate a memory for array of size %d\n",
        d->curArrSize);
      return (-1);
    }
    //register
    (d->pArray + j)->name = strdup(name);
    (d->pArray + j)->id = strdup(id);
    (d->pArray + j)->purchase = purchase;
  }
  return 0;
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /*d의 사용자 목록이 저장되어 있는 array인 pArray 에서 id와 일치한 
  사용자를 찾아 그 사용자의 정보를 해제하고 name 을 NULL 로 바꿈.
  성공하면 0을 반환하고 d 나 id가 NULL 인경우, 
  사용자에 대한 정보가 없는 경우 -1을 반환*/
  if(d == NULL || id == NULL)
    return (-1);
  for(int i = 0; i < d->curArrSize; i++){
    if((d->pArray+i)->name != NULL)
      if(strcmp((d->pArray + i)->id, id)==0){
        free((d->pArray + i)->name);
        free((d->pArray + i)->id);
        (d->pArray + i)->name = NULL;
        return 0;
      }
  }
  return (-1);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /*d의 사용자 목록이 저장되어 있는 array인 pArray 에서 name과 일치한 
  사용자를 찾아 그 사용자의 정보를 해제하고 name 을 NULL 로 바꿈.
  성공하면 0을 반환하고 d 나 name이 NULL 인경우, 
  사용자에 대한 정보가 없는 경우 -1을 반환*/
  if(d == NULL || name == NULL)
    return (-1);
  for(int i = 0; i < d->curArrSize; i++){
    if((d->pArray+i)->name != NULL)
      if(strcmp((d->pArray + i)->name, name)==0){
        free((d->pArray + i)->name);
        free((d->pArray + i)->id);
        (d->pArray + i)->name = NULL;
        return 0;
      }
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  /*d의 사용자 목록이 저장되어 있는 array인 pArray 에서 id와 일치한 
  사용자를 찾아 그 사용자의 purchase를 반환.
  d 나 id이 NULL 인경우, 사용자에 대한 정보가 없는 경우 -1을 반환*/
  if(d == NULL || id == NULL)
    return (-1);
  for(int i = 0; i < d->curArrSize; i++){
    if((d->pArray+i)->name != NULL)
      if(strcmp((d->pArray + i)->id, id)== 0){
        return (d->pArray + i)->purchase;
      }
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  /*d의 사용자 목록이 저장되어 있는 array인 pArray 에서 name과 일치한 
  사용자를 찾아 그 사용자의 purchase를 반환.
  d 나 name이 NULL 인경우, 사용자에 대한 정보가 없는 경우 -1을 반환*/
  if(d == NULL || name == NULL)
    return (-1);
  for(int i = 0; i < d->curArrSize; i++){
    if((d->pArray+i)->name != NULL)
      if(strcmp((d->pArray + i)->name, name)==0){
        return (d->pArray + i)->purchase;
      }
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /*d 안에 들어있는 모든 사용자에 대하여 사용자 정보(id, name, purchase)를
  입력으로 하는 함수fp 의 반환값을 모두 더한값을 반환한다.
  d나 fp가 NULL 일경우 -1을 반환*/
  if(d == NULL || fp == NULL)
    return (-1);
  int sum = 0;
  int result = 0;
  for(int i = 0; i < d->curArrSize; i++){
    if((d->pArray+i)->name != NULL){
      result = fp((d->pArray + i)->id, (d->pArray + i)->name, 
                                        (d->pArray + i)->purchase);
      sum += result;
    }
  }
  return sum;
}