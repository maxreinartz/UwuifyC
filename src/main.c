#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int totalWords = 0;

static unsigned int seed = 1;

typedef struct {
  char *data;
  size_t length;
  size_t capacity;
} Output;

typedef struct {
  const char *word;
  const char *replacement;
} WordReplacement;

static const WordReplacement replacements[] = {
    {"uwu", "UwU~"},    {"small", "smol"}, {"cute", "kawaii~"},
    {"fluff", "floof"}, {"love", "wuv"},   {"stupid", "baka"},
    {"meow", "nya~"},   {"i", "i"},
};

static inline int fastRandom(void) {
  seed = (214013 * seed + 2531011);
  return (seed >> 16) & 0x7FFF;
}

static char lowerAscii(char c) {
  return c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c;
}

static const size_t replacementCount =
    sizeof(replacements) / sizeof(*replacements);

int ensureCapacity(Output *output, size_t amount) {
  size_t required = output->length + amount + 1;

  if (required <= output->capacity)
    return 1;

  size_t capacity = output->capacity ? output->capacity : 64;
  while (capacity < required)
    capacity *= 2;

  char *data = realloc(output->data, capacity);
  if (!data)
    return 0;

  output->data = data;
  output->capacity = capacity;
  return 1;
}

static inline void appendCharacter(Output *output, char character) {
  if (ensureCapacity(output, 1)) {
    output->data[output->length++] = character;
    output->data[output->length] = '\0';
  }
}

static inline void appendString(Output *output, const char *string) {
  size_t length = strlen(string);

  if (ensureCapacity(output, length)) {
    memcpy(output->data + output->length, string, length + 1);
    output->length += length;
  }
}

const char *findReplacement(const char *token, const char **suffix) {
  for (size_t i = 0; i < replacementCount; i++) {
    const char *word = replacements[i].word;

    if (lowerAscii(token[0]) != lowerAscii(word[0]))
      continue;

    const char *part = token;

    while (*word && *part && lowerAscii(*word) == lowerAscii(*part)) {
      word++;
      part++;
    }

    if (!*word && (!*part || !isalnum((unsigned char)*part))) {
      *suffix = part;
      return replacements[i].replacement;
    }
  }

  return NULL;
}

char *getRandomFace(void) {
  const char *faces[] = {
      " rawr x3", " OwO", " UwU",    " o.O", " -.-", " >w<",  " :3",
      " :3",      " XD",  " nyaa~~", " mya", " >_<", " rawr", " ^^",
  };

  int count = sizeof(faces) / sizeof(faces[0]);
  int r = fastRandom() % count;

  return (char *)faces[r];
}

char *uwuifyString(char *message) {
  size_t inputLength = strlen(message);

  Output output = {.data = malloc(inputLength + 64),
                   .length = 0,
                   .capacity = inputLength + 64};

  if (!output.data)
    return NULL;

  output.data[0] = '\0';

  char *token = message;
  // int wordCount = 0;
  int charCount = 0;
  int priorCharIsN = 0;
  int r = 0;

  while (*token) {
    if (*token == ' ') {
      appendCharacter(&output, *token++);
      continue;
    }

    char *end = token;
    while (*end && *end != ' ')
      end++;

    char saved = *end;
    *end = '\0';

    const char *suffix;
    const char *replacement = findReplacement(token, &suffix);

    if (replacement) {
      appendString(&output, replacement);
      while (*suffix)
        appendCharacter(&output, *suffix++);
      *end = saved;
      token = end;
      totalWords++;
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
          appendCharacter(&output, 'y');
        appendChar = *p;
        priorCharIsN = 0;
        break;
      case 'A':
        if (priorCharIsN == 1)
          appendCharacter(&output, 'Y');
        appendChar = *p;
        priorCharIsN = 0;
        break;
      default:
        appendChar = *p;
        priorCharIsN = 0;
        break;
      }

      int isAsciiLetter = (appendChar >= 'A' && appendChar <= 'Z') ||
                          (appendChar >= 'a' && appendChar <= 'z');

      int r = fastRandom() % 3;
      if (charCount == 0 && r == 0 && isAsciiLetter) {
        appendCharacter(&output, appendChar);
        appendCharacter(&output, '-');
      }

      if (appendChar != ' ')
        appendCharacter(&output, appendChar);

      if (appendFace == 1)
        appendString(&output, getRandomFace());

      charCount++;
    }

    // printf("| Word %d: %s\n", ++wordCount, token);
    *end = saved;
    token = end;
    charCount = 0;
    priorCharIsN = 0;
    totalWords++;
  }

  return output.data;
}

int main(int argc, char *argv[]) {
  struct timeval t1, t2;
  double elapsedTime;

  gettimeofday(&t1, NULL);

  printf("Uwuify C\n");

  if (argc != 2) {
    printf("Invalid args\n");
    printf("\tuwuify [\"message\" or <filepath>]");
    return 0;
  }

  char *uwuifyMessage;

  FILE *file = fopen(argv[1], "r");

  if (file) {
    char *buffer = NULL;
    size_t bufferCapacity = 0;

    char outname[512];
    snprintf(outname, sizeof(outname), "%s.uwu", argv[1]);

    FILE *uwuFile = fopen(outname, "w");
    setvbuf(uwuFile, NULL, _IOFBF, 64 * 1024);

    while (getline(&buffer, &bufferCapacity, file) != -1) {
      buffer[strcspn(buffer, "\n")] = '\0';

      char *uwu = uwuifyString(buffer);

      // printf("%s\n", uwu);
      fputs(uwu, uwuFile);
      fputc('\n', uwuFile);

      free(uwu);
    }

    uwuifyMessage = "Done!";

    free(buffer);
    fclose(file);
  } else {
    char *message = argv[1];
    uwuifyMessage = uwuifyString(message);
  }

  gettimeofday(&t2, NULL);

  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;

  printf("\n-----------------\n      Stats\n-----------------\nTime: %.2f "
         "ms\nTotal Words: %i",
         elapsedTime, totalWords);

  printf("\n\n-----------------\n      Uwuify\n-----------------\n%s\n",
         uwuifyMessage);

  if (!file) {
    free(uwuifyMessage);
  }

  return 0;
}