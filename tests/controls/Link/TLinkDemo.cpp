/*
 * TLinkDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TLinkDemo.h"

void TLinkDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TLinkDemo());
}

WControl* TLinkDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

void TLinkDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	const char *text =
			"The SWT component is designed to provide <a>efficient</a>, <a>portable</a>"
					"<a href=\"native\">access to the user-interface facilities of the operating systems</a>"
					"on which it is implemented.";
	this->SetText(text);
}
