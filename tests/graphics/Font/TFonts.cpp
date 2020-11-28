/*
 * TFonts.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TFonts.h"

void TFonts::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Font", new TFonts());
}

bool TFonts::Expand(WTreeItem &item) {
	return true;
}
