/*
 * link.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_LINK_H_
#define SRC_WIN32_CONTROLS_LINK_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/control.h"
typedef struct _w_link {
	struct _w_control control;
}_w_link;

struct _w_link_reserved {
	struct _w_control_reserved control;

};
#define _W_LINK(x) ((_w_link*)x)
#define _W_LINK_RESERVED(x) ((struct _w_link_reserved*)x)
void _w_link_class_init (struct _w_link_class* clazz);




#endif
#endif /* SRC_WIN32_CONTROLS_LINK_H_ */
