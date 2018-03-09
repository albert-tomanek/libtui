#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <termbox.h>

#include "../include/tui.h"
#include "../include/box.h"
#include "../include/misc.h"

tui_Box *__tui_root;	// A pointer to the root widget/container. Global because there can only be one root widget.
						// ^- TODO: Need to subclass Box to contain a pointer to floating windows
bool __mainloop = false;	// true while the mainloop is running.

tui_Box *tui_init()
{
	int rc  = tb_init();
	if (rc != 0) return NULL;
	
	__tui_root = tui_Box_new();
	if (__tui_root == NULL) return NULL;
	
	__tui_root->width  = tb_width();
	__tui_root->height = tb_height();
	
	return __tui_root;
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
		tb_fill(TB_BLUE, 3, 3, 5, 6);
		
		/* Update the terminal */
		tb_present();
		
		/* Wait for an event */
		tb_poll_event(&event);
		
		/* Act upon the event */
		
		if (event.type == TB_EVENT_RESIZE)
		{
			TUI_BOX(__tui_root)->width  = event.w;
			TUI_BOX(__tui_root)->height = event.h;
			
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
	tui_mainloop();
	tui_shutdown();
	return 0;
}
