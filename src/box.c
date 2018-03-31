#include <stdlib.h>
#include <stdint.h>
#include <termbox.h>

#include "../include/tui.h"
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
	
	box->selectable = true;
	
	box->width  = 0;
	box->height = 0;
	
	box->fg = TB_WHITE;
	box->bg = TB_BLUE;
	box->box_chars = &TUI_BOX_CHARS_SINGLE;
	
	box->next = NULL;
	box->prev = NULL;
	box->child  = NULL;
	box->parent = NULL;
	
	box->on_draw  = tui_Box_draw;
	box->on_event = NULL;
	box->on_event_data = NULL;
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
		box->on_draw(box, x, y, x + box->width - 1, y + box->height - 1, box->fg, (*(tui_focused()) == box) ? TB_RED : box->bg);
	
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

void tui_Box_draw(tui_Box *box, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fg, uint16_t bg)
{
	/* This function actually draws the box. */
		
	/* Corners */
	tb_change_cell(x1, y1, box->box_chars->es, fg, bg);	// Top left corner
	tb_change_cell(x2, y1, box->box_chars->sw, fg, bg);	// Top right corner
	tb_change_cell(x2, y2, box->box_chars->nw, fg, bg);	// Bottom right corner
	tb_change_cell(x1, y2, box->box_chars->ne, fg, bg);	// Bottom left corner
	
	/* Horizontals */
	for (uint16_t x = x1 + 1; x < x2; x++)
	{
		tb_change_cell(x, y1, box->box_chars->ew, fg, bg);	// Top
		tb_change_cell(x, y2, box->box_chars->ew, fg, bg);	// Bottom
	}
	
	/* Verticals */
	for (uint16_t y = y1 + 1; y < y2; y++)
	{
		tb_change_cell(x1, y, box->box_chars->ns, fg, bg);	// Left
		tb_change_cell(x2, y, box->box_chars->ns, fg, bg);	// Right
	}
	
	/* Clear */
	for (uint16_t y = y1 + 1; y < y2; y++)
	{
		for (uint16_t x = x1 + 1; x < x2; x++)
		{
			tb_change_cell(x, y, ' ', box->fg, bg);
		}
	}	
}
