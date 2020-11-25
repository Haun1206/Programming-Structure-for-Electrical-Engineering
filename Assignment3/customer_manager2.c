//김하운, Assignment 3, customer_manager2.c
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define UNIT_BUCKET_SIZE 1024

enum {
  HASH_MULTIPLIER = 65599
};

struct UserInfo {
  char *name;                 // customer name
  char *id;                   // customer id
  int purchase;               // purchase amount (> 0)
  struct UserInfo *next_id;   // next node in a id bucket
                              // next is null == last
  struct UserInfo *prev_id;   // previous node in a id bucket
                              // prev is null == beginning
  struct UserInfo *next_name; // next node in a name bucket
  struct UserInfo *prev_name; // previous node in a name bucket
};

struct DB {
  struct UserInfo **idhash;  //id hash table
  struct UserInfo **namehash;//name hash table
  int curbucketSize;         // current bucket size
  int numItems;              // # of stored items, needed to determine
};

static int hash_function(const char *pcKey, int iBucketCount)

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)(uiHash % (unsigned int)iBucketCount);
}

/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  /*새로운 d의 메모리를 할당하고 hash table idhash, nmaehash를 할당한다
  메모리 할당에 성공한 경우 할당된 d 주소를 반환하고
  메모리 공간이 부족한 경우 stderr로 에러 메세지를 보내고 NULL을 반환*/
  DB_T d;

  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curbucketSize = UNIT_BUCKET_SIZE;
  d->idhash = (struct UserInfo **)calloc(d->curbucketSize,
               sizeof(struct UserInfo *));
  d->namehash = (struct UserInfo **)calloc(d->curbucketSize,
               sizeof(struct UserInfo *));
  if (d->idhash == NULL || d->namehash == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curbucketSize);
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
  struct UserInfo *cur;
  struct UserInfo *next;
  //Destroy nodes using ID
  for(int i = 0; i < d->curbucketSize; i++){
    if(d->idhash[i] != NULL){
      cur = d->idhash[i];
      while(cur){
        next = cur->next_id;
        free(cur);
        cur = next;
      }
    }
  }
  //Destory hash table & db
  free(d->idhash);
  free(d->namehash);
  free(d);
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /*d 의 hash table idhash, name hash에 새로운 사용자 정보
  (id, name, purchase)에 대한 hash key들을 구하고 UserInfo node 메모리를
  할당하여 해당 bucket에 저장한다.
  node 수가 hashtable buket 수의 75%가 넘으면 bucket을 두배로 확장한다.
  성공하면 0을 반환하고 d, id, name이 NULL 인경우, purchase<0 인 경우,
  이미 사용자 정보가 존재하는 경우 -1을 반환하고 메모리 공간이 부족하면
  stderr 로 에러 메세지를 보내고 -1을 반환한다*/
  if(d == NULL || id == NULL || name == NULL)
    return (-1);
  if(purchase <= 0)
    return (-1);
  //determine keys
  int key_id = hash_function(id, d->curbucketSize);
  int key_name = hash_function(name, d->curbucketSize);
  //check existence using id
  struct UserInfo *cur;
  cur = d->idhash[key_id];
  
  while(cur){
    if(strcmp(cur->id, id)==0)
      return (-1);
    cur = cur->next_id;
  }
  cur = d->namehash[key_name];
  while(cur){
    if(strcmp(cur->name, name)==0)
      return (-1);
    cur = cur->next_name;
  }

  //register
  struct UserInfo *cur_id = d->idhash[key_id];
  struct UserInfo *cur_name = d->namehash[key_name];
  struct UserInfo *new = (struct UserInfo *)calloc(1,
                                          sizeof(struct UserInfo));
  if (new == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return (-1);
  }
  new->name = strdup(name);
  new->id = strdup(id);
  new->purchase = purchase;
  new->next_id = cur_id;
  new->prev_id = NULL; //prev is null means this is first element.
  new->next_name = cur_name;
  new->prev_name = NULL;

  if(cur_id != NULL){
    cur_id->prev_id = new;
  }
  if(cur_name != NULL){
    cur_name->prev_name = new;
  }
  d->idhash[key_id] = new;
  d->namehash[key_name] = new;
  d->numItems++;
  //printf("size:%d\n", d->curbucketSize);
  //expansion
  if(d->numItems >= d->curbucketSize*0.75 
                                && d->curbucketSize <= 1048576){
    //printf("chekc");
    d->curbucketSize += UNIT_BUCKET_SIZE;
    struct UserInfo **past_idhash = d->idhash;
    struct UserInfo **past_namehash = d->namehash;
    struct UserInfo **new_idht;
    new_idht = (struct UserInfo **)calloc(d->curbucketSize,
                                        sizeof(struct UserInfo *));
    struct UserInfo **new_nameht; 
    new_nameht = (struct UserInfo **)calloc(d->curbucketSize,
                                        sizeof(struct UserInfo *));
    if (new_idht == NULL || new_nameht == NULL ) {
      fprintf(stderr, "Can't allocate a memory for array of size %d\n",
        d->curbucketSize);
      return (-1);
    }
    d->idhash = new_idht;
    d->namehash = new_nameht;
    d->numItems = 0;
    struct UserInfo *entry; //for iterate
    struct UserInfo *entry_next; //for iterate
    for(int i=0; i < d->curbucketSize-UNIT_BUCKET_SIZE; i++){
      entry = past_idhash[i];
      while(entry){
        RegisterCustomer(d, entry->id, entry->name, entry->purchase);
        //free entry
        free(entry->name);
        free(entry->id);
        entry_next = entry->next_id;
        free(entry);
        entry = entry_next;
      }
    }
    free(past_idhash);
    free(past_namehash);
  }
  return 0;
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /*d의 idhash, nameahsh 에서 해당 hashkey를 이용하여 id와 일치한 
  사용자를 찾아 그 사용자의 정보공간을 해제하고 node간 연결 상태를 수정.
  성공하면 0을 반환하고 d 나 id가 NULL 인경우, 
  사용자에 대한 정보가 없는 경우 -1을 반환*/
  if(d == NULL || id == NULL)
    return (-1);
  int key = hash_function(id, d->curbucketSize);
  struct UserInfo* cur = d->idhash[key];
  struct UserInfo* next;
  struct UserInfo* prev;
  while(cur){
    if(strcmp(cur->id, id)==0){
      //unregister id table
      next = cur->next_id;
      prev = cur->prev_id;
      if(next == NULL){
        if(prev == NULL)
          d->idhash[key] = NULL;
        else
          prev->next_id = NULL;
      }
      else{
        if(prev == NULL){
          next->prev_id = NULL;
          d->idhash[key] = next;
        }
        else{
          next->prev_id = prev;
          prev->next_id = next;
        }
      }
      //unregister name table
      key = hash_function(cur->name, UNIT_BUCKET_SIZE);
      key += d->curbucketSize - UNIT_BUCKET_SIZE;
      next = cur->next_name;
      prev = cur->prev_name;
      if(next == NULL){
        if(prev == NULL)
          d->namehash[key] = NULL;
        else
          prev->next_name = NULL;
      }
      else{
        if(prev == NULL){
          next->prev_name = prev;
          d->namehash[key] = next;
        }
        else{
          next->prev_name = prev;
          prev->next_name = next;
        }
      }
      //free
      free(cur);
      d->numItems--;
      return 0;
    }
    cur = cur->next_id;
  }
  return (-1);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /*d의 idhash, nameahsh 에서 해당 hashkey를 이용하여 name과 일치한 
  사용자를 찾아 그 사용자의 정보공간을 해제하고 node간 연결 상태를 수정.
  성공하면 0을 반환하고 d 나 name이 NULL 인경우, 
  사용자에 대한 정보가 없는 경우 -1을 반환*/
  if(d == NULL || name == NULL)
    return (-1);
  int key = hash_function(name, d->curbucketSize);
  struct UserInfo* cur = d->namehash[key];
  struct UserInfo* next;
  struct UserInfo* prev;
  while(cur){
    if(strcmp(cur->name, name)==0){
      //unregister name table
      next = cur->next_name;
      prev = cur->prev_name;
      if(next == NULL){
        if(prev == NULL){
          d->namehash[key] = NULL;
        }
        else{
          prev->next_name = NULL;
        }
      }
      else{
        if(prev == NULL){
          next->prev_name = NULL;
          d->namehash[key] = next;
        }
        else{
          next->prev_name = prev;
          prev->next_name = next;
        }
      }
      //unregister name table
      key = hash_function(cur->id, UNIT_BUCKET_SIZE);
      key += d->curbucketSize - UNIT_BUCKET_SIZE;
      next = cur->next_id;
      prev = cur->prev_id;
      if(next == NULL){
        if(prev == NULL)
          d->idhash[key] = NULL;
        else
          prev->next_id = NULL;
      }
      else{
        if(prev == NULL){
          next->prev_id = prev;
          d->idhash[key] = next;
        }
        else{
          next->prev_id = prev;
          prev->next_id = next;
        }
      }
      //free
      free(cur);
      d->numItems--;
      return 0;
    }
    cur = cur->next_name;
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  /*d의 name hash table인 namehash 에서 parameter name에 대한 hash key
  를 이용하여 사용자를 찾아 그 사용자의 purchase를 반환한다.
  사용자에 대한 정보가 없는 경우 -1을 반환한다.*/
  if(d == NULL || id == NULL)
    return (-1);
  int key = hash_function(id, d->curbucketSize);
  struct UserInfo* cur = d->idhash[key];
  while(cur){
    if(strcmp(cur->id, id)==0)
      return cur->purchase;
    cur = cur->next_id;
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  /*d의 name hash table인 namehash 에서 parameter name에 대한 hash key
  를 이용하여 사용자를 찾아 그 사용자의 purchase를 반환한다.
  사용자에 대한 정보가 없는 경우 -1을 반환*/
  if(d == NULL || name == NULL)
    return (-1);
  int key = hash_function(name, d->curbucketSize);
  struct UserInfo* cur = d->namehash[key];
  while(cur){
    if(strcmp(cur->name, name)==0)
      return cur->purchase;
    cur = cur->next_name;
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /*d 안에 들어있는 모든 사용자에 대하여 사용자 정보(id, name, purchase)를
  입력으로 하는 함수fp 의 반환값을 모두 더한값을 반환한다.
  d나 fp 가 NULL 이면 -1을 반환한다.*/
  if(d == NULL || fp == NULL)
    return (-1);
  int sum = 0;
  int result = 0;
  struct UserInfo *cur;
  for(int i = 0; i < d->curbucketSize; i++){
    if(d->idhash[i] != NULL){
      cur = d->idhash[i];
      while(cur){
        result = fp(cur->id, cur->name, cur->purchase);
        sum += result;
        cur = cur->next_id;
      }
    }
  }
  return sum;
}