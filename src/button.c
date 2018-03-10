#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <termbox.h>

#include "../include/box.h"
#include "../include/button.h"
#include "../include/misc.h"

#define CLICK_DURATION 100

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
	TUI_BOX(button)->on_event  = tui_Button_on_event;
		
	/* Initialize the button */
	button->text = strdup(text);
	
	button->on_click = NULL;
	button->on_click_data = NULL;
	
	return button;
}

void tui_Button_free(tui_Button *but)
{
	free(but->text);
	free(but);
}

void tui_Button_draw(tui_Box *box, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fg, uint16_t bg)
{
	tui_Button *but = (tui_Button *) box;
		
	/* Draw shadow */
	tb_fill(TB_BLACK, x1 + 1, y1 + 1, x2 + 1, y2 + 1);		// Shadow is shifted down and right by one character.
	
	/* Draw but */
	tui_Box_draw(TUI_BOX(but), x1, y1, x2, y2, fg, bg);
	
	/* Draw text */
	tb_print(x1 + ((TUI_BOX(but)->width / 2) - (strlen(but->text) / 2)), y1 + (TUI_BOX(but)->height / 2), but->text, fg, bg);
}

void tui_Button_on_event(tui_Box *box, struct tb_event *event, void *data)
{
	if (event->type == TB_EVENT_KEY && event->key == TB_KEY_ENTER)
	{
		tui_Button_click((tui_Button *) box);
	}
}

void tui_Button_click(tui_Button *but)
{
	/* Redraw the button while clicked */
	
	{
		/* Get its bounds */
		uint16_t x1 = 0, y1 = 0, x2, y2;
		
		for (tui_Box *current = TUI_BOX(but); current != NULL; current = current->parent)
		{
			/* Go up the widget hierarchy, growing the button's	*
			 * x and y until we reach the top.					*/
			
			x1 += current->x;
			y1 += current->y;
		}
		
		x2 = x1 + TUI_BOX(but)->width  - 1;
		y2 = y1 + TUI_BOX(but)->height - 1;
		
		/* Draw the button */
		if (TUI_BOX(but)->on_draw)
			TUI_BOX(but)->on_draw((tui_Box *) but, x1, y1, x2, y2, TUI_BOX(but)->fg | TB_BOLD, TB_RED);
		
		tb_present();
	}
	
	m_sleep(CLICK_DURATION);
	
	if (but->on_click)
		but->on_click(but, but->on_click_data);
}
