/*
 * TTabFolderClose.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TABFOLDER_TTABFOLDERCLOSE_H_
#define TESTS_CONTROLS_TABFOLDER_TTABFOLDERCLOSE_H_
#include "TTabFolders.h"
class TTabFolderClose : public WTabFolder , public ITreeItem {
public:
	static const int numofTab = 20;
	static void Registre(WTreeItem &parent);
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);
	bool OnItemClose(WTabItemEvent &e);
	WText text[numofTab];
};

#endif /* TESTS_CONTROLS_TABFOLDER_TTABFOLDERCLOSE_H_ */
