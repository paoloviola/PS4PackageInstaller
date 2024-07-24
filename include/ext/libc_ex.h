
#ifndef LIBC_EX_H
#define LIBC_EX_H

#define offsetof(st, m) \
    ((size_t)&(((st *)0)->m))

extern long long int (*strtoll)(const char* str, char** endptr, int base);
extern double (*strtod)(const char* str, char** endptr);
extern int (*isxdigit)(int arg);

void initLibcEx(void);

#endif
