#define FIRST_FREE_ADDR 0x1000000
#define MAX_FREE_ADDR 0xC0000000

typedef struct _allocated_memory_block allocated_memory_block;

struct _allocated_memory_block {
	int size;
};

typedef struct _free_memory_block free_memory_block;

struct _free_memory_block {
	unsigned int size;
	free_memory_block * next;
};

free_memory_block * free_list;