/*
 * TTabFolderDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TTabFolderDemo.h"

void TTabFolderDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TTabFolderDemo());
}

WControl* TTabFolderDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

void TTabFolderDemo::CreateControl(WComposite *parent) {
	WTabItem item;
	char txt[50];
	this->Create(parent, W_NONE);
	this->SetImageList(MShell::GetImageList16_(this));
	int ImageListcount = MShell::GetImageList16_(this)->GetCount();
	if (ImageListcount <= 0)
		ImageListcount = 1;
	for (int i = 0; i < numofTab; i++) {
		this->text[i].Create(this, W_MULTI | W_WRAP | W_VSCROLL);
		sprintf(txt, "text #%d", i);
		this->text[i].SetText(txt);
		sprintf(txt, "tab %d", i);
		this->AppendItem(item, txt);
		item.SetControl(&this->text[i]);
		item.SetImage(i % ImageListcount);
	}
}
