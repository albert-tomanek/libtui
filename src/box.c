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
	
	box->on_draw  = tui_Box_draw;
	box->on_event = NULL;
}

void tui_Box_free(tui_Box *box)
{
	free(box);
}

void tui_Box_draw(tui_Box *box, uint16_t x, uint16_t y)
{
	
}
