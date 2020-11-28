/*
 * TCombos.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_COMBO_TCOMBOS_H_
#define TESTS_CONTROLS_COMBO_TCOMBOS_H_
#include "../TControls.h"
class TCombos: public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_CONTROLS_COMBO_TCOMBOS_H_ */
