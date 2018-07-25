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

tui_Box **tui_focused();		// returns a pointer to the pointer that determines which widget is in focus.
tui_Box  *tui_focused_win();	// returns a pointer to the window that is currently in focus.
void      tui_set_focused_win(tui_Box *win);	// Sets focus to the given window.

void      tui_on_resize(void (*on_resize)(uint16_t width, uint16_t height, void *data), void *data);	// Set the callback to be called when the window is resized

void      tui_add(tui_Box *child, tui_Box *parent);		// Add the child to the parent boxs.
void      tui_win_add(tui_Box *box);					// Add a box as a window

void      tui_destroy(tui_Box *box);			// Dispose of a widget and its children. Null-proof. Root widget does not have to be destroyed.

tui_Box *__tui_getlast(tui_Box *first);			// Get the last node of the linked list.

#endif

#ifdef __cplusplus
#endif
