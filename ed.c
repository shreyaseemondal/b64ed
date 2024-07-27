#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *b64_table = 
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789"
  "+/";

char *
base64_encode(const unsigned char *input, size_t length){
  // we can "ceil" the result of an integer subtraction 
  // by adding the maximum remainder (divisor - 1) to the dividend
  size_t output_length = 4 * ((length + 2)/3);

  char *encoded = (char *)malloc(output_length + 1);
  if (!encoded){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  char *enc_ptr = encoded;
  for(size_t i = 0; i < length;){
    unsigned octet_a = (i < length)?input[i++]:0;
    unsigned octet_b = (i < length)?input[i++]:0;
    unsigned octet_c = (i < length)?input[i++]:0;

    // move octet_a 16 bits to the left
    // add the result of shifting octet_b 8 bits to the left
    // then simply add octet_c
    // this gives you the 24 bits 
    unsigned triple_octets = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

    // now, i'll isolate each of the four sixtets
    // shifting triple_octets and using 0x3F as a mask to only get the lower 6 bits
    for(int j = 3; j >= 0; j--){
      *(enc_ptr++) = b64_table[(triple_octets >> (j * 6)) & 0x3F];
    }
  }

  // adding the required padding
  size_t k = 0;
  while ((length + k) % 3 != 0) {
    encoded[output_length - 1 - k++] = '=';
  }

  encoded[output_length] = '\0';

  return encoded;
}

char *
base64_decode(const unsigned char *input, size_t length){
  int padding = 0;
  while(input[length - 1 - padding] == '=')
    padding++;

  size_t decoded_size = (length / 4) * 3 - padding;
  length -= padding;

  char *decoded = (char *)malloc(decoded_size + 1);
  if (!decoded){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  int j = 0;
  for(size_t i = 0; i < length;){
    unsigned sextet_a = (i < length)?strchr(b64_table, input[i++]) - b64_table:0;
    unsigned sextet_b = (i < length)?strchr(b64_table, input[i++]) - b64_table:0;
    unsigned sextet_c = (i < length)?strchr(b64_table, input[i++]) - b64_table:0;
    unsigned sextet_d = (i < length)?strchr(b64_table, input[i++]) - b64_table:0;

    unsigned triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

    if (j < decoded_size) decoded[j++] = (triple >> 2 * 8) & 0xFF;
    if (j < decoded_size) decoded[j++] = (triple >> 1 * 8) & 0xFF;
    if (j < decoded_size) decoded[j++] = triple & 0xFF;
  }

  decoded[decoded_size] = '\0';
  return decoded;
}
