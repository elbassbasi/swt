//
// Created by azeddine on 28/08/2020.
//

#ifndef SRC_GTK_WIDGETS_THEME_H
#define SRC_GTK_WIDGETS_THEME_H
#ifdef __linux
#include "shell.h"
typedef struct _w_theme_gtk {
    w_theme theme;
}_w_theme_gtk;

void _w_theme_gtk_init(_w_theme_gtk* theme);

#endif
#endif //SRC_GTK_WIDGETS_THEME_H
