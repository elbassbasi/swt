/*
 * TListDemo.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_LIST_TLISTDEMO_H_
#define TESTS_CONTROLS_LIST_TLISTDEMO_H_
#include "TLists.h"
class TListDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WGridLayout layout;
	WList lists[2];
	WLabel labels[2];
};

#endif /* TESTS_CONTROLS_LIST_TLISTDEMO_H_ */
