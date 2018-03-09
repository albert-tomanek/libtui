#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <termbox.h>

#include "../include/box.h"
#include "../include/button.h"
#include "../include/misc.h"

tui_Button *tui_Button_new(const char *text)
{
	tui_Button *button = malloc(sizeof(tui_Button));
	if (! button) return NULL;
	
	/* Initialize the superclass */
	tui_Box_init(&(button->box));

	TUI_BOX(button)->width  = strlen(text) + 2 + 2;	// +2 for borders; +2 for horizontal padding
	TUI_BOX(button)->height = 3;

	TUI_BOX(button)->bg = TB_WHITE;
	TUI_BOX(button)->bg = TB_CYAN;

	TUI_BOX(button)->on_draw   = tui_Button_draw;		// Button has its own draw function.
		
	/* Initialize the button */
	button->text = strdup(text);
	
	return button;
}

void tui_Button_free(tui_Button *box)
{
	free(box->text);
	free(box);
}

void tui_Button_draw(tui_Box *__box, uint16_t x, uint16_t y)
{
	tui_Button *box = (tui_Button *) __box;
	
	uint16_t x1 = x, y1 = y, x2 = x + TUI_BOX(box)->width - 1, y2 = y + TUI_BOX(box)->height - 1;
	
	/* Draw shadow */
	tb_fill(TB_BLACK, x1 + 1, y1 + 1, x2 + 1, y2 + 1);		// Shadow is shifted down and right by one character.
	
	/* Draw box */
	tui_Box_draw(TUI_BOX(box), x, y);
	
	/* Draw text */
	tb_print(x1 + ((TUI_BOX(box)->width / 2) - (strlen(box->text) / 2)), y1 + (TUI_BOX(box)->height / 2), box->text, TUI_BOX(box)->fg, TUI_BOX(box)->bg);
}
