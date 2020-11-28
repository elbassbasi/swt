/*
 * TScaleDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TScaleDemo.h"

void TScaleDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TScaleDemo());
}

void TScaleDemo::CreateControl(WComposite *parent) {
	this->Create(parent,W_NONE);
	this->SetLayout(this->layout);
	scaleH.Create(this,W_NONE);
	scaleH.SetLayoutData(WGridData(W_GRID_FILL_HORIZONTAL));
	scaleV.Create(this,W_VERTICAL);
	scaleV.SetLayoutData(WGridData(W_GRID_FILL_VERTICAL));
}
