/*
 * TBrowserDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_BROWSER_TBROWSERDEMO_H_
#define TESTS_CONTROLS_BROWSER_TBROWSERDEMO_H_
#include "TBrowsers.h"
class TBrowserDemo : public WBrowser , public ITreeItem {
public:
	static void Registre(WTreeItem &parent);
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);

};

#endif /* TESTS_CONTROLS_BROWSER_TBROWSERDEMO_H_ */
