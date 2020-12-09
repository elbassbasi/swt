/*
 * clipboard.c
 *
 *  Created on: 4 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "toolkit.h"
void w_clipboard_init(w_clipboard *clipboard) {
}
wresult _w_toolkit_open_clipboard(w_toolkit *toolkit, w_clipboard *clipboard,
        int clipboards) {
    return W_TRUE;
}
wresult w_clipboard_clear_contents(w_clipboard *clipboard) {
    if (clipboard == 0)
        return W_ERROR_NULL_ARGUMENT;
    return W_TRUE;
}
wresult w_clipboard_close(w_clipboard *clipboard) {
    return W_TRUE;
}
wresult w_clipboard_is_ok(w_clipboard *clipboard) {
    return W_TRUE;
}
wresult w_clipboard_get_available_types(w_clipboard *clipboard,w_iterator* types){
    w_iterator_close(types);
    if (clipboard == 0)
        return W_ERROR_NULL_ARGUMENT;

    return W_FALSE;
}
wresult w_clipboard_get_contents(w_clipboard *clipboard, w_transfer *transfer,
        w_alloc alloc,void* user_data) {
    if (clipboard == 0)
        return W_ERROR_NULL_ARGUMENT;
    return W_TRUE;
}
wresult w_clipboard_set_contents(w_clipboard *clipboard, w_transfers *transfers,
        size_t length) {
    return W_TRUE;
}
#endif
