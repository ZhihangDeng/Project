#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int _i2a(int num, char buf[32], int radix) {
	static const char s[] = "0123456789abcdef";
	int n = num, R = radix;
	char *dst = buf;
	if (n < 0) { *dst++ = '-'; n = -n; }
	if (n < 10)
	{
		*dst++ = s[n]; *dst = 0;
	}else
	{
		char tmp[32], *p = tmp;
		while (n) { *p++ = s[n % R]; n /= R; }
		while (--p != tmp) *dst++ = *p;
		*dst++ = *tmp; *dst = 0;
	}
	return dst-buf;
}

int printf(const char *fmt, ...) {
  va_list ap;
  char *p, *sval, out[128], *tmp = out;
  int ival;
  
  va_start(ap, fmt);
  for(p = (char *)fmt; *p; p++){
    int n;
    if(*p != '%'){
      *(tmp++) = *p;
      continue;
    }
    switch(*(++p)){
      case 'd':
        ival = va_arg(ap, int);
        n = _i2a(ival, tmp, 10);
        tmp += n;
        break;
      case 's':
        for(sval = va_arg(ap, char*); *sval; sval++)
          *(tmp++) = *sval;
        break;
      case 'c':
        ival = va_arg(ap, int);
        *(tmp++) = (char)ival;
      default: *(tmp++) = *p;
        break;
    }
  }
  *tmp = '\0';
  va_end(ap); 
  putstr(out);
  return tmp - (char *)out;
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
    int n;
    if(*p != '%'){
      *(out++) = *p;
      continue;
    }
    switch(*(++p)){
      case 'd':
        ival = va_arg(ap, int);
        n = _i2a(ival, out, 10);
        out += n;
        break;
      case 's':
        for(sval = va_arg(ap, char*); *sval; sval++)
          *(out++) = *sval;
        break;
      default: *(out++) = *p;
        break;
    }
  }
  *out = '\0';
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
