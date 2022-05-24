#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  char *p, *sval, *tmp = out;
  int ival;

  va_start(ap, fmt);
  for(p = (char *)fmt; *p; p++){
    if(*p != '%'){
      *(out++) = *p;
      continue;
    }
    switch(*(++p)){
      case 'd':
        ival = va_arg(ap, int);
        *(out++) = ival;
        break;
      case 's':
        for(sval = va_arg(ap, char*); *sval; sval++)
          *(out++) = *sval;
        break;
      default: putch(*p);
        break;
    }
  }
  va_end(ap);
  return out - tmp;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
