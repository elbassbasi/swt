/*
 * TGroups.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TGroups.h"
#include "TGroupDemo.h"

void TGroups::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Group", new TGroups());
}

bool TGroups::Expand(WTreeItem &item) {
	TGroupDemo::Registre(item);
	return true;
}
