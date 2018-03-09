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

#endif

#ifdef __cplusplus
}
#endif
