#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char* append_char_dynamic(char* str, char ch) {
  int len = strlen(str);
  char* new_str = realloc(str, len + 2);
  if (!new_str) return str;
  new_str[len] = ch;
  new_str[len + 1] = '\0';
  return new_str;
}

char* append_str_dynamic(char* str, char* ch) {
  size_t lenStr = strlen(str);
  size_t lenCh = strlen(ch);

  char* new_str = realloc(str, lenStr + lenCh + 1);
  if (!new_str) return str;

  memcpy(new_str + lenStr, ch, lenCh + 1);
  return new_str;
}

char* toLowerCase(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }

  return str;
}

char* toLowerCopy(const char *str) {
  char *copy = strdup(str);
  for (int i = 0; copy[i]; i++)
    copy[i] = tolower(copy[i]);
  return copy;
}

int exists(const char *fname) {
  FILE *file;
  if ((file = fopen(fname, "r")))
  {
    fclose(file);
    return 1;
  }
  return 0;
}

char* getRandomFace(void) {
  const char* faces[] = {
    " rawr x3",
    " OwO",
    " UwU",
    " o.O",
    " -.-",
    " >w<",
    " :3",
    " :3",
    " XD",
    " nyaa~~",
    " mya",
    " >_<",
    " rawr",
    " ^^",
  };

  int count = sizeof(faces) / sizeof(faces[0]);
  int r = rand() % count;

  return (char*)faces[r];
}

char* uwuifyString(char *message) {
  char *uwuifyMessage = strdup("");
  const char *delimiters = " ";

  char *token = strtok(message, delimiters);
  int wordCount = 0;
  int charCount = 0;
  int priorCharIsN = 0;
  int r = 0;
  
  while (token != NULL) {
    // UwU -> UwU~
    char *lower = toLowerCopy(token);

    if (strcmp(lower, "uwu") == 0) {
      uwuifyMessage = append_str_dynamic(uwuifyMessage, "UwU~ ");
      free(lower);
      token = strtok(NULL, delimiters);
      continue;
    }

    // I -> i
    if (strcmp(lower, "I") == 0) {
      uwuifyMessage = append_str_dynamic(uwuifyMessage, "i ");
      free(lower);
      token = strtok(NULL, delimiters);
      continue;
    }

    for (char *p = token; *p != '\0'; p++) {
      char appendChar;
      int appendFace = 0;

      /*
      l, r  -> w
      L, R  -> W
      na    -> nya
      */
      switch (*p) {
        case '.':
        case '!':
        case '?':
          appendChar = *p;
          appendFace = 1;
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
      
      r = rand() % 3;
      if (charCount == 0 && r == 0) {
        char tmp[3] = { appendChar, '-', '\0' };
        uwuifyMessage = append_str_dynamic(uwuifyMessage, tmp);
      }

      if (appendChar != ' ')
        uwuifyMessage = append_char_dynamic(uwuifyMessage, appendChar);
      
      if (appendFace == 1)
        uwuifyMessage = append_str_dynamic(uwuifyMessage, getRandomFace());
      
      charCount++;
    }

    free(lower);

    printf("| Word %d: %s\n", ++wordCount, token);
    token = strtok(NULL, delimiters);
    charCount = 0;

    uwuifyMessage = append_char_dynamic(uwuifyMessage, ' ');
  }

  return uwuifyMessage;
}

int main (int argc, char *argv[]) {
  printf("Uwuify C\n");

  // srand(time(NULL));
  srand(1);

  if (argc != 2) {
    printf("Invalid args\n");
    printf("\tuwuify [\"message\" or <filepath>]");
    return 0;
  }

  char *uwuifyMessage;

  if(exists(argv[1])) {
    FILE *file = fopen(argv[1], "r");
    char buffer[256];

    if (file == NULL) {
      perror("Unable to open file!");
      return 1;
    }

    char outname[512];
    snprintf(outname, sizeof(outname), "%s.uwu", argv[1]);

    FILE *uwuFile = fopen(outname, "w");

    while (fgets(buffer, sizeof(buffer), file)) {
      buffer[strcspn(buffer, "\n")] = '\0';

      char *tmp = strdup(buffer);
      char *uwu = uwuifyString(tmp);

      printf("%s\n", uwu);
      fputs(uwu, uwuFile);
      fputc('\n', uwuFile);

      free(tmp);
      free(uwu);
    }

    uwuifyMessage = "Done!";

    fclose(file);
  } else {
    char *message = argv[1];
    uwuifyMessage = uwuifyString(message);
  }

  printf("Uwuify: %s", uwuifyMessage);

  if (!exists(argv[1])) {
    free(uwuifyMessage);
  }

  return 0;
}