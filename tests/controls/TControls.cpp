/*
 * TControls.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TControls.h"
#include "Browser/TBrowsers.h"
#include "Button/TButtons.h"
#include "Combo/TCombos.h"
#include "CoolBar/TCoolBars.h"
#include "DateTime/TDateTimes.h"
#include "ExpandBar/TExpandBars.h"
#include "Group/TGroups.h"
#include "Label/TLabels.h"
#include "Link/TLinks.h"
#include "List/TLists.h"
#include "ProgressBar/TProgressBars.h"
#include "Sash/TSashes.h"
#include "Scale/TScales.h"
#include "Slider/TSliders.h"
#include "Spinner/TSpinners.h"
#include "TabFolder/TTabFolders.h"
#include "Table/TTables.h"
#include "Text/TTexts.h"
#include "ToolBar/TToolBars.h"
#include "Tree/TTrees.h"
void TControls::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Controls", new TControls());
}
bool TControls::Expand(WTreeItem &item) {
	TBrowsers::Registre(item);
	TButtons::Registre(item);
	TCombos::Registre(item);
	TCoolBars::Registre(item);
	TDateTimes::Registre(item);
	TExpandBars::Registre(item);
	TGroups::Registre(item);
	TLabels::Registre(item);
	TLinks::Registre(item);
	TLists::Registre(item);
	TProgressBars::Registre(item);
	TSashes::Registre(item);
	TScales::Registre(item);
	TSliders::Registre(item);
	TSpinners::Registre(item);
	TTabFolders::Registre(item);
	TTables::Registre(item);
	TTexts::Registre(item);
	TToolBars::Registre(item);
	TTrees::Registre(item);
	return true;
}

