#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <termbox.h>

#include "../include/misc.h"

uint32_t *to_utf32(char *string)	// Supposedly works for ASCII only...
{
	size_t length = strlen(string);
	uint32_t *out = calloc(length + 1, sizeof(uint32_t));
	
	uint32_t *b = out;
	
	for (char *a = string; *a != '\0'; a++, b++)
	{
		*b = *a;
	}
	
	*b = 0;		// 32-bit null-terminator. `b` would have been incremented before the loop finished.

	return out;
}

void tb_print(uint16_t x, uint16_t y, char *string, uint16_t fg, uint16_t bg)
{
	for (char *c = string; *c != '\0'; c++, x++)
	{
		tb_change_cell(x, y, (uint32_t) *c, fg, bg);
	}
}

void tb_fill(uint16_t bg, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{	
	struct tb_cell *buffer = tb_cell_buffer();
	
	for (uint16_t y = y1; y <= y2; y++)
	{
		for (uint16_t x = x1; x <= x2; x++)
		{
			buffer[y * tb_width() + x].bg = bg;
		}
	}
}

void m_sleep(uint32_t millis)
{
	usleep(millis * 1000);
}
