/*
 * TTables.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTables.h"
#include "TTableDemo.h"
void TTables::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Table", new TTables());
}

bool TTables::Expand(WTreeItem &item) {
	TTableDemo::Registre(item);
	return true;
}
