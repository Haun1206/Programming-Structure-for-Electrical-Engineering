// 김하운, Assignment 2, str.c
#include <assert.h> /* to use assert() */
#include <stdio.h>
#include "str.h"

/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*------------------------------------------------------------------------*/
size_t StrGetLength(const char* pcSrc)
{
  //pcSrc 의 길이를 반환한다.
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

/*------------------------------------------------------------------------*/
char *StrCopy(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  // pcDest에 pcSrc를 복사하여 입력받았던 처음 pcDest 주소를 반환한다.
  assert(pcDest);
  assert(pcSrc);
  char *start = pcDest;
  while(*pcSrc){
    *pcDest = *pcSrc;
    pcDest++;
    pcSrc++;
  }
  *pcDest = '\0';
  
  return start;
}

/*------------------------------------------------------------------------*/
int StrCompare(const char* pcS1, const char* pcS2)
{
  /* TODO: fill this function */
  //pcS1의 참조 값과 pcS2의참조 값을 앞에서 부터 차례데로 비교하여
  //pcS1이 크면 1, 같으면 0, 작으면 1을 반환한다.
  assert(pcS1);
  assert(pcS2);

  while(*pcS1){
    if(*pcS1 > *pcS2)
      return 1;
    else if(*pcS1 < *pcS2)
      return -1;
    else{
      pcS1++;
      pcS2++;
    }
  }

  if(*pcS2 == '\0')
    return 0;
  else
    return -1;

}
/*------------------------------------------------------------------------*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle)
{
  /* TODO: fill this function */
  //pcNeedle 이 pcHaystack에서 처음 발견된 주소를 반환한다
  //발견되지 않으면 NULL 반환
  assert(pcHaystack);
  assert(pcNeedle);
  const char *start;
  const char *pcEnd = pcNeedle;
  int is_find = 1;

  if(*pcNeedle == '\0')
    return (char *)pcHaystack;
  
  while(*pcHaystack){
    if(*pcHaystack == *pcNeedle){
      start = pcHaystack;
      //같은 시점으로부터 pcNeedle 만큼 비교시작
      while(*pcEnd){
	if(*pcHaystack != *pcEnd){
	  is_find = 0;
	  break;
	}
	pcHaystack++;
	pcEnd++;
      }
      //위에서 찾았다면 시작 포인터 반환, 실패시 시작포인터로 돌아감
      if(is_find)
	return (char *)start;
      else{
	pcHaystack = start;
	pcEnd = pcNeedle;
      }
    }
    pcHaystack++;
  }
  
  return (char *)NULL;
}
/*------------------------------------------------------------------------*/
char *StrConcat(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  //pcDest 뒤에 pcSrc 를 이어 붙이고 입력 받은 첫 pcDest 반환한다.
  char *start = pcDest;
  while(*pcDest)
    pcDest++;
  while(*pcSrc){
    *pcDest = *pcSrc;
    pcDest++;
    pcSrc++;
  }
  *pcDest = '\0';
  return start;
}
