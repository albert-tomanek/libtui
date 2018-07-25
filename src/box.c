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
	box->destructor = tui_Box_free;
	
	box->x = 0;
	box->y = 0;
	
	box->selectable = false;
	
	box->width  = 0;
	box->height = 0;
	
	box->fg  = TB_WHITE;
	box->bg  = TB_BLUE;
	box->sbg = box->bg;
	
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
	tui_Box_free_children(box);
	
	/* Free struct */
	free(box);					// This will work even if the function is called on a struct which inherits tui_Box, since free() magically knows how large the memory is.
}

void tui_Box_free_children(tui_Box *box)
{
	/* Go through list freeing children */
	if (box->child != NULL)
	{
		tui_Box *current = box->child, *next;
		
		current->prev->next = NULL;		// The last widget [in the circular list of children] won't point to the first widget anymore (it will point to null), so we can loop over the list without the fear of going round in circles. Especially since the first widget will be gone once we get to the last widget.
		
		while (current != NULL)
		{
			next = current->next;
			current->destructor(current);
			current = next;
		}
	}
}

void tui_Box_draw(tui_Box *box, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	/* This function actually draws the box. */
	uint16_t fg = (state == TUI_CLICKED) ? box->fg | TB_BOLD : box->fg;
	uint16_t bg = (state == TUI_NORMAL) ? box->bg : box->sbg;
		
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
