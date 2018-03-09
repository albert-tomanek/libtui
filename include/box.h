#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_BOX_H__
#define __TUI_BOX_H__

#include <stdint.h>
#include "chars.h"

typedef struct tui_Box tui_Box;

struct tui_Box
{
	uint8_t x, y;			// The box's x and y WITHIN ITS CONTAINER
	uint8_t width, height;	// INCLUDES the box's border.

	uint16_t fg, bg;
	const tui_BoxChars *box_chars;	// The characters used to draw the bo	x border. This pointer is _not_ freed upon destruction.
	
	/* Pointers */
	tui_Box *next;
	tui_Box *child;
	
	void (*on_draw)(tui_Box *box, uint16_t x, uint16_t y);		// Gets called to draw the box. Points to tui_Box_draw() by default. If you are overriding this but still want the normal box to be drawn, call tui_Box_draw() yourself.
	void (*on_event)(tui_Box *box, struct tb_event *event);	// This gets called to handle termbox events
};

#define TUI_BOX(B) ((tui_Box *) B)		// GLib-style struct cast to make it nicer to use Box member functions on classes that inherit it.

/* Member functions */
tui_Box *tui_Box_new();
void     tui_Box_free(tui_Box *box);
void     tui_Box_draw(tui_Box *box, uint16_t x, uint16_t y);	// x and y are absolute.

#endif

#ifdef __cplusplus
}
#endif
