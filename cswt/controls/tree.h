//
// Created by azeddine on 27/08/2020.
//

#ifndef SRC_CUSTOM_CONTROLS_TREE_H
#define SRC_CUSTOM_CONTROLS_TREE_H
#include "../widgets/composite.h"
typedef struct w_tree_model {
    void* v_table;//not used in c
    struct w_tree_model_class* clazz;
}w_tree_model;
typedef struct w_tree_path {
}w_tree_path;
typedef struct w_tree_model_class {
    void* (*get_root)(w_tree_model* model);
    void* (*get_child)(w_tree_model* model,void* parent, int index);
    int (*get_child_count)(w_tree_model* model,void* parent);
    wbool (*is_leaf)(w_tree_model* model,void* node);
    int (*get_index_of_child)(w_tree_model* model,void* parent, void* child);
}w_tree_model_class;
typedef struct wc_tree_priv {
    w_imagelist* imagelist;
    int itemheight;
}wc_tree_priv;
void wc_tree_init_class(struct _w_tree_class *clazz);
#endif //SRC_CUSTOM_CONTROLS_TREE_H
