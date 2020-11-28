/*
 * TScales.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TScales.h"
#include "TScaleDemo.h"

void TScales::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Scale", new TScales());
}

bool TScales::Expand(WTreeItem &item) {
	TScaleDemo::Registre(item);
	return true;
}
