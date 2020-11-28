/*
 * module.c
 *
 *  Created on: 22 janv. 2020
 *      Author: azeddine El Bassbasi
 */
#if defined( __APPLE__)
#include <swt.h>

wresult w_module_unload(w_module *module) {
	return W_TRUE;
}
w_module* w_module_load(const char *module) {
}
w_module* w_module_get(const char *module) {
}
void* w_module_get_symbol(w_module *module, const char *symbol) {
}
const char* w_module_default_extension() {
}
const char* w_get_command_line() {
	return 0;
}
#endif
