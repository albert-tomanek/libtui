#ifndef __TUI_MISC_H__
#define __TUI_MISC_H__

#include <stdint.h>

void tb_print(uint16_t x, uint16_t y, char *string, uint16_t fg, uint16_t bg);
void tb_fill(uint16_t bg, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

uint32_t *to_utf32(char *string);

#endif
