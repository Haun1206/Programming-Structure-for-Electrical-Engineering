//김하운, Assignment 2, sgrep.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define FIND_STR        "-f"
#define REPLACE_STR     "-r"
#define DIFF_STR        "-d"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

typedef enum {
  INVALID,
  FIND,
  REPLACE,
  DIFF
} CommandType;

/*
 * Fill out your functions here (If you need) 
 */

/*--------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                     */
/*--------------------------------------------------------------------*/
void 
PrintUsage(const char* argv0) 
{
  const static char *fmt = 
    "Simple Grep (sgrep) Usage:\n"
    "%s [COMMAND] [OPTIONS]...\n"
    "\nCOMMNAD\n"
    "\tFind: -f [search-string]\n"
    "\tReplace: -r [string1] [string2]\n"
    "\tDiff: -d [file1] [file2]\n";

  printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* DoFind()
   Your task:
   1. Do argument validation 
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long, 
   print out "Error: argument is too long"
   
   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes, 
   print out "Error: input line is too long" 
   - Error message should be printed out to standard error (stderr)
   
   3. Check & print out the line contains a given string (search-string)
      
   Tips:
   - fgets() is an useful function to read characters from file. Note 
   that the fget() reads until newline or the end-of-file is reached. 
   - fprintf(sderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoFind(const char *pcSearch)
{
  // pcSearch 문자를 입력 스트림 stdin 에서 한줄씩검색을 한다.
  // 발견되는 경우 해당 줄을 출력하고 TRUE를 리턴한다.
  // 발견되지 않거나 문제가 발생하는 경우는 FALSE를 리턴한다.
  // 또한 문제 발생시 stderr로 에러를 출력한다.
  char buf[MAX_STR_LEN + 2]; 
  int len;
  /* 
   *  Fill out your variables here 
   */

  if((len = StrGetLength(pcSearch)) > MAX_STR_LEN){
    fprintf(stderr, "Error: argument is too long\n");
    return FALSE;
  }
  /* Read the line by line from stdin, Note that this is an example */
  while (fgets(buf, sizeof(buf), stdin)) {
    /* check input line length */
    if ((len = StrGetLength(buf)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
    /* TODO: fill out this function */
    if(StrSearch(buf, pcSearch))
      printf("%s", buf);
      
  }
   
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoReplace()
   Your task:
   1. Do argument validation 
      - String length is no more than 1023
      - If you encounter a command-line argument that's too long, 
        print out "Error: argument is too long"
      - If word1 is an empty string,
        print out "Error: Can't replace an empty substring"
      
   2. Read the each line from standard input (stdin)
      - If you encounter a line larger than 1023 bytes, 
        print out "Error: input line is too long" 
      - Error message should be printed out to standard error (stderr)

   3. Replace the string and print out the replaced string

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoReplace(const char *pcString1, const char *pcString2)
{
  // 입력 스트림(stdin) 에서 한줄씩 검사하여
  // 모든 pcString1을 pcString2로 바꾼다.
  // 없으면 바꾸지 않고 그렇게 바꾼 결과를 한줄마다
  // 출력 스트림(stdout)에 출력한다.
  // 문제가 발생하면FALSE를 반환하고stderr로 에러를 출력한다.
  // 성공시 TRUE를 반환한다.
  /* TODO: fill out this function */
  char buf[MAX_STR_LEN + 2];
  char *check;
  char copy[MAX_STR_LEN +2];
  int len;
  int num = 0;
  char *location;
  char *next_start;

  // argument 가 너무 긴지 체크
  if ((len = StrGetLength(pcString1)) > MAX_STR_LEN){
    fprintf(stderr, "Error: argument is too long\n");
    return FALSE;
  }
  if ((len = StrGetLength(pcString2)) > MAX_STR_LEN){
    fprintf(stderr, "Error: argument is too long\n");
    return FALSE;
  }  
  // word1이 empty인지 체크
  if(( len = StrGetLength(pcString1) ) == 0){
    fprintf(stderr, "Error: Can't replace an empty substring");
    return FALSE;
  }
  
  while (fgets(buf, sizeof(buf), stdin)) {
    // line 너무 긴지 체크
    if ((len = StrGetLength(buf)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
    //line에 몇개의 pcString1 이 있는지 세기
    check = buf;
    len = StrGetLength(pcString1);
    while((check = StrSearch(check, pcString1)) != NULL){
      num++;
      check += len;
    }
    // replacment 후에도 line 이  너무 긴지 체크
    if((len = (StrGetLength(buf) -
	       num*(StrGetLength(pcString1)-
		    StrGetLength(pcString2)))) > MAX_STR_LEN){
      fprintf(stderr, "Error:argument is too long\n");
      return FALSE;
    }
    
    
    while((location = StrSearch(buf, pcString1))){
      next_start = location + StrGetLength(pcString1);
      StrCopy(copy, next_start);
      *location = '\0';
      StrConcat(buf, pcString2);
      StrConcat(buf, copy);
    }
    
    fprintf(stdout, "%s", buf);
    
  }
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoDiff()
   Your task:
   1. Do argument validation 
     - file name length is no more than 1023
     - If a command-line argument is too long, 
       print out "Error: argument is too long" to stderr

   2. Open the two files
      - The name of files are given by two parameters
      - If you fail to open either file, print out error messsage
      - Error message: "Error: failed to open file [filename]\n"
      - Error message should be printed out to stderr

   3. Read the each line from each file
      - If you encounter a line larger than 1023 bytes, 
        print out "Error: input line [filename] is too long" 
      - Error message should be printed out to stderr

   4. Compare the two files (file1, file2) line by line 

   5. Print out any different line with the following format
      file1@linenumber:file1's line
      file2@linenumber:file2's line

   6. If one of the files ends earlier than the other, print out an
      error message "Error: [filename] ends early at line XX", where
      XX is the final line number of [filename].

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoDiff(const char *file1, const char *file2)
{
  // file1 file2 를 열어서 한줄마다 비교하여 다른 줄을 출력한다.
  // 문제 발생시 stderr로 에러를 출력하고 FALSE를 반환한다.
  // 성공시 TRUE를 반환한다.
  /* TODO: fill out this function */  
  int len;
  int lines1 = 0;
  int lines2 = 0;
  char buf1[MAX_STR_LEN+2];
  char buf2[MAX_STR_LEN+2];
  FILE *f1;
  FILE *f2;  
  // file name 길이가 너무 긴지 체크
  if ((len = StrGetLength(file1)) > MAX_STR_LEN){
    fprintf(stderr, "Error: argument is too long\n");
    return FALSE;
  }
  if ((len = StrGetLength(file2)) > MAX_STR_LEN){
    fprintf(stderr, "Error: argument is too long\n");
    return FALSE;
  }
  
  //file1 이 open이 잘 되었는지 체크
  if((f1 = fopen(file1, "r")) == NULL){
    fprintf(stderr, "Error: failed to open file %s\n", file1);
    return FALSE;
  }
  //file2 가 open이 잘 되었는지 체크
  if((f2 = fopen(file2, "r")) == NULL){
    fprintf(stderr, "Error: failed to open file %s\n", file2);
    return FALSE;
  }  
  while (fgets(buf1, sizeof(buf1), f1)){
    //file2가 먼저 끝나는 경우
    if(fgets(buf2, sizeof(buf2), f2) == NULL){
      fprintf(stderr,
	      "Error: %s ends early at line %d", file2, lines2);
      fclose(f1);
      fclose(f2);
      return FALSE;
    }
    //file1의 line 크기가 너무 큰지 체크
    if ((len = StrGetLength(buf1)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line %s is too long", file1);
      fclose(f1);
      fclose(f2);
      return FALSE;
    }
    //file2의 line 크기가 너무 큰지 체크  
    if ((len = StrGetLength(buf2)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line %s is too long", file2);
      fclose(f1);
      fclose(f2);
      return FALSE;
    }
    // line 수 count
    lines1++;
    lines2++;
    // 각 line 비교
    if(StrCompare(buf1, buf2) != 0){
      printf("%s@%d:%s", file1, lines1, buf1);
      printf("%s@%d:%s", file2, lines2, buf2);
    }
  }
  //file1 이 file2 보다 먼저 끝났는지 확인
  if(fgets(buf2, sizeof(buf2), f2) != NULL){
    fprintf(stderr,
	    "Error: %s ends early at line %d", file1, lines1);
    fclose(f1);
    fclose(f2);
    return FALSE;
  }
  
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* CommandCheck() 
   - Parse the command and check number of argument. 
   - It returns the command type number
   - This function only checks number of argument. 
   - If the unknown function is given or the number of argument is 
   different from required number, this function returns FALSE.
   
   Note: You SHOULD check the argument rule later                    */
/*-------------------------------------------------------------------*/ 
int
CommandCheck(const int argc, const char *argv1)
{
  int cmdtype = INVALID;
   
  /* check minimum number of argument */
  if (argc < 3)
    return cmdtype;
   
  /* check command type */ 
  if (strcmp(argv1, FIND_STR) == 0) {
    if (argc != 3)
      return FALSE;    
    cmdtype = FIND;       
  }
  else if (strcmp(argv1, REPLACE_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = REPLACE;
  }
  else if (strcmp(argv1, DIFF_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = DIFF;
  }
   
  return cmdtype;
}
/*-------------------------------------------------------------------*/
int 
main(const int argc, const char *argv[]) 
{
  int type, ret;
   
  /* Do argument check and parsing */
  if (!(type = CommandCheck(argc, argv[1]))) {
    fprintf(stderr, "Error: argument parsing error\n");
    PrintUsage(argv[0]);
    return (EXIT_FAILURE);
  }
   
  /* Do appropriate job */
  switch (type) {
  case FIND:
    ret = DoFind(argv[2]);
    break;
  case REPLACE:
    ret = DoReplace(argv[2], argv[3]);
    break;
  case DIFF:
    ret = DoDiff(argv[2], argv[3]);
    break;
  } 

  return (ret)? EXIT_SUCCESS : EXIT_FAILURE;
}
