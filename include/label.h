/* TODO: Need support for '\n' */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_LABEL_H__
#define __TUI_LABEL_H__

#include <stdint.h>
#include "box.h"

typedef struct tui_Label tui_Label;

struct tui_Label
{
	tui_Box box;
	
	char *text;
};

tui_Label  *tui_Label_new(const char *text);
void        tui_Label_free(tui_Label *but);
void        tui_Label_draw(tui_Box *box, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#endif

#ifdef __cplusplus
}
#endif
