/*
 * TBrowserDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TBrowserDemo.h"

void TBrowserDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent,"Demonstrates",new TBrowserDemo());
}

WControl* TBrowserDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
    
}

void TBrowserDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->SetText("<!DOCTYPE html><html><head><title>Page Title</title></head>"
			"<body><h1>My First Heading</h1><a href=\"hehh\">"
			"Test Browser.</a> HEllO AZEDDINE</body></html>");
}
