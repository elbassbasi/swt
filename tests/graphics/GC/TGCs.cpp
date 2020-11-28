/*
 * TGCs.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TGCs.h"
#include "TGCDemo.h"
#include "TGCLine.h"

void TGCs::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Graphics", new TGCs());
}

bool TGCs::Expand(WTreeItem &item) {
	TGCDemo::Registre(item);
	TGCLine::Registre(item);
	return true;
}
