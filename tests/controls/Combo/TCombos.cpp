/*
 * TCombos.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TCombos.h"
#include "TComboDemo.h"

void TCombos::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Combo", new TCombos());
}

bool TCombos::Expand(WTreeItem &item) {
	TComboDemo::Registre(item);
	return true;
}
