#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_BUTTON_H__
#define __TUI_BUTTON_H__

#include <stdint.h>
#include <bstring/bstrlib.h>

#include "box.h"

typedef struct tui_Button tui_Button;

struct tui_Button
{
	tui_Box box;	// We're inheriting tui_Box.
	
	bstring text;
	
	/* Click callback */
	void  *on_click_data;
	void (*on_click)(tui_Button *but, void *data);
};

tui_Button *tui_Button_new(const char *text);
void        tui_Button_free(tui_Button *but);
void        tui_Button_draw(tui_Box *box, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);	// Although the pointer is to a tui_Box, it is cast to a tui_Button
void        tui_Button_on_event(tui_Box *box, struct tb_event *event, void *data);
void        tui_Button_click(tui_Button *but);

#endif

#ifdef __cplusplus
}
#endif
