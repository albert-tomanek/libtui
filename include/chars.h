#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TUI_CHARS_H__
#define __TUI_CHARS_H__

#include <stdint.h>

/* Box Characters */
typedef struct
{
	uint32_t ne;	// Order priority: N, E, S, W
	uint32_t nw;
	uint32_t es;
	uint32_t sw;
	uint32_t ns;
	uint32_t ew;

	uint32_t nes;
	uint32_t nsw;
	uint32_t new;
	uint32_t esw;
	
	uint32_t nesw;
} tui_BoxChars;

extern const tui_BoxChars TUI_BOX_CHARS_SINGLE;
extern const tui_BoxChars TUI_BOX_CHARS_DOUBLE;
extern const tui_BoxChars TUI_BOX_CHARS_EMPTY;

/* CharMap */

typedef struct
{
	/* This struct holds the codepoints for all the available	*
	 * box-drawing characters in its codepage.					*/
	
/*	uint32 ns;		// Capital signifies double line
	uint32 nsw;
	uint32 nsW;
	uint32 NSw;
	uint32 */
	
} tui_CharMap;

struct
{
	/* This contains a charmap struct for each supported codepage. */
	
	tui_CharMap CP437;
	tui_CharMap UTF8;
	
} tui_BOX_CHARS/* = {
	/* Here we fill in all the structs with their characters' codepoints. *
	
	.CP437 = {
		},
	.UTF8 = {
		}
}*/;

#endif

#ifdef __cplusplus
}
#endif
