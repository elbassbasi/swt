/*
 * TTextDemo.h
 *
 *  Created on: 12 mai 2020
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_TEXT_TTEXTDEMO_H_
#define TESTS_CONTROLS_TEXT_TTEXTDEMO_H_
#include "TTexts.h"
class TTextDemo : public CompositeTreeItem {
public:
	WGridLayout layout;
	WText text[4];
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
};

#endif /* TESTS_CONTROLS_TEXT_TTEXTDEMO_H_ */
