#include "libc.h" 

int main(int argc, char** argv, char* work_dir, LIBC* libc) {
	argc = argc;
	argv = argv;
	work_dir = work_dir;

	char itoa_buffer[16];
	unsigned int amount = 0;
	multiboot_memory_map_t* memory_map = libc->get_memory_map();
	int mmap_size = libc->get_memory_map_size();

	for (int i = 0; i < mmap_size; ++i) {
		multiboot_memory_map_t m = memory_map[i];
		amount += m.len;
		libc->printf("0x");
		libc->printf(libc->format_number_decimals(libc->itoa(m.addr, itoa_buffer, 16), 8));
		libc->printf(" -> 0x");
		libc->printf(libc->format_number_decimals(libc->itoa(m.addr + m.len, itoa_buffer, 16), 8));
		libc->printf("; Type = %x", m.type);
		if (m.type == 1) {
			libc->printf(" (Usable)");
		}
		else if (m.type == 2) {
			libc->printf(" (Reserved)");
		}
		else {
			libc->printf(" (Undefined)");
		}
		libc->printf("\n");
	}
	libc->printf("Total size: %d MB (%d B)\n", amount / 1000000, amount);

	return 0;
}