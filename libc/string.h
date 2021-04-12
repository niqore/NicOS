#ifndef STRING_H
#define STRING_H

void strrev(unsigned char *str);

char* itoa(int value, char* buffer, int base);

int strlen(const char *str);

char lower_case(char c);

char upper_case(char c);

int strcmp(const char *p1, const char *p2);

char* format_number_decimals(char *s, int decimals);

#endif