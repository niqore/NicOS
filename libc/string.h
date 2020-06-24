#ifndef STRING_H
#define STRING_H

void strrev(unsigned char *str);

int itoa(int num, unsigned char* str, int len, int base);

int strlen(const char *str);

char lower_case(char c);

char upper_case(char c);

#endif