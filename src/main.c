#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>

char* append_char_dynamic(char* str, char ch) {
  int len = strlen(str);
  char* new_str = realloc(str, len + 2);
  if (!new_str) return str;
  new_str[len] = ch;
  new_str[len + 1] = '\0';
  return new_str;
}

int main (int argc, char *argv[]) {
  printf("Uwuify C\n");

  if (argc != 2) {
    printf("Invalid args\n");
    printf("\tuwuify \"message\"");
    return 0;
  }

  char *message = argv[1];
  char *uwuifyMessage = strdup("");
  const char *delimiters = " ";

  char *token = strtok(message, delimiters);
  int wordCount = 0;
  int priorCharIsN = 0;
  
  while (token != NULL) {
    for (char *p = token; *p != '\0'; p++) {
      char appendChar;

      switch (*p) {
        case 'I':
          appendChar = 'i';
          break;
        case 'l':
        case 'r':
          appendChar = 'w';
          break;
        case 'L':
        case 'R':
          appendChar = 'W';
          break;
        case 'n':
        case 'N':
          appendChar = *p;
          priorCharIsN = 1;
          break;
        case 'a':
          if (priorCharIsN == 1)
            uwuifyMessage = append_char_dynamic(uwuifyMessage, 'y');
          appendChar = *p;
          priorCharIsN = 0;
          break;
        case 'A':
          if (priorCharIsN == 1)
            uwuifyMessage = append_char_dynamic(uwuifyMessage, 'Y');
          appendChar = *p;
          priorCharIsN = 0;
          break;
        default:
          appendChar = *p;
          priorCharIsN = 0;
          break;
      }

      uwuifyMessage = append_char_dynamic(uwuifyMessage, appendChar);
    }

    printf("| Word %d: %s\n", ++wordCount, token);
    token = strtok(NULL, delimiters);

    uwuifyMessage = append_char_dynamic(uwuifyMessage, ' ');
  }

  printf("Uwuify: %s", uwuifyMessage);

  free(uwuifyMessage);

  return 0;
}