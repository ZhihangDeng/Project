#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  while((*dst++ = *src++) != '\0');
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  char *tmp = dst;
  while(*dst)
    dst++;
  while((*dst++ = *src++)!='\0');
  return tmp;
}

int strcmp(const char *s1, const char *s2) {
  while ((*s1) && (*s1 == *s2))  
  {  
      s1++;  
      s2++;  
  }  
  if (*(unsigned char*)s1 > *(unsigned char*)s2)  
    return 1;  
  else if (*(unsigned char*)s1 < *(unsigned char*)s2)   
    return -1;  
  else  
    return 0;  
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  for(size_t i = 0; i < n; i++){
    *(char*)(s+i) = (char)c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  for(size_t i = 0; i < n; i++){
    *(char*)(out+i) = *(char*)(in+i);
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  for(size_t i = 0; i < n; i++){
    if(*(unsigned char*)(s1+i)-*(unsigned char*)(s2+i) > 0)
      return 1;
    else if(*(unsigned char*)(s1+i)-*(unsigned char*)(s2+i) < 0)
      return -1;
  }
  return 0;
}

#endif
