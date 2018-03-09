#include <string.h>
#include <stdlib.h>
#include <stdint.h>
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
	uint16_t string_len = strlen(string);

	/* Convert the string to unicode */

	uint32_t *string32 = to_utf32(string);

	/* Now print it out */

	for (uint16_t i = 0; i < string_len; i++)
	{
		tb_change_cell(x + i, y, string32[i], fg, bg);
	}

	free(string32);
}

void tb_fill(uint16_t bg, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	struct tb_cell *buffer = tb_cell_buffer();
	
	for (uint16_t y = y1; y < y2; y++)
	{
		for (uint16_t x = x1; x < x2; x++)
		{
			buffer[y * tb_width() + x].bg = bg;
		}
	}
}
/*
void tb_draw_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fg, uint16_t bg)
{
	uint16_t width  = x2 - x1;
	uint16_t height = y2 - y1;

	tb_change_cell(x1, y1, TB_BOX_NW, fg, bg);	 			 // NW
	tb_change_cell(x2, y1, TB_BOX_NE, fg, bg);	 			 // NE
	tb_change_cell(x2, y2, TB_BOX_SE, fg, bg);	 			 // SE
	tb_change_cell(x1, y2, TB_BOX_SW, fg, bg);	 			 // SW

	for (uint16_t i = 0; i < (width - 1); i++)			 // Horizontals
	{
		tb_change_cell(1 + x1 + i, y1, TB_BOX_N, fg, bg);	 // N
		tb_change_cell(1 + x1 + i, y2, TB_BOX_S, fg, bg);	 // S
	}

	for (uint16_t i = 0; i < (height - 1); i++)			 // Verticals
	{
		tb_change_cell(x1, 1 + y1 + i, TB_BOX_E, fg, bg);	 // E
		tb_change_cell(x2, 1 + y1 + i, TB_BOX_W, fg, bg);	 // W
	}


	for (uint16_t x = 0; x < (width - 1); x++)
	{
		for (uint16_t y = 0; y < (height - 1); y++)
		{
			tb_change_cell(1 + x1 + x, 1 + y1 + y, TB_CHAR_SPACE, fg, bg);	 // For now, we have to fill it with a space character, so the box's contents will have to be re-written.
		}
	}

}
*/
