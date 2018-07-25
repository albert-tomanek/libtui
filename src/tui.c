#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <termbox.h>

#include "../include/tui.h"
#include "../include/box.h"
#include "../include/misc.h"

#include <stdio.h>

void     __tui_draw(tui_Box *box, uint16_t x, uint16_t y);
tui_Box *__tui_selectable_traverse(tui_Box *current, bool direction);	// Gets the next/previous (determined by `direction`) widget that is selectable

struct
{
	/* The root widget/container. We had to subclass tui_Box	*
	 * because the root of the widget tree has some extra		*
	 * variables that the other widgets don't need.	Unlike most	*
	 * widgets where box.next & .prev point to sibling widgets,	*
	 * here they point to other windows.						*/
	
	tui_Box box;	// Superclass needs to be first for TUI_BOX() cast to work.
	
	tui_Box *focused;
	tui_Box *focused_win;
	
	void (*on_resize)(uint16_t width, uint16_t height, void *data);
	void  *on_resize_data;
	
} __tui_root;

static bool __mainloop = false;	// true while the mainloop is running.

/* Functions */

tui_Box *tui_init()
{
	/* Initialize Termbox */
	int rc  = tb_init();
	if (rc != 0) return NULL;
	
	/* Initialize the root container */
	tui_Box_init(&__tui_root.box);

	__tui_root.box.width  = tb_width();
	__tui_root.box.height = tb_height();
	
	__tui_root.box.box_chars = &TUI_BOX_CHARS_NONE;
	
	__tui_root.box.destructor = NULL;		// We will take care of the destruction of __tui_root in tui_shutdown().
	
	__tui_root.focused = NULL;
	__tui_root.focused_win = &__tui_root.box;
	
	__tui_root.on_resize = NULL;
	__tui_root.on_resize_data = NULL;
	
	return (tui_Box *) &__tui_root;
}

void tui_mainloop()
{
	__mainloop = true;
	struct tb_event event;
	
	/* Focus on the first widget */
	__tui_root.focused = __tui_root.box.child;	// If no children have been added, then ->child will remain NULL.
	
	while (__mainloop)
	{
		/* Clear the screen first */
		tb_clear();
		
		/* Draw children */
		tb_set_cursor(-1, -1);	// Hide the cursor unless a widget wants it.
		
		__tui_draw(TUI_BOX(&__tui_root), 0, 0);
		
		/* Draw floating boxes */
		for (tui_Box *current = __tui_root.focused_win->next; current != __tui_root.focused_win && current != NULL; current = current->next)
		{
			if (current != TUI_BOX(&__tui_root))
				__tui_draw(current, current->x, current->y);
		}
		
		/* Draw the one that is in focus on top */
		if (__tui_root.focused_win != TUI_BOX(&__tui_root))
		{
			tui_Box *focused_win = __tui_root.focused_win;
			__tui_draw(focused_win, focused_win->x, focused_win->y);
		}
		else
		{
			/* Unless the root widget is in focus in which case	*
			 * drawing it again would draw over the floating	*
			 * windows.											*/
		}
		
		/* Update the terminal */
		tb_present();
		
		/* Wait for an event */
		tb_poll_event(&event);
		
		/* Act upon the event */
		
		if (event.type == TB_EVENT_RESIZE)
		{
			__tui_root.box.width  = event.w;
			__tui_root.box.height = event.h;
			
			if (__tui_root.on_resize)
				__tui_root.on_resize(event.w, event.h, __tui_root.on_resize_data);
		}
		else if (event.type == TB_EVENT_KEY && event.key == TB_KEY_TAB)
		{
			__tui_root.focused = __tui_selectable_traverse(__tui_root.focused, true);
		}
		else if (event.key == TB_KEY_CTRL_RSQ_BRACKET)
		{
			tui_set_focused_win(__tui_root.focused_win->next);
		}
		else if (event.key == TB_KEY_CTRL_LSQ_BRACKET)
		{
			tui_set_focused_win(__tui_root.focused_win->next);
		}
		else
		{
			tui_Box *focused = __tui_root.focused;
			if (focused->on_event)
				focused->on_event(focused, &event, focused->on_event_data);
		}
	}
}

void tui_quit()
{
	__mainloop = false;
}

void tui_shutdown()
{
	/* Delete all floating windows */
	for (tui_Box *current = __tui_root.box.next; current != &__tui_root.box && current != NULL; current = current->next)
	{
		tui_destroy(current);
	}
	
	/* Finalize the root widget */
	tui_Box_free_children(TUI_BOX(&__tui_root));
	
	/* Quit termbox */
	tb_shutdown();
}

void __tui_draw(tui_Box *box, uint16_t x, uint16_t y)
{
	/* This is the recursive function that calls a box's	*
	 * draw function if present and draws the children.		*
	 * IT DOES NOT DO ANY ACTUAL DRAWING.					*/
	
	/* Draw the current box */
	if (box->on_draw)
	{
		box->on_draw(box, (__tui_root.focused == box || __tui_root.focused_win == box) ? TUI_SELECTED : TUI_NORMAL, x, y, x + box->width - 1, y + box->height - 1);
	}
	
	/* Draw children */
	if (box->child != NULL)
	{
		tui_Box *current = box->child;
		
		do
		{
			__tui_draw(current, x + current->x, y + current->y);
			
			current = current->next;	
		}
		while (current != box->child);
	}
}

