/*base64.c*/  
#ifndef _BASE64_H  
#define _BASE64_H  
  
#include <stdlib.h>  
#include <string.h>  
unsigned char *utils_base64_encode(char *buf, unsigned char *str);  
unsigned char *utils_bae64_decode(char *code);  
#endif  
  