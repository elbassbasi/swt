/*
 * TTreeCustom.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TTreeCustom.h"

void TTreeCustom::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "CustomDraw", new TTreeCustom());
}

WControl* TTreeCustom::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

TTreeCustom::TTreeCustom() {
}

TTreeCustom::~TTreeCustom() {
}

void TTreeCustom::CreateControl(WComposite *parent) {
	WTreeItem item, tmp, root;
	WTreeColumn column;
	char txt[50];
	this->Create(parent, W_HSCROLL | W_VSCROLL | W_FULL_SELECTION | W_CHECK);
	this->SetImageList(MShell::GetImageList32_(this));
	this->GetColumn(0, column).SetText("id");
	this->AppendColumn(column, "progress"); //.SetResizable(false);
	this->AppendColumn(column, "int");
	column.SetAlignment(W_CENTER);
	this->SetHeaderVisible(true);
	this->SetLinesVisible(true);
	this->GetRootItem(root);
	int imagelistcount = this->GetImageList()->GetCount();
	if (imagelistcount == 0)
		imagelistcount = 1;
	for (int i = 0; i < 50; i++) {
		sprintf(txt, " test %d", i);
		if (root.AppendItem(item, txt).IsOk()) {
			item.SetImage(i % imagelistcount);
			item.SetData(new Person(i, -1));
			for (int j = 0; j < 5; j++) {
				sprintf(txt, "test %d,%d", i, j);
				item.AppendItem(tmp, 0);
				if (tmp.IsOk()) {
					tmp.SetImage(j);
					tmp.SetData(new Person(i, j));
				}
			}
		}
	}
	text.Create(this, W_NONE);
	WFontData fontdata;
	this->GetFont()->GetFontData(fontdata);
	fontdata.SetStyle(W_BOLD);
	this->fontBold.Create(fontdata);
	text.SetVisible(false);
}

Person::Person(int i, int j) {
	this->i = i;
	this->j = j;
	if (j < 0) {
		sprintf(this->name, "person %d", i);
		this->progress = i % 5;
	} else {
		sprintf(this->name, "_person %d,%d", i, j);
		this->progress = j;
	}
}
#define NUM_PIXEL 1
bool TTreeCustom::OnItemMeasure(WTreeEvent &e) {
	e.rect->height = 40;
	return true;
}

bool TTreeCustom::OnItemErase(WTreeEvent &e) {
	e.detail &= ~W_HOT;
	if ((e.detail & W_SELECTED) == 0)
		return false; /* item not selected */
	WRect rect;
	GetClientArea(rect);
	int clientWidth = rect.width;
	int oldForeground = e.gc->GetForeground();
	int oldBackground = e.gc->GetBackground();
	e.gc->SetForeground(W_COLOR_RED);
	e.gc->SetBackground(W_COLOR_YELLOW);
	e.gc->FillGradientRectangle(0, e.rect->y, clientWidth, e.rect->height,
			false);
	e.gc->SetForeground(oldForeground);
	e.gc->SetBackground(oldBackground);
	e.detail &= ~W_SELECTED;
	return true;
}

bool TTreeCustom::OnItemPaint(WTreeEvent &e) {
	Person *p = (Person*) e.item->GetData();
	char txt[20];
	if (e.column->GetIndex() == 1) {
		WRect clip;
		e.gc->GetClipping(clip);
		p = (Person*) e.item->GetData();
		WRect r(*e.rect);
		r.x += NUM_PIXEL;
		r.y += NUM_PIXEL;
		r.width -= 2 * NUM_PIXEL;
		r.height -= 2 * NUM_PIXEL;
		e.gc->SetForeground(W_COLOR_BLUE);
		e.gc->SetBackground(W_COLOR_RED);
		if (p != 0) {
			WTheme *theme = GetToolkit()->GetTheme();
			if (theme) {
				e.gc->FillRectangle(r);
				WProgressBarThemeData data;
				data.maximum = 5;
				data.selection = p->progress;
				theme->DrawBackground(data, *e.gc, r);
				sprintf(txt, "%d%%", p->progress);
				theme->DrawText(data, *e.gc, r, txt, W_THEME_DRAW_CENTER);
			} else {
				//WColor foreground = e.gc->GetForeground();
				//WColor background = e.gc->GetBackground();
				WSize sz = e.gc->TextExtent(txt);
				int w = r.x + (r.width - sz.width) / 2;
				r.width = (r.width * p->progress) / 5;
				e.gc->SetForeground(W_COLOR_RED);
				e.gc->SetBackground(W_COLOR_YELLOW);
				e.gc->FillGradientRectangle(r, true);
				e.gc->DrawRectangle(r);
				sprintf(txt, "%d%%", p->progress);
				e.gc->SetForeground(0xFFFF8080);
				e.gc->DrawText(txt, w, r.y, W_DRAW_TRANSPARENT);
			}
		} else {
			e.gc->SetForeground(W_COLOR_BLUE);
			e.gc->DrawText("unknown", r.x, r.y, W_DRAW_TRANSPARENT);
		}
		return true;
	}
	return WTree::OnItemPaint(e);
}

bool TTreeCustom::OnItemGetValue(WTreeEvent &e) {
	Person *p = (Person*) e.item->GetData();
	switch (e.column->GetIndex()) {
	case 0:
		e.value->SetString(p->name);
		break;
	case 2:
		e.value->Sprint("i %s %d", p->name, p->progress);
		break;
	default:
		return WTree::OnItemGetValue(e);
		break;
	}
	return true;
}

bool TTreeCustom::OnItemGetAttr(WTreeEvent &e) {
	Person *p;
	if (e.column->GetIndex() == 2) {
		p = (Person*) e.item->GetData();
		if (p != 0) {
			if (e.item->GetChecked()) {
				e.attr->foreground = W_COLOR_RED;
				e.attr->background = W_COLOR_MAGENTA;
				e.attr->font = &this->fontBold;
			} else {
				e.attr->foreground = W_COLOR_BLUE;
			}
		}
		return true;
	}
	return false;
}
bool TTreeCustom::OnItemDispose(WTreeEvent &e) {
	Person *p = (Person*) e.item->GetData();
	if (p != 0) {
		delete p;
	}
	return false;
}

bool TTreeCustom::OnMouseDoubleClick(WMouseEvent &e) {
	WTreeItem item;
	GetItem(item, e.x, e.y);
	if (item.IsOk()) {
		WRect bounds;
		item.GetBounds(bounds);
		text.SetBounds(bounds);
		text.SetVisible(true);
	}
	return false;
}
