/*
 * button.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef SRC_CUSTOM_CONTROLS_BUTTON_H_
#define SRC_CUSTOM_CONTROLS_BUTTON_H_
#include "../widgets/ccanvas.h"
typedef struct wc_button_priv {
	int state;
	char *text;
	w_image image;
}wc_button_priv;
void wc_button_init_class(struct _w_widget_class *clazz);
#endif /* SRC_CUSTOM_CONTROLS_BUTTON_H_ */
