#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <termbox.h>

#include "../include/tui.h"
#include "../include/box.h"
#include "../include/misc.h"

struct
{
	/* The root widget/container. We had to subclass tui_Box	*
	 * because the root of the widget tree has some extra		*
	 * variables (eg. list of floating windows, *on_resize		*
	 * function pointer) that the other widgets don't need.		*/
	
	tui_Box box;	// Superclass needs to be first for TUI_BOX() cast to work.
	
	tui_Box *focused;
	
	void (*on_resize)(uint16_t width, uint16_t height, void *data);
	void  *on_resize_data;
	
} __tui_root;

bool __mainloop = false;	// true while the mainloop is running.

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
	
	__tui_root.focused = NULL;
	
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
		tui_Box_call_draw(&__tui_root.box, 0, 0);
		
		/* Draw floating boxes */
		
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
			__tui_root.focused = __tui_root.focused->next;
		}
		else if (event.type == TB_EVENT_KEY && event.ch == 'q')
		{
			/* q quits the mainloop */
			__mainloop = false;
			break;
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
	tb_shutdown();
}

tui_Box **tui_focused()
{
	return &(__tui_root.focused);
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

#include "../include/button.h"

void on_quit_button(tui_Button *but, void *data)
{
	tui_quit();
}

int main()
{
	tui_Box *root = tui_init();
	
	/* Create GUI */
	tui_Button *hello_button = tui_Button_new("Hello, world!");
	TUI_BOX(hello_button)->x = 3;
	TUI_BOX(hello_button)->y = 2;
	tui_add(TUI_BOX(hello_button), root);

	tui_Button *quit_button = tui_Button_new("Quit");
	TUI_BOX(quit_button)->x = 3;
	TUI_BOX(quit_button)->y = root->height - TUI_BOX(quit_button)->height - 2;
	quit_button->on_click = on_quit_button;
	tui_add(TUI_BOX(quit_button), root);
		
	/* Run */
	tui_mainloop();
	
	/* Clean up */
	tui_Button_free(quit_button);
	tui_Button_free(hello_button);
	
	tui_shutdown();
	return 0;
}
