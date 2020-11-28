/*
 * dialogs.c
 *
 *  Created on: 1 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "toolkit.h"

int _w_toolkit_messagebox_check_style(int style) {
	int mask = (W_YES | W_NO | W_OK | W_CANCEL | W_ABORT | W_RETRY | W_IGNORE);
	int bits = style & mask;
	if (bits == W_OK || bits == W_CANCEL || bits == (W_OK | W_CANCEL))
		return style;
	if (bits == W_YES || bits == W_NO || bits == (W_YES | W_NO)
			|| bits == (W_YES | W_NO | W_CANCEL))
		return style;
	if (bits == (W_RETRY | W_CANCEL) || bits == (W_ABORT | W_RETRY | W_IGNORE))
		return style;
	style = (style & ~mask) | W_OK;
	return style;
}
int _w_toolkit_messagebox_open(w_toolkit *toolkit, w_messagebox *messagebox) {
}
#endif
