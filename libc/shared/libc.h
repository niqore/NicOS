#ifndef LIBC_H
#define LIBC_H

typedef struct _libc {
	void (*printf)(const char *format, ...);
} LIBC;

#endif