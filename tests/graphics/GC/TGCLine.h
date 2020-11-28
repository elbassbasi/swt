/*
 * TLine.h
 *
 *  Created on: 10 mai 2020
 *      Author: azeddine
 */

#ifndef TESTS_GRAPHICS_GC_TGCLINE_H_
#define TESTS_GRAPHICS_GC_TGCLINE_H_
#include "TGCs.h"
class TGCLine : public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool OnPaint(WPaintEvent &e);
};

#endif /* TESTS_GRAPHICS_GC_TGCLINE_H_ */
