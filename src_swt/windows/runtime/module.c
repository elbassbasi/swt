/*
 * module.c
 *
 *  Created on: 10 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "core.h"
w_module* w_module_load(const char *module) {
	struct UnicodeString str;
	unicode_set(&str, module, -1);
	w_module *hmodule = LoadLibraryW(str.str);
	unicode_free(&str);
	return hmodule;
}
w_module* w_module_get(const char *module) {
	struct UnicodeString str;
	unicode_set(&str, module, -1);
	w_module *hmodule = GetModuleHandleW(str.str);
	unicode_free(&str);
	return hmodule;
}
wresult w_module_unload(w_module *module) {
	return FreeLibrary((HMODULE) module);
}
const char* w_module_default_extension() {
	return ".dll";
}
void* w_module_get_symbol(w_module *module, const char *symbol) {
	return GetProcAddress((HMODULE) module, symbol);
}

#endif
