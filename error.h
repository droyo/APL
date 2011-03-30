#ifndef ERROR_ENM_H
#define ERROR_ENM_H
#include "error-enm.h"
#endif

void*  enil (int(*f)(va_list),...);
int    eneg (int(*f)(va_list),...);
void*  ezil (int(*f)(va_list),...);
int    ezro (int(*f)(va_list),...);
void   edie (int(*f)(va_list),...);
