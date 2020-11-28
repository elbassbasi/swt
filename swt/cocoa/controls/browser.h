/*
 * browser.h
 *
 *  Created on: 11 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_BROWSER_H_
#define SRC_COCOA_CONTROLS_BROWSER_H_

#ifdef __APPLE__
#include "../widgets/composite.h"
typedef struct _w_browser {
	_w_composite composite;
} _w_browser;
typedef struct _w_browser_kit {
	_w_browser browser;
	const char* htmlBytes;
} _w_browser_kit;

#define _W_BROWSER_KIT(x) ((struct _w_browser_kit*)x)

struct _w_browser_reserved {
	struct _w_composite_reserved composite;
	void* reserved[30];
};
struct _w_browser_kit_reserved {
	struct _w_composite_reserved composite;
	void *ExternalClass;
	struct {
		unsigned char version;
		union {
		};
	};
};
#define _W_BROWSER_KIT_RESERVED(x) ((struct _w_browser_kit_reserved*)x)
#define _W_BROWSER(x) ((_w_browser*)x)
#define _W_BROWSER_RESERVED(x) ((struct _w_browser_reserved*)x)
int _w_browser_kit_verify(struct _w_browser_class *clazz);
void _w_browser_kit_class_init(struct _w_browser_class *clazz);
void _w_browser_class_init(struct _w_browser_class *clazz);
#endif
#endif /* SRC_GTK_CONTROLS_BROWSER_H_ */
