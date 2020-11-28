/*
 * TTableDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TTableDemo.h"
Person::Person(int i) {
	this->i = i;
	this->j = 0;
	this->progress = i;
}
void TTableDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TTableDemo());
}

void TTableDemo::CreateControl(WComposite *parent) {
	WTableItem item;
	WTableColumn column;
	char txt[50];
	this->Create(parent, W_HSCROLL | W_VSCROLL | W_FULL_SELECTION | W_CHECK);
	this->SetImageList(MShell::GetImageList32_(this));
	this->GetColumn(0, column).SetText("id");
	this->AppendColumn(column, "progress"); //.SetResizable(false);
	this->AppendColumn(column, "int");
	column.SetAlignment(W_CENTER);
	this->SetHeaderVisible(true);
	this->SetLinesVisible(true);
	int imagelistcount = this->GetImageList()->GetCount();
	for(wuint i=0;i<100;i++){
		sprintf(txt,"item %d",i);
		AppendItem(item, txt).SetImage(i % imagelistcount);
		item.SetData(new Person(i));
	}
}

WControl* TTableDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}
