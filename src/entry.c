#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <termbox.h>
#include <bstring/bstrlib.h>

#include "../include/box.h"
#include "../include/entry.h"
#include "../include/misc.h"

#define GROW_SIZE 64

tui_Entry *tui_Entry_new(uint16_t width)
{
	tui_Entry *entry = malloc(sizeof(tui_Entry));
	if (! entry) return NULL;
	
	/* Initialize the superclass */
	tui_Box_init(&(entry->box));

	TUI_BOX(entry)->width  = width;
	TUI_BOX(entry)->height = 1;
	
	TUI_BOX(entry)->bg  = TB_BLACK;
	TUI_BOX(entry)->sbg = TB_BLACK;
	
	TUI_BOX(entry)->selectable = true;

	TUI_BOX(entry)->on_draw  = tui_Entry_draw;
	TUI_BOX(entry)->on_event = tui_Entry_on_event;
	
	TUI_BOX(entry)->destructor = tui_Entry_free;
		
	/* Initialize the entry */
	entry->text = strdup("");
	entry->text_len = 0;
	
	return entry;
}

void tui_Entry_free(tui_Entry *entry)
{
	free(entry->text);
	
	tui_Box_free(TUI_BOX(entry));
}

void tui_Entry_draw(tui_Box *box, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	tui_Entry *entry = (tui_Entry *) box;
	
	uint16_t fg = TUI_BOX(entry)->fg;
	uint16_t bg = (state == TUI_NORMAL) ? TUI_BOX(entry)->bg : TUI_BOX(entry)->sbg;
	
	uint16_t cursor_pos = (entry->text_len < TUI_BOX(entry)->width - 1) ? entry->text_len : TUI_BOX(entry)->width - 1;
		
	/* Draw text */
	tb_fill(TB_BLACK, x1, y1, x2, y2);
	tb_print(x1, y1, entry->text + entry->text_len - cursor_pos, fg, bg);
	
	/* Draw the cursor if we're selected */
	if (state == TUI_SELECTED)
		tb_set_cursor(x1 + cursor_pos, y1);
}

void tui_Entry_on_event(tui_Box *box, struct tb_event *event, void *data)
{
	tui_Entry *entry = (tui_Entry *) box;
	
	if (event->ch)
	{
		if (entry->text_len < TUI_BOX(entry)->width ||		// If we have space for a nother character
			entry->limit_len == false)							// Or we're allowed to go beyond the limit
		{
			tui_Entry_add_c(entry, (char) event->ch);
		}
	}
	else if (event->key)
	{
		switch (event->key)
		{
			case TB_KEY_BACKSPACE:
			case TB_KEY_BACKSPACE2:
				/* Remove the last character */
				if (entry->text_len != 0)
				{
					entry->text[entry->text_len - 1] = '\0';
					entry->text_len--;
				}
				break;
				
			case TB_KEY_SPACE:
				tui_Entry_add_c(entry, ' ');
				break;
			
			case TB_KEY_ENTER:
			{
				bool clear = entry->on_submit(entry, entry->on_submit_data);
				
				if (clear)
					tui_Entry_clear(entry);
				
				break;
			}
		}
		
	}	
}

void tui_Entry_clear(tui_Entry *entry)
{
	free(entry->text);
	
	entry->text     = strdup("");
	entry->text_len = 0;
}

void tui_Entry_set(tui_Entry *entry, const char *str)
{
	free(entry->text);
	
	entry->text     = strdup(str);
	entry->text_len = strlen(str);
}

void tui_Entry_add_c(tui_Entry *entry, char chr)
{
	entry->text = (char *) realloc(entry->text, entry->text_len + 2/*(entry->text_len + GROW_SIZE) - (entry->text_len % GROW_SIZE) + 2*/);		// This ensures that the memory is reallocated only whenever we reach a multiple of GROW_SIZE, hopefully saving us from having to resize the memory every time. +1 for new character; +1 for null-term.
	entry->text[entry->text_len] = chr;
	entry->text[entry->text_len + 1] = '\0';
	
	entry->text_len++;
}
