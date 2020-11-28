/*
 * dnd.c
 *
 *  Created on: 26 févr. 2020
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "toolkit.h"
void w_clipboard_init(w_clipboard *clipboard) {
}
wresult w_clipboard_clear_contents(w_clipboard *clipboard) {
}
wresult w_clipboard_close(w_clipboard *clipboard) {
}
wresult w_clipboard_is_ok(w_clipboard *clipboard) {
}
wresult w_clipboard_get_available_types(w_clipboard *clipboard,
		w_iterator *types) {
}
wresult w_clipboard_get_contents(w_clipboard *clipboard, w_transfer *transfer,
		w_alloc alloc, void *user_data) {
}
wresult w_clipboard_set_contents(w_clipboard *clipboard, w_transfers *transfers,
		size_t length) {
}

#endif
