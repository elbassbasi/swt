/*
 * TTrees.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTrees.h"
#include "TTreeCustom.h"
void TTrees::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Tree", new TTrees());
}

bool TTrees::Expand(WTreeItem &item) {
	TTreeCustom::Registre(item);
	return true;
}
