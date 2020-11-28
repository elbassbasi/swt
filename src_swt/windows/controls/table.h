/*
 * table.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_TABLE_H_
#define SRC_WIN32_CONTROLS_TABLE_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
typedef struct _w_table {
	struct _w_composite composite;
}_w_table;

struct _w_table_reserved {
	struct _w_composite_reserved composite;

};
#define _W_TABLE(x) ((_w_table*)x)
#define _W_TABLE_RESERVED(x) ((struct _w_table_reserved*)x)
void _w_table_class_init (struct _w_table_class* clazz);


#endif
#endif /* SRC_WIN32_CONTROLS_TABLE_H_ */
