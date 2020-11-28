/*
 * TListDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TListDemo.h"

void TListDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TListDemo());
}

void TListDemo::CreateControl(WComposite *parent) {
	char txt[50];
	this->Create(parent, W_NONE);
	this->SetLayout(this->layout);
	this->layout.numColumns = 2;
	labels[0].Create(this, W_NONE);
	labels[0].SetText("SINGLE");
	labels[1].Create(this, W_NONE);
	labels[1].SetText("MULTI");
	lists[0].Create(this, W_BORDER | W_VSCROLL);
	lists[1].Create(this, W_BORDER | W_MULTI | W_VSCROLL);
	lists[0].SetLayoutData(WGridData(W_GRID_FILL_HORIZONTAL | W_GRID_FILL_VERTICAL));
	lists[1].SetLayoutData(WGridData(W_GRID_FILL_HORIZONTAL | W_GRID_FILL_VERTICAL));
	for (int i = 0; i < 50; i++) {
		sprintf(txt, "item %d", i);
		lists[0].Add(txt);
		lists[1].Add(txt);
	}
}
