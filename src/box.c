#include <stdlib.h>
#include <stdint.h>
#include <termbox.h>

#include "../include/box.h"
#include "../include/misc.h"

tui_Box *tui_Box_new()
{
	tui_Box *box = malloc(sizeof(tui_Box));
	
	box->x = 0;
	box->y = 0;
	
	box->width  = 0;
	box->height = 0;
	
	box->fg = TB_WHITE;
	box->bg = TB_BLUE;
	box->box_chars = &TUI_BOX_CHARS_SINGLE;
	
	box->next = NULL;
	box->child = NULL;
	
	box->on_draw  = tui_Box_draw_func;
	box->on_event = NULL;
	
	return box;
}

void tui_Box_free(tui_Box *box)
{
	free(box);
}

void tui_Box_draw(tui_Box *box, uint16_t x, uint16_t y)
{
	/* This is the recursive function that calls the	*
	 * draw function if present and draws the children.	*
	 * IT DOES NOT DO ANY ACTUAL DRAWING.				*/
	
	/* Draw the current box */
	if (box->on_draw)
		box->on_draw(box, x, y);
	
	/* Draw children */
	for (tui_Box *current = box->child; current != NULL; current = current->next)
	{
		tui_Box_draw(current, x + current->x, y + current->y);
	}
}

void tui_Box_draw_func(tui_Box *box, uint16_t x, uint16_t y)
{
	/* This function actually draws the box. */
	
	uint16_t x1 = x, y1 = y, x2 = x + box->width - 1, y2 = y + box->height - 1;
	
	/* Corners */
	tb_change_cell(x1, y1, box->box_chars->es, box->fg, box->bg);	// Top left corner
	tb_change_cell(x2, y1, box->box_chars->sw, box->fg, box->bg);	// Top right corner
	tb_change_cell(x2, y2, box->box_chars->nw, box->fg, box->bg);	// Bottom right corner
	tb_change_cell(x1, y2, box->box_chars->ne, box->fg, box->bg);	// Bottom left corner
	
	/* Horizontals */
	for (uint16_t x = x1 + 1; x < x2; x++)
	{
		tb_change_cell(x, y1, box->box_chars->ew, box->fg, box->bg);	// Top
		tb_change_cell(x, y2, box->box_chars->ew, box->fg, box->bg);	// Bottom
	}
	
	/* Verticals */
	for (uint16_t y = y1 + 1; y < y2; y++)
	{
		tb_change_cell(x1, y, box->box_chars->ns, box->fg, box->bg);	// Left
		tb_change_cell(x2, y, box->box_chars->ns, box->fg, box->bg);	// Right
	}
	
	/* Clear */
	for (uint16_t y = y1 + 1; y < y2; y++)
	{
		for (uint16_t x = x1 + 1; x < x2; x++)
		{
			tb_change_cell(x, y, ' ', box->fg, box->bg);
		}
	}	
}
