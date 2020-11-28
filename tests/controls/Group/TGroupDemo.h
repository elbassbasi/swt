/*
 * TGroupDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_GROUP_TGROUPDEMO_H_
#define TESTS_CONTROLS_GROUP_TGROUPDEMO_H_
#include "TGroups.h"
class TGroupDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	static const char* RADIO_TEXTS[];
	WGridLayout layout;
	WGroup group[2];
	WGridLayout grouplayout[2];
	WButton radio[8];
};

#endif /* TESTS_CONTROLS_GROUP_TGROUPDEMO_H_ */
