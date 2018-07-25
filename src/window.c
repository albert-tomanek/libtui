#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <termbox.h>

#include "../include/tui.h"
#include "../include/misc.h"
#include "../include/window.h"

tui_Box *tui_Win_new(uint16_t width, uint16_t height)
{
	tui_Box *win = tui_Box_new();
	if (! win) return NULL;
	
	win->width  = width;
	win->height = height;

	win->fg  = TB_WHITE;
	win->bg  = TB_CYAN;
	win->sbg = TB_CYAN;
	
	win->box_chars = &TUI_BOX_CHARS_DOUBLE;

	win->on_draw   = tui_Win_draw;		// Button has its own draw function.
	//win->on_event  = tui_Button_on_event;
		
	return win;
}

void tui_Win_close(tui_Box *box)
{
	/* Remove the selection if it's selected */
	if (tui_focused_win() == box)
	{
		tui_set_focused_win(box->prev);
	}
	
	/* Remove it from the list of open windows */
	{
		box->prev->next = box->next;
		box->next->prev = box->prev;
	}
	
	tui_destroy(box);
}

void tui_Win_draw(tui_Box *win, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	/* Draw shadow */
	tb_fill(TB_BLACK, x1 + 1, y1 + 1, x2 + 1, y2 + 1);		// Shadow is shifted down and right by one character.
	
	/* Draw box */
	tui_Box_draw(win, (state == TUI_NORMAL) ? TUI_NORMAL : TUI_CLICKED, x1, y1, x2, y2);
}
