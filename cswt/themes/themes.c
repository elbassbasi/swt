//
// Created by azeddine on 27/08/2020.
//
#include "metal.h"
struct themes_creates {
    const char* name;
    themes_creates_function create;
};
struct themes_creates _themes_creates[] = {
        {"Metal",w_theme_metal_create},

};
themes_creates_function w_toolkit_custom_find_theme_0(const char* name){
    for(size_t i=0;i<sizeof(_themes_creates) / sizeof(_themes_creates[0]);i++){
        if(!strcmp(name,_themes_creates[i].name)){
            return _themes_creates[i].create;
        }
    }
    return 0;
}