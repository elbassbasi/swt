/*
 * TLinks.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TLinks.h"
#include "TLinkDemo.h"

void TLinks::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Link", new TLinks());
}

bool TLinks::Expand(WTreeItem &item) {
	TLinkDemo::Registre(item);
	return true;
}
