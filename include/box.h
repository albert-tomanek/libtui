#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_BOX_H__
#define __TUI_BOX_H__

#include <stdint.h>
#include <stdbool.h>

#include "chars.h"
#include "state.h"

typedef struct tui_Box tui_Box;

struct tui_Box
{
	void (*destructor)(tui_Box *box);
	
	uint16_t x, y;			// The box's x and y WITHIN ITS CONTAINER
	uint16_t width, height;	// INCLUDES the box's border.
	
	bool selectable;

	uint16_t fg, bg, sbg;			// sbg = selected background
	const tui_BoxChars *box_chars;	// The characters used to draw the box border. This pointer is _not_ freed upon destruction. This variable may be ignored by some subclasses.
	
	/* Pointers */
	tui_Box *next, *prev;	// Boxes are stored in a dually-linked list.
	tui_Box *parent;
	tui_Box *child;			// Pointer to the box's first child box.
	
	void (*on_draw)(tui_Box *box, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		// Gets called to draw the box. Points to tui_Box_draw() by default. If you are overriding this but still want the normal box to be drawn, call tui_Box_draw() .
	void (*on_event)(tui_Box *box, struct tb_event *event, void *data);	// This gets called to handle termbox events
	void  *on_event_data;
};

#define TUI_BOX(B) ((tui_Box *) B)		// GObject-style struct cast to make it nicer to use Box member functions on classes that inherit it. For this to work, subclasses have to have a tui_Box as their first member.

/* Member functions */
tui_Box *tui_Box_new();
void     tui_Box_init(tui_Box *box);	// Initializes an existing tui_Box.
void     tui_Box_free(tui_Box *box);
void     tui_Box_free_children(tui_Box *box);
void     tui_Box_draw     (tui_Box *box, tui_State state, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);	// This function does the actual drawing of the box. tui_Box->on_draw is set to this by default. Subclasses of tui_Box do not need to call this.

#endif

#ifdef __cplusplus
}
#endif
