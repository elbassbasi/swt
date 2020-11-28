/*
 * TTabFolders.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTabFolders.h"
#include "TTabFolderDemo.h"
#include "TTabFolderClose.h"

void TTabFolders::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "TabFolder", new TTabFolders());
}

bool TTabFolders::Expand(WTreeItem &item) {
	TTabFolderDemo::Registre(item);
	TTabFolderClose::Registre(item);
	return true;
}