tui_Box **tui_focused()
{
	return &(__tui_root.focused);
}

tui_Box *tui_focused_win()
{
	return __tui_root.focused_win;
}

void tui_set_focused_win(tui_Box *win)
{
	__tui_root.focused_win = win;
	__tui_root.focused     = __tui_root.focused_win->child;
}

void tui_on_resize(void (*on_resize)(uint16_t width, uint16_t height, void *data), void *data)
{
	__tui_root.on_resize = on_resize;
	__tui_root.on_resize_data = data;
}

void tui_add(tui_Box *child, tui_Box *parent)
{
	child->parent = parent;
	
	if (parent->child == NULL)
	{
		parent->child = child;
		
		child->next = child;
		child->prev = child;
	}
	else
	{
		tui_Box *last = __tui_getlast(parent->child);
		
		last->next = child;
		parent->child->prev = child;	// Make prev of first node point to new last node.
		
		child->next = parent->child;	// Next of last node points to first node in the list.
		child->prev = last;
	}
}

void tui_add_win(tui_Box *win)
{
	/* Add the given box to the circular list of windows.	*/
	
	if (__tui_root.box.next == NULL)
	{
		/* If there aren't any other windows yet, we need	*
		 * to make both the root window's prev and next		*
		 * pointers point to the new window.				*/
		
		__tui_root.box.next = win;
		__tui_root.box.prev = win;
		
		win->next = &__tui_root.box;
		win->prev = &__tui_root.box;
	}
	else
	{
		tui_Box *last = __tui_getlast(&__tui_root.box);
		
		last->next = win;
		win->prev = last;
		
		win->next = &__tui_root.box;
		__tui_root.box.prev = win;
	}
}

void tui_destroy(tui_Box *box)
{
	/* Call a widget's destructor */
	if (box == NULL) return;
	
	if (box->destructor)
		box->destructor(box);
}

tui_Box *__tui_getlast(tui_Box *first)
{
	if (! first)
		return NULL;
	
	tui_Box *current = first;
	
	while (current->next != first)
	{
		current = current->next;
	}
	
	return current;
}

tui_Box *__tui_selectable_traverse(tui_Box *current, bool direction)
{
	do
	{
		current = direction ? current->next : current->prev;
	} while (current->selectable == false && current != NULL);
	
	return current;
}

/* Simple test program */

#include <stdio.h>
#include "../include/window.h"
#include "../include/label.h"
#include "../include/button.h"
#include "../include/entry.h"

void on_quit_button(tui_Button *but, void *data)
{
	tui_quit();
}

void on_win_close_button(tui_Button *but, void *data)
{
	tui_Win_close(TUI_BOX(but)->parent);
}

uint16_t wins = 8;
void on_win_button(tui_Button *but, void *data)
{
	/* Create a window, configure and add it */
	tui_Box *win = tui_Win_new(16, 8);
	tui_add_win(win);
	
	win->x = wins * 2;
	win->y = wins;
	
	wins += 2;
	
	/* Add widgets to it */
	tui_Button *close_button = tui_Button_new("Close");
	TUI_BOX(close_button)->x = 3;
	TUI_BOX(close_button)->y = 2;
	close_button->on_click = on_win_close_button;
	tui_add(TUI_BOX(close_button), win);
}

bool on_entry_submit(tui_Entry *entry, void *data)
{
	tui_Button_set_text((tui_Button *) data, entry->text);
	return true;
}

void on_resize(uint16_t width, uint16_t height, void *data)
{
	tui_Box *box = (tui_Box *) data;	// It's actually our label
	
	box->x = width - box->width;
	box->y = height - box->height;
}

int main()
{
	tui_Box *root = tui_init();
		
	/* Create GUI */
	
	tui_Button *hello_button = tui_Button_new("Hello, world!");
	TUI_BOX(hello_button)->x = 3;
	TUI_BOX(hello_button)->y = 2;
	tui_add(TUI_BOX(hello_button), root);

	tui_Button *win_button = tui_Button_new("New Window");
	TUI_BOX(win_button)->x = 23;
	TUI_BOX(win_button)->y = 2;
	win_button->on_click = on_win_button;
	tui_add(TUI_BOX(win_button), root);
	
	/*tui_Label *dims_label = tui_Label_new("(0,0)");		// Dimensions
	TUI_BOX(dims_label)->x = root->width - TUI_BOX(dims_label)->width;
	TUI_BOX(dims_label)->y = root->height - 1;
	tui_add(TUI_BOX(dims_label), root);
	
	tui_on_resize(on_resize, dims_label);*/
	
	tui_Entry *entry = tui_Entry_new(16);
	TUI_BOX(entry)->x = 3;
	TUI_BOX(entry)->y = 7;
	entry->on_submit = on_entry_submit;
	entry->on_submit_data = hello_button;
	tui_add(TUI_BOX(entry), root);
	
	tui_Button *quit_button = tui_Button_new("Quit");
	TUI_BOX(quit_button)->x = 3;
	TUI_BOX(quit_button)->y = root->height - TUI_BOX(quit_button)->height - 2;
	quit_button->on_click = on_quit_button;
	tui_add(TUI_BOX(quit_button), root);
	
	tui_Box *box = tui_Box_new();
	box->width  = 8;
	box->height = 4;
	box->x = root->width - box->width - 1;
	box->y = 1;
	tui_add(box, root);
		
	/* Run */
	tui_mainloop();
		
	tui_shutdown();
	return 0;
}
