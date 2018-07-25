#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_WINDOW_H__
#define __TUI_WINDOW_H__

#include <stdint.h>
#include "box.h"

tui_Box *tui_Win_new(uint16_t width, uint16_t height);
void     tui_Win_draw(tui_Box *box, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     tui_Win_close(tui_Box *box);		// Close and dispose of a window

#endif

#ifdef __cplusplus
}
#endif
