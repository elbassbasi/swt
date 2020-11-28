/*
 * app.c
 *
 *  Created on: 20 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include <swt.h>
#include <unistd.h>
void w_app_init_thread(w_app *app){

}
const char* w_app_get_current_directory(w_app *app) {
	if (app->currentdir == 0) {
		app->currentdir = getcwd(NULL, 0);
	}
	return app->currentdir;
}
#endif
