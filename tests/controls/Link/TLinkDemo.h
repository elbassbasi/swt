/*
 * TLinkDemo.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_LINK_TLINKDEMO_H_
#define TESTS_CONTROLS_LINK_TLINKDEMO_H_
#include "TLinks.h"
class TLinkDemo: public WLink, public ITreeItem {
public:
	static void Registre(WTreeItem &parent);
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);
};

#endif /* TESTS_CONTROLS_LINK_TLINKDEMO_H_ */
