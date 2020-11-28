/*
 * TTableDemo.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TABLE_TTABLEDEMO_H_
#define TESTS_CONTROLS_TABLE_TTABLEDEMO_H_
#include "TTables.h"
class Person {
public:
	Person(int i);
public:
	char name[30];
	int i;
	int j;
	int progress;
};
class TTableDemo : public WTable,public ITreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WControl* GetControl(WComposite *parent);
};

#endif /* TESTS_CONTROLS_TABLE_TTABLEDEMO_H_ */
