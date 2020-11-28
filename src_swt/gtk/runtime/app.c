/*
 * app.c
 *
 *  Created on: 20 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include <swt.h>
#include <unistd.h>
const char* w_app_get_current_directory(w_app *app) {
	if (app->currentdir == 0) {
		app->currentdir = getcwd(NULL, 0);
	}
	return app->currentdir;
}
#endif
