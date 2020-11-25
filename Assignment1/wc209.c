#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

enum DFAState{ IN, OUT, START_C, IN_C, FINISH_C };
int main() {
  //변수 선언
  int c = 0;
  int lines = 0, words = 0, chars = 0;
  int start_line;
  enum DFAState state = OUT;
  enum DFAState pre_state = OUT;
  //DFA
  while((c = getchar()) != EOF){
    if(c=='\n')lines++;
    switch (state){
    case IN:// 글자 안
      if (c == '/') {
	state = START_C;
	pre_state = IN;
      }
      else if (isspace(c)) state = OUT;
      chars ++;
      break;
    case OUT:// 글자 밖
      if(!isspace (c)) {
	if(c == '/'){
	  state = START_C;
	  pre_state = OUT;
	  chars++;
	  break;
	}
	state = IN;
	words++;
      }
      chars++;
      break;
    case START_C:// 주석 시작 표시 확인하기
      // 주석 시작이 맞을때
      if(c == '*') {
	state = IN_C;
	start_line = lines;
      }
      //주석 시작이 아닐때
      else {
	if(pre_state == OUT) words++;
	if(c == '/') {
	  state = START_C;
	  pre_state = START_C;
	}
	else state = IN;
	chars++;
      }
      break;
    case IN_C:// 주석 안
      if(c == '\n' || c == '\f' || c == '\r' || c == '\t' || c == '\v') chars++;
      if(c == '*') state = FINISH_C;
      break;
    case FINISH_C:// 주석 마침표시  확인하기
      //주석 마침이 맞을때
      if(c == '/') state = OUT;
      // 주석 마침이 아닐때
      else if(c == '*') state = FINISH_C;
      else if(c == '\n' || c == '\f' || c == '\r' || c == '\t' || c == '\v') chars++;
      else state = IN_C;
      break;
    default:
      assert(0);
      break;
    }
  }
  if(state == IN_C || state == FINISH_C){
    fprintf(stderr, "Error: line %d: unterminated comment\n", start_line+1);
    return EXIT_FAILURE;
  }
  else{
    if(chars != 0) lines++;
    printf("%d %d %d\n", lines, words, chars);
    return EXIT_SUCCESS;
  }

}
