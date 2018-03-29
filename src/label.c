#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <termbox.h>

#include "../include/box.h"
#include "../include/label.h"
#include "../include/misc.h"

tui_Label *tui_Label_new(const char *text)
{
	tui_Label *lab = malloc(sizeof(tui_Label));
	if (! lab) return NULL;
	
	/* Initialize the superclass */
	tui_Box_init(&(lab->box));

	TUI_BOX(lab)->width  = strlen(text);
	TUI_BOX(lab)->height = 1;

	TUI_BOX(lab)->on_draw   = tui_Label_draw;		// Button has its own draw function.
		
	/* Initialize the lab */
	lab->text = strdup(text);
	if (! lab->text) return NULL;
	
	return lab;
}

void tui_Label_free(tui_Label *lab)
{
	free(lab->text);
	free(lab);
}

void tui_Label_draw(tui_Box *box, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fg, uint16_t bg)
{
	tui_Label *lab = (tui_Label *) box;
		
	/* Draw text */
	tb_print((x2 - x1) / 2 - strlen(lab->text) / 2, (y2 - y1) / 2, lab->text, fg, bg);
}