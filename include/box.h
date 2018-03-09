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
	uint16_t x, y;			// The box's x and y WITHIN ITS CONTAINER
	uint16_t width, height;	// INCLUDES the box's border.

	uint16_t fg, bg;
	const tui_BoxChars *box_chars;	// The characters used to draw the bo	x border. This pointer is _not_ freed upon destruction.
	
	/* Pointers */
	tui_Box *next;
	tui_Box *child;
	
	void (*on_draw)(tui_Box *box, uint16_t x, uint16_t y);		// Gets called to draw the box. Points to tui_Box_draw() by default. If you are overriding this but still want the normal box to be drawn, call tui_Box_draw() yourself.
	void (*on_event)(tui_Box *box, struct tb_event *event);	// This gets called to handle termbox events
};

#define TUI_BOX(B) ((tui_Box *) B)		// GObject-style struct cast to make it nicer to use Box member functions on classes that inherit it. For this to work, subclasses have to have a tui_Box as their first member.

/* Member functions */
tui_Box *tui_Box_new();
void     tui_Box_free(tui_Box *box);
void     tui_Box_draw     (tui_Box *box, uint16_t x, uint16_t y);	// This does *NOT* do any drawing. It can be called on _any_ subclass of tui_Box and calls the ->on_draw function (if present) and calls itsself on the box's children. x and y are the absolute coordinates of the top left of the box.
void     tui_Box_draw_func(tui_Box *box, uint16_t x, uint16_t y);	// This function does the actual drawing of the box. tui_Box->on_draw is set to this by default. Subclasses of tui_Box do not need to call this.

#endif

#ifdef __cplusplus
}
#endif
