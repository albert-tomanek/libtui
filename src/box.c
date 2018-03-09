#include <stdlib.h>
#include <stdint.h>
#include <termbox.h>

#include "../include/box.h"
#include "../include/misc.h"

tui_Box *tui_Box_new()
{
	tui_Box *box = malloc(sizeof(tui_Box));
	
	if (! box) return NULL;
	
	tui_Box_init(box);
	
	return box;
}

void tui_Box_init(tui_Box *box)
{
	box->x = 0;
	box->y = 0;
	
	box->width  = 0;
	box->height = 0;
	
	box->fg = TB_WHITE;
	box->bg = TB_BLUE;
	box->box_chars = &TUI_BOX_CHARS_SINGLE;
	
	box->next = NULL;
	box->prev = NULL;
	box->child = NULL;
	
	box->on_draw  = tui_Box_draw;
	box->on_event = NULL;
}

void tui_Box_free(tui_Box *box)
{
	/* Free children */
	/* Children are freed manually by the library user */
	
	/* Free struct */
	free(box);
}

void tui_Box_call_draw(tui_Box *box, uint16_t x, uint16_t y)
{
	/* This is the recursive function that calls the	*
	 * draw function if present and draws the children.	*
	 * IT DOES NOT DO ANY ACTUAL DRAWING.				*/
	
	/* Draw the current box */
	if (box->on_draw)
		box->on_draw(box, x, y);
	
	/* Draw children */
	if (box->child != NULL)
	{
		tui_Box *current = box->child;
		
		do
		{
			tui_Box_call_draw(current, x + current->x, y + current->y);
			
			current = current->next;	
		}
		while (current != box->child);
	}
}

void tui_Box_draw(tui_Box *box, uint16_t x, uint16_t y)
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
