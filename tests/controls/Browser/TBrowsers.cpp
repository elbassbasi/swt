/*
 * TBrowsers.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TBrowsers.h"
#include "TBrowserDemo.h"

void TBrowsers::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Browser", new TBrowsers());
}

bool TBrowsers::Expand(WTreeItem &item) {
	TBrowserDemo::Registre(item);
	return true;
}
