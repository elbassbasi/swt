/*
 * TScaleDemo.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_SCALE_TSCALEDEMO_H_
#define TESTS_CONTROLS_SCALE_TSCALEDEMO_H_
#include "TScales.h"
class TScaleDemo : public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WGridLayout layout;
	WScale scaleH;
	WScale scaleV;
};

#endif /* TESTS_CONTROLS_SCALE_TSCALEDEMO_H_ */
