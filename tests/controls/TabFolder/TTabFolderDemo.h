/*
 * TTabFolderDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TABFOLDER_TTABFOLDERDEMO_H_
#define TESTS_CONTROLS_TABFOLDER_TTABFOLDERDEMO_H_
#include "TTabFolders.h"
class TTabFolderDemo : public WTabFolder , public ITreeItem {
public:
	static const int numofTab = 20;
	static void Registre(WTreeItem &parent);
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);
	WText text[numofTab];
};

#endif /* TESTS_CONTROLS_TABFOLDER_TTABFOLDERDEMO_H_ */
