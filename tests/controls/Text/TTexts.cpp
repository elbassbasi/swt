/*
 * TTexts.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTexts.h"
#include "TTextDemo.h"

void TTexts::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Text", new TTexts());
}

bool TTexts::Expand(WTreeItem &item) {
	TTextDemo::Registre(item);
	return true;
}
