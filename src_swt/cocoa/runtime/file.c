/*
 * file.c
 *
 *  Created on: 9 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include <swt.h>
FILE* w_fopen(const char *file, const char *mode) {
	FILE *f = fopen(file, mode);
	return f;
}
typedef struct w_iterator_dir {
} w_iterator_dir;
wresult _w_iterator_dir_next(w_iterator *it, void *obj) {
}
wresult _w_iterator_dir_reset(w_iterator *it) {
	return W_TRUE;
}
wresult _w_iterator_dir_close(w_iterator *it) {
}
wresult _w_iterator_dir_remove(w_iterator *it) {
	return W_FALSE;
}
size_t _w_iterator_dir_get_count(w_iterator *it) {
}
_w_iterator_class _w_iterator_dir_class = { _w_iterator_dir_close,
		_w_iterator_dir_next, _w_iterator_dir_reset, _w_iterator_dir_remove,
		_w_iterator_dir_get_count };
wresult w_dir_list(const char *dir, w_iterator *files) {
}
#endif
