/* TODO: Need support for '\n' */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_ENTRY_H__
#define __TUI_ENTRY_H__

#include <stdint.h>
#include <stdbool.h>
#include <bstring/bstrlib.h>

#include "box.h"

typedef struct tui_Entry tui_Entry;

struct tui_Entry
{
	tui_Box box;
	
	char    *text;
	uint16_t text_len;		// Excludes null-term
	bool    limit_len;
	
	/* Enter key callback */
	void  *on_submit_data;
	bool (*on_submit)(tui_Entry *entry, void *data);		// Return value signifies whether the text field should be cleared afterwards
};

tui_Entry  *tui_Entry_new(uint16_t width);
void        tui_Entry_free(tui_Entry *entry);
void        tui_Entry_draw(tui_Box *box, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void        tui_Entry_on_event(tui_Box *box, struct tb_event *event, void *data);

void        tui_Entry_clear(tui_Entry *entry);
void        tui_Entry_set(tui_Entry *entry, const char *str);
void        tui_Entry_add_c(tui_Entry *entry, char c);

#endif

#ifdef __cplusplus
}
#endif
