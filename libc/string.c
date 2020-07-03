#include "string.h"

/* Return the length of the null-terminated string STR.  Scan for
   the null terminator quickly by testing four bytes at a time.  */
int strlen(const char *str) {
	const char *char_ptr;
	const unsigned long int *longword_ptr;
	unsigned long int longword, himagic, lomagic;

	/* Handle the first few characters by reading one character at a time.
	Do this until CHAR_PTR is aligned on a longword boundary.  */
	for (char_ptr = str; ((unsigned long int) char_ptr
			& (sizeof (longword) - 1)) != 0;
			++char_ptr)
		if (*char_ptr == '\0')
      		return char_ptr - str;

  	/* All these elucidatory comments refer to 4-byte longwords,
    	but the theory applies equally well to 8-byte longwords.  */

  	longword_ptr = (unsigned long int *) char_ptr;

  	/* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
    	the "holes."  Note that there is a hole just to the left of
    	each byte, with an extra at the end:
    	bits:  01111110 11111110 11111110 11111111
    	bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
    	The 1-bits make sure that carries propagate to the next 0-bit.
    	The 0-bits provide holes for carries to fall into.  */
	himagic = 0x80808080L;
 	lomagic = 0x01010101L;
 	if (sizeof (longword) > 4) {
    	/* 64-bit version of the magic.  */
    	/* Do the shift in two steps to avoid a warning if long has 32 bits.  */
    	himagic = ((himagic << 16) << 16) | himagic;
    	lomagic = ((lomagic << 16) << 16) | lomagic;
	}
	if (sizeof (longword) > 8)
		return -1;

	/* Instead of the traditional loop which tests each character,
		we will test a longword at a time.  The tricky part is testing
		if *any of the four* bytes in the longword in question are zero.  */
	for (;;) {
		longword = *longword_ptr++;

		if (((longword - lomagic) & ~longword & himagic) != 0) {
			/* Which of the bytes was the zero?  If none of them were, it was
			a misfire; continue the search.  */

			const char *cp = (const char *) (longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
			if (cp[3] == 0)
				return cp - str + 3;
			if (sizeof (longword) > 4) {
				if (cp[4] == 0)
					return cp - str + 4;
				if (cp[5] == 0)
					return cp - str + 5;
				if (cp[6] == 0)
					return cp - str + 6;
				if (cp[7] == 0)
					return cp - str + 7;
			}
		}
    }
} 

void strrev(unsigned char *str) {

	int i;
	int j;
	unsigned char a;
	unsigned len = strlen((const char *)str);
	for (i = 0, j = len - 1; i < j; i++, j--) {
		a = str[i];
		str[i] = str[j];
		str[j] = a;
	}
}

char* itoa(int value, char* buffer, int base) {

	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = value;
	if (n < 0) {
		n = - n;
	}

	int i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	strrev((unsigned char*) buffer);
	return buffer;
}

char lower_case(char c) {
	if (c <= 'Z' && c >= 'A') {
		return c + 'a' - 'A';
	}
	return c;
}

char upper_case(char c) {
	if (c <= 'z' && c >= 'a') {
		return c - 'a' + 'A';
	}
	return c;
}

int strcmp(const char *p1, const char *p2) {
	const unsigned char *s1 = (const unsigned char *) p1;
	const unsigned char *s2 = (const unsigned char *) p2;
	unsigned char c1, c2;
	do {
		c1 = (unsigned char) *s1++;
		c2 = (unsigned char) *s2++;
		if (c1 == '\0')
			return c1 - c2;
		}
	while (c1 == c2);
	return c1 - c2;
}

/*
fait en sorte que le nombre dans s fasse decimals décimales
*/
char* format_number_decimals(char *s, int decimals) {
	int len = strlen(s);
	int missing_dec = decimals - len;
	if (missing_dec <= 0) {
		return s;
	}
	for (int i = len; i != -1; --i) {
		s[i + missing_dec] = s[i];
	}
	for (int i = 0; i < missing_dec; ++i) {
		s[i] = '0';
	}
	return s;
} 
