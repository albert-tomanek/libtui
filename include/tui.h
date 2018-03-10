#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_H__
#define __TUI_H__

#include "chars.h"
#include "box.h"

tui_Box  *tui_init();
void      tui_mainloop();
void      tui_quit();		// Stop the mainloop
void      tui_shutdown();	// Clean up

tui_Box **tui_focused();	// returns a pointer to the pointer which determines which widget is in focus.
void      tui_on_resize(void (*on_resize)(uint16_t width, uint16_t height, void *data), void *data);	// Set the callback to be called when the window is resized

/* Linked list functions */
void tui_add(tui_Box *child, tui_Box *parent);	// Add the child to the parent boxs.
tui_Box *__tui_getlast(tui_Box *first);			// Get the last node of the linked list.

#endif

#ifdef __cplusplus
}
#endif
