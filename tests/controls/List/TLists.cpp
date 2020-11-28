/*
 * TLists.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TLists.h"
#include "TListDemo.h"

void TLists::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "List", new TLists());
}

bool TLists::Expand(WTreeItem &item) {
	TListDemo::Registre(item);
	return true;
}
