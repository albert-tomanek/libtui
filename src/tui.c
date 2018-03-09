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
} __tui_root;

bool __mainloop = false;	// true while the mainloop is running.

tui_Box *tui_init()
{
	/* Initialize Termbox */
	int rc  = tb_init();
	if (rc != 0) return NULL;
	
	/* Initialize the root container */
	__tui_root.box.width  = tb_width();
	__tui_root.box.height = tb_height();
	
	return (tui_Box *) &__tui_root;
}

void tui_mainloop()
{
	__mainloop = true;
	struct tb_event event;
	
	while (__mainloop)
	{
		/* Clear the screen first */
		tb_clear();
		
		/* Draw children */
		tui_Box_draw(&__tui_root.box, 0, 0);
		
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
			
			// TODO: on_resize
		}

		if (event.type == TB_EVENT_KEY)
		{
			if (event.ch == 'q')
			{
				/* ALT + X quits the mainloop */
				__mainloop = false;
				break;
			}
		}
	}
}

void tui_shutdown()
{
	tb_shutdown();
}

int main()
{
	tui_Box *root = tui_init();
	tui_Box *my_box = tui_Box_new();
	my_box->x = 10;
	my_box->y = 2;
	my_box->width  = 12;
	my_box->height = 8;
	root->child = my_box;
	
	tui_mainloop();
	
	tui_Box_free(my_box);
	tui_shutdown();
	return 0;
}
