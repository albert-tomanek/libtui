#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_BUTTON_H__
#define __TUI_BUTTON_H__

#include <stdint.h>
#include "box.h"

typedef struct
{
	tui_Box box;	// We're inheriting from tui_Box.
	
	char *text;
} tui_Button;

tui_Button *tui_Button_new(const char *text);
void        tui_Button_free(tui_Button *box);
void        tui_Button_draw(tui_Box *__box, uint16_t x, uint16_t y);	// Although the pointer is to a tui_Box, it is cast to a tui_Button

#endif

#ifdef __cplusplus
}
#endif
