/*
 * TTransforms.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTransforms.h"

void TTransforms::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Transform", new TTransforms());
}

bool TTransforms::Expand(WTreeItem &item) {
	return true;
}
