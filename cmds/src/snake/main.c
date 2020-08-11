#include "libc.h" 

#define WIDTH 80
#define HEIGHT 25
#define GRID_WIDTH 30
#define GRID_HEIGHT 25
#define SLEEP_WAIT_MS 150

int8_t x_pos[GRID_WIDTH*GRID_HEIGHT];
int8_t y_pos[GRID_WIDTH*GRID_HEIGHT];
int16_t pos_offset;
int16_t snake_length;
uint8_t direction; // 0: x positive, 1: x negative, 2: y positive, 3: y negative
int8_t apple_x;
int8_t apple_y;
uint8_t paused;
uint8_t lose;

int is_in_the_snake(int8_t x, int8_t y) {
	int16_t offset = pos_offset;
	for (int16_t i = 0; i < snake_length; ++i) {
		if (x_pos[offset] == x && y_pos[offset] == y) {
			return 1;
		}
		offset--;
		if (offset < 0) {
			offset += GRID_WIDTH*GRID_HEIGHT;
		}
	}
	return 0;
}

void place_random_apple(LIBC* libc) {
	do {
		apple_x = libc->randint(0, GRID_WIDTH);
		apple_y = libc->randint(0, GRID_HEIGHT);
	} while (is_in_the_snake(apple_x, apple_y));
	libc->print_char_at(' ', apple_x * 2, apple_y, 0x40); // Red background
	libc->print_char_at(' ', apple_x * 2 + 1, apple_y, 0x40);
}

int game_finished() {
	return snake_length == GRID_WIDTH*GRID_HEIGHT;
}

int8_t get_next_x(int8_t x, uint8_t direction) {
	if (direction == 0) return x + 1;
	if (direction == 1) return x - 1;
	return x;
}

int8_t get_next_y(int8_t y, uint8_t direction) {
	if (direction == 2) return y + 1;
	if (direction == 3) return y - 1;
	return y;
}

int16_t get_last_tail_offset() {
	int16_t off = pos_offset - snake_length + 1;
	while (off < 0) {
		off += GRID_WIDTH*GRID_HEIGHT;
	}
	off = off % (GRID_WIDTH*GRID_HEIGHT);
	return off;
}

void change_direction(uint8_t key) {
	if (key == 0x48 || key == 0xC8 || key == 0x11 || key == 0x91) { // Up
		if (direction <= 1) {
			direction = 3;
		}
	}
	if (key == 0x4B || key == 0xCB || key == 0x1E || key == 0x9E) { // Left
		if (direction >= 2) {
			direction = 1;
		}
	}
	if (key == 0x4D || key == 0xCD || key == 0x20 || key == 0xA0) { // Right
		if (direction >= 2) {
			direction = 0;
		}
	}
	if (key == 0x50 || key == 0xD0 || key == 0x1F || key == 0x9F) { // Down
		if (direction <= 1) {
			direction = 2;
		}
	}
}

void init_ui(LIBC* libc) {

	libc->print_char_at('S', WIDTH - 15, 4, 0);
	libc->printf("core total");

	libc->print_char_at('E', WIDTH - 15, 10, 0);
	libc->printf("tat du jeu");
}

void update_ui(LIBC* libc) {

	int length_digit3 = snake_length / 100;
	int length_digit2 = (snake_length - length_digit3 * 100) / 10;
	int length_digit1 = snake_length % 10;
	libc->print_char_at('0' + length_digit3, WIDTH - 11, 6, 0);
	libc->print_char_at('0' + length_digit2, WIDTH - 10, 6, 0);
	libc->print_char_at('0' + length_digit1, WIDTH - 9, 6, 0);

	int press = 0;

	for (int i = 1; i < WIDTH - GRID_WIDTH * 2; ++i) {
		libc->print_char_at(' ', GRID_WIDTH * 2 + i, 12, 0);
	}
	if (paused) {
		libc->print_char_at('P', WIDTH - 12, 12, 0);
		libc->printf("ause");
	}
	else if (game_finished()) {
		libc->print_char_at('V', WIDTH - 14, 12, 0);
		libc->printf("ictoire!");
		press = 1;
	}
	else if (lose) {
		libc->print_char_at('G', WIDTH - 14, 12, 0);
		libc->printf("ame over");
		press = 1;
	}
	else {
		libc->print_char_at('E', WIDTH - 14, 12, 0);
		libc->printf("n partie");
	}
	if (press) {
		libc->print_char_at('A', WIDTH - 15, 16, 0);
		libc->printf("ppuyez sur");
		libc->print_char_at('u', WIDTH - 17, 17, 0);
		libc->printf("ne touche pour");
		libc->print_char_at('q', WIDTH - 14, 18, 0);
		libc->printf("uitter..");
	}
}

int game_loop(LIBC* libc) {

	int8_t cur_x = x_pos[pos_offset];
	int8_t cur_y = y_pos[pos_offset];
	uint8_t last_key = libc->get_last_key();

	if (last_key == 0x81) {
		libc->reset_last_key();
		paused = 1;
		update_ui(libc);
		while (libc->get_last_key() != 0x81) {
			libc->sleep(50);
		}
		paused = 0;
	}
	else {
		change_direction(last_key);
	}
	libc->reset_last_key();

	int8_t next_x = get_next_x(cur_x, direction);
	int8_t next_y = get_next_y(cur_y, direction);

	if (next_x < 0 || next_y < 0 || next_x >= GRID_WIDTH || next_y >= GRID_HEIGHT) {
		return 0;
	}
	if (is_in_the_snake(next_x, next_y)) {
		return 0;
	}

	int16_t tail_offset = get_last_tail_offset();
	libc->print_char_at(' ', next_x * 2, next_y, 0x20); // Green background
	libc->print_char_at(' ', next_x * 2 + 1, next_y, 0x20);
	if (next_x == apple_x && next_y == apple_y) {
		apple_x = -1;
		apple_y = -1;
		snake_length++;
	}
	else {
		libc->print_char_at(' ', x_pos[tail_offset] * 2, y_pos[tail_offset], 0);
		libc->print_char_at(' ', x_pos[tail_offset] * 2 + 1, y_pos[tail_offset], 0);
	}

	pos_offset = (pos_offset + 1) % (GRID_WIDTH*GRID_HEIGHT);
	x_pos[pos_offset] = next_x;
	y_pos[pos_offset] = next_y;

	if (!game_finished() && apple_x == -1 && apple_y == -1) {
		place_random_apple(libc);
	}

	update_ui(libc);

	return !game_finished();
}

int main(int argc, char** argv, char* work_dir, LIBC* libc) {
	argc = argc;
	argv = argv;
	work_dir = work_dir;

	libc->clear_screen();

	/* Right column */
	for (int i = 0; i < GRID_HEIGHT; ++i) {
		libc->print_char_at(' ', GRID_WIDTH * 2, i, 0xf0); // White background
	}

	pos_offset = 0;
	snake_length = 1;
	apple_x = -1;
	apple_y = -1;
	paused = 0;
	place_random_apple(libc);
	x_pos[pos_offset] = (int8_t) libc->randint(10, GRID_WIDTH - 10);
	y_pos[pos_offset] = (int8_t) libc->randint(10, GRID_HEIGHT - 10);
	direction = (uint8_t) libc->randint(0, 4);
	init_ui(libc);
	
	while(game_loop(libc)) {
		libc->sleep(SLEEP_WAIT_MS);
	}

	if (!game_finished()) {
		lose = 1;
	}
	update_ui(libc);
	libc->reset_last_key();
	while (libc->get_last_key() == 0);

	libc->clear_screen();

	return 0;
}