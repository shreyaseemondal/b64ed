#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define usage() do{ \
  fprintf(stderr, "usage: \nb64ed [-e|-d] <string>\nb64ed <string> (assumes encoding mode)\n"); \
  exit(EXIT_FAILURE); \
} while(0)


extern char *base64_encode(const unsigned char *input, const size_t length);
extern char *base64_decode(const unsigned char *input, const size_t length);

int
main(int argc, char *argv[]){
  char * (*fptr)(const unsigned char *, const size_t);
  fptr = base64_encode;

  char *input;
  if(argc == 2)
    input = argv[1];
  else if (argc == 3){
    if(strcmp(argv[1], "-d") == 0)
      fptr = base64_decode;
    input = argv[2];
  } else {
    usage();
  }

  char *output = fptr((const unsigned char *)input, strlen(input));
  printf("%s\n", output);
  free(output);
}
