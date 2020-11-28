/*
 * transfer.c
 *
 *  Created on: 4 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "dnd.h"
#include "toolkit.h"
/*
 * Transfer
 */
wresult w_transfer_registre(w_transfer *transfer) {
	size_t types_length = 0;
	w_transfer_type *types = transfer->get_types(transfer, &types_length);
	return W_TRUE;
}
wresult w_transfer_registre_with_name(w_transfer *transfer, const char *name) {
	transfer->type.name = name;
	transfer->is_supported_type = w_transfer_is_supported_type;
	transfer->get_data = w_transfer_get_data;
	transfer->set_data = w_transfer_set_data;
	return w_transfer_registre(transfer);
}
wresult w_transfer_is_supported_type(w_transfer *transfer,
		w_transfer_data *transferData) {
	if (transferData == 0)
		return W_FALSE;
	size_t types_length = 0;
	w_transfer_type *types = transfer->get_types(transfer, &types_length);
	return W_FALSE;
}
w_transfer_type* w_transfer_get_types(w_transfer *transfer, size_t *length) {
	if (length != 0)
		*length = 1;
	return (w_transfer_type*) &transfer->type;
}
size_t w_transfer_get_data(w_transfer *transfer, w_transfer_data *transferData,
		w_alloc alloc,void* user_data) {
}
wresult w_transfer_set_data(w_transfer *transfer, w_transfer_data *transferData,
		const void *data, size_t size) {
}
/*
 * TextTransfer
 */
w_transfer_type w_transfer_text_types[] = { //
		{ "UTF8_STRING" }, //
				{ "STRING" }, //
				{ "COMPOUND_TEXT" }, //
		};
w_transfer_type* w_transfer_text_get_types(w_transfer *transfer,
		size_t *length) {
	return w_transfer_text_types;
}
size_t w_transfer_text_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc,void* user_data) {

}
wresult w_transfer_text_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	return W_TRUE;
}
w_transfer w_transfer_text = { //
		w_transfer_is_supported_type, //
				w_transfer_text_get_types, //
				w_transfer_text_get_data, //
				w_transfer_text_set_data, //
				"UTF8_STRING", 0, 0 };
/*
 * FileTransfer
 */
w_transfer_type w_transfer_file_types[] = { //
		{ "text/uri-list" }, //
				{ "x-special/gnome-copied-files" }, //
		};
w_transfer_type* w_transfer_file_get_types(w_transfer *transfer,
		size_t *length) {
	*length = 2;
	return w_transfer_file_types;
}
size_t w_transfer_file_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc *alloc) {
}
wresult w_transfer_file_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
}
w_transfer w_transfer_file = { //
		w_transfer_is_supported_type, //
				w_transfer_file_get_types, //
				w_transfer_file_get_data, //
				w_transfer_file_set_data, //
				"text/uri-list", 0, 0 };
/*
 * HTMLTransfer
 */
w_transfer_type w_transfer_html_types[] = { //
		{ "text/html" }, //
				{ "TEXT/HTML" }, //
		};
w_transfer_type* w_transfer_html_get_types(w_transfer *transfer,
		size_t *length) {
	*length = 2;
	return w_transfer_html_types;
}
size_t w_transfer_html_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc,void* user_data) {
}
wresult w_transfer_html_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	return W_TRUE;
}
w_transfer w_transfer_html = { //
		w_transfer_is_supported_type, //
				w_transfer_html_get_types, //
				w_transfer_html_get_data, //
				w_transfer_html_set_data, //
				"text/html", 0, 0 };
/*
 * ImageTransfer
 */
w_transfer_type w_transfer_image_types[] = { //
		{ "image/jpeg" }, //
				{ "image/png" }, //
				{ "image/bmp" }, //
				{ "image/eps" }, //
				{ "image/pcx" }, //
				{ "image/ppm" }, //
				{ "image/rgb" }, //
				{ "image/tga" }, //
				{ "image/xbm" }, //
				{ "image/xpm" }, //
				{ "image/xv" }, //
		};
w_transfer_type* w_transfer_image_get_types(w_transfer *transfer,
		size_t *length) {
	*length = sizeof(w_transfer_image_types)
			/ sizeof(w_transfer_image_types[0]);
	return w_transfer_image_types;
}
size_t w_transfer_image_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc,void* user_data) {
}
wresult w_transfer_image_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	return W_TRUE;
}
w_transfer w_transfer_image = { //
		w_transfer_is_supported_type, //
				w_transfer_image_get_types, //
				w_transfer_image_get_data, //
				w_transfer_image_set_data, //
				"image/jpeg", 0, 0 };
/*
 * RTFTransfer
 */
w_transfer_type w_transfer_rtf_types[] = { //
		{ "text/rtf" }, //
				{ "TEXT/RTF" }, //
				{ "application/rtf" }, //
		};
w_transfer_type* w_transfer_rtf_get_types(w_transfer *transfer,
		size_t *length) {
	*length = sizeof(w_transfer_rtf_types) / sizeof(w_transfer_rtf_types[0]);
	return w_transfer_rtf_types;
}
size_t w_transfer_rtf_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc,void* user_data) {

}
wresult w_transfer_rtf_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
}
w_transfer w_transfer_rtf = { //
		w_transfer_is_supported_type, //
				w_transfer_rtf_get_types, //
				w_transfer_rtf_get_data, //
				w_transfer_rtf_set_data, //
				"text/rtf", 0, 0 };
/*
 * URLTransfer
 */
w_transfer_type w_transfer_url_types[] = { //
		{ "text/unicode" }, //
				{ "text/x-moz-url" }, //
		};
w_transfer_type* w_transfer_url_get_types(w_transfer *transfer,
		size_t *length) {
	*length = sizeof(w_transfer_url_types) / sizeof(w_transfer_url_types[0]);
	return w_transfer_url_types;
}
size_t w_transfer_url_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc,void* user_data) {
}
wresult w_transfer_url_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	return W_TRUE;
}
w_transfer w_transfer_url = { //
		w_transfer_is_supported_type, //
				w_transfer_url_get_types, //
				w_transfer_url_get_data, //
				w_transfer_url_set_data, //
				"text/unicode", 0, 0 };
#endif
