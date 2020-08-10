#include "libc.h" 

int main(int argc, char** argv, char* work_dir, LIBC* libc) {
	argc = argc;
	argv = argv;

	libc->printf("%s\n", work_dir);

	return 0;
}