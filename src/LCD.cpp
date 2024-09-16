#include "../include/Firmware/LCD.h"

byte screen_buffer[1 << 10];

ScreenBuffer::ScreenBuffer(int rows, int cols, int update_interval) : Timer(update_interval) {
	int size = cols + 1;
	ByteTable::Create(rows, size, screen_buffer);

	for (int i = 0; i < rows; i++) {
		*(GetRow(i) + cols) = 0; // chốt NULL cho mỗi dòng
	}
}
