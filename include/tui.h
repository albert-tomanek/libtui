#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_H__
#define __TUI_H__

#include "chars.h"
#include "box.h"

tui_Box *tui_init();
void     tui_mainloop();
void     tui_shutdown();

/* Linked list functions */
void tui_add(tui_Box *child, tui_Box *parent);	// Add the child to the parent boxs.
tui_Box *__tui_getlast(tui_Box *first);			// Get the last node of the linked list.

#endif

#ifdef __cplusplus
}
#endif
