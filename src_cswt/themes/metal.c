/*
 * metal.c
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */
#include "metal.h"
void w_theme_metal_draw_flush_3d_border(w_theme *theme, w_graphics *gc,
                                        w_rect *rect) {
	w_theme_metal *metal = (w_theme_metal*) theme;
	w_rect r;
	r.x = rect->x;
	r.y = rect->y;
	w_graphics_set_foreground(gc, metal->secondary1);
	r.width = rect->width - 2;
	r.height = rect->height - 2;
	w_graphics_draw_rectangle(gc, &r);
	w_graphics_set_foreground(gc, 0xFFFFFFFF);
	r.x = rect->x + 1;
	r.y = rect->y + 1;
	w_graphics_draw_rectangle(gc, &r);
	w_graphics_set_foreground(gc, metal->secondary3);
	r.x = rect->x + 0;
	r.y = rect->y + rect->height - 1;
	r.width = rect->x + 1;
	r.height = rect->y + rect->height - 1;
	w_graphics_draw_line(gc, &r.pt, (w_point*) &r.sz);
	r.x = rect->x + rect->width - 1;
	r.y = rect->y;
	r.width = rect->x + rect->width - 2;
	r.height = rect->y + 1;
	w_graphics_draw_line(gc, &r.pt, (w_point*) &r.sz);
}
void w_theme_metal_draw_gradient(w_theme *theme, w_graphics *gc, w_rect *rect,
		int vertical) {
	w_theme_metal *metal = (w_theme_metal*) theme;
	w_rect r;
	int mid;
	int mid2;
	int wh;
	if (vertical) {
		mid = (int) (metal->ratio1 * rect->height);
		mid2 = (int) (metal->ratio2 * rect->height);
		wh = rect->height;
	} else {
		mid = (int) (metal->ratio1 * rect->width);
		mid2 = (int) (metal->ratio2 * rect->width);
		wh = rect->width;
	}
	w_color c1 = metal->gradiant, c2 = W_COLOR_WHITE, c3 = metal->primary3;
	r.x = rect->x;
	r.y = rect->y;
	if (mid > 0) {
		w_graphics_set_background(gc, c2);
		w_graphics_set_foreground(gc, c1);
		if (vertical) {
			r.width = rect->width;
			r.height = mid;
		} else {
			r.width = mid;
			r.height = rect->height;
		}
		w_graphics_fill_gradientrectangle(gc, &r, vertical);
	}
	if (mid2 > 0) {
		w_graphics_set_foreground(gc, c2);
		if (vertical) {
			r.x = rect->x;
			r.y = rect->y + mid;
			r.width = rect->width;
			r.height = mid2;
		} else {
			r.x = rect->x + mid;
			r.y = rect->y;
			r.width = mid2;
			r.height = rect->height;
		}
		w_graphics_fill_rectangle(gc, &r);
	}
	if (mid > 0) {
		w_graphics_set_background(gc, c1);
		w_graphics_set_foreground(gc, c2);
		if (vertical) {
			r.x = rect->x;
			r.y = rect->y + mid + mid2;
			r.width = rect->width;
			r.height = mid;
		} else {
			r.x = rect->x + mid + mid2;
			r.y = rect->y;
			r.width = mid;
			r.height = rect->height;
		}
		w_graphics_fill_gradientrectangle(gc, &r, vertical);
	}
	if (wh - mid * 2 - mid2 > 0) {
		w_graphics_set_background(gc, c3);
		w_graphics_set_foreground(gc, c1);
		if (vertical) {
			r.x = rect->x;
			r.y = rect->y + mid * 2 + mid2;
			r.width = rect->width;
			r.height = rect->height - mid * 2 - mid2;
		} else {
			r.x = rect->x + mid * 2 + mid2;
			r.y = rect->y;
			r.width = rect->width - mid * 2 - mid2;
			r.height = rect->height;
		}
		w_graphics_fill_gradientrectangle(gc, &r, vertical);
	}
}
void metal_button_draw_background_push(w_theme *theme,w_themedata *data, w_graphics *gc,
                                  w_rect *bounds) {
    if (data->state & W_PRESSED) {
        w_graphics_set_background(gc, ((w_theme_metal*) theme)->primary2);
        w_graphics_fill_rectangle(gc, bounds);
        return;
    }
    w_theme_metal_draw_gradient(theme, gc, bounds, W_TRUE);
    w_theme_metal_draw_flush_3d_border(theme, gc, bounds);
}
void metal_button_draw_background_radio(w_theme *theme,w_themedata *data, w_graphics *gc,
                                       w_rect *bounds) {
    w_rect r;
    w_point p1,p2;
    if(data->style & W_CHECK) {
        r.x = bounds->x + 3;
        r.y = bounds->y + 5;
        r.width = 2;
        r.height = 13 - 8;
        w_graphics_fill_rectangle(gc, &r);
        p1.x = bounds->x + (13 - 4);
        p1.y = bounds->y + 3;
        p2.x = bounds->x + 5;
        p2.y = bounds->y + (13 - 6);
        w_graphics_draw_line(gc, &p1, &p2);
        p1.x = bounds->x + (13 - 4);
        p1.y = bounds->y + 4;
        p2.x = bounds->x + 5;
        p2.y = bounds->y + (13 - 5);
        w_graphics_draw_line(gc, &p1, &p2);
    }else{
        r.x = bounds->x + 1;
        r.y = bounds->y + 1;
        r.width = 6;
        r.height = 6;
        w_graphics_fill_oval(gc,&r);
    }
}
void metal_button_draw_background_arrow(w_theme *theme,w_themedata *data, w_graphics *gc,
                                       w_rect *bounds) {
    int width = 4;
    int height = 8;
    w_point pt[3];
    pt[0].x = bounds->x;
    pt[0].y = bounds->y;
    pt[1].x = bounds->x + width;
    pt[1].y = bounds->y + (height/2);
    pt[2].x = bounds->x;
    pt[2].y = bounds->y + height;
    w_graphics_fill_polygon(gc,pt,3);
}
void metal_button_draw_background(w_theme *theme,w_themedata *data, w_graphics *gc, w_rect *bounds) {
    if(data->style & W_CHECK || data->style & W_RADIO){
        metal_button_draw_background_radio(theme,data,gc,bounds);
    }else if(data->style & W_ARROW){
        metal_button_draw_background_arrow(theme,data,gc,bounds);
    }else{
        metal_button_draw_background_push(theme,data,gc,bounds);
    }
}
/*
 *
 */
typedef void (*draw_background)(w_theme *theme,w_themedata *data, w_graphics *gc, w_rect *bounds);
typedef struct clazz_info {
    draw_background drawBackground;
}clazz_info;
clazz_info metal_clazz_info[] = {
        {},//_W_THEME_CLASS_UNKNOWN =0,
        {},//_W_THEME_CLASS_MENU,
        {metal_button_draw_background},//_W_THEME_CLASS_BUTTON,
        {},//_W_THEME_CLASS_LABEL,
        {},//_W_THEME_CLASS_LINK,
        {},//_W_THEME_CLASS_PROGRESSBAR,
        {},//_W_THEME_CLASS_SASH,
        {},//_W_THEME_CLASS_SCALE,
        {},//_W_THEME_CLASS_SLIDER,
        {},// _W_THEME_CLASS_SCROLLBAR,
        {},//_W_THEME_CLASS_TEXT,
        {},//_W_THEME_CLASS_LIST,
        {},//_W_THEME_CLASS_COMPOSITE,
        {},//_W_THEME_CLASS_BROWSER,
        {},//_W_THEME_CLASS_TREE,
        {},//_W_THEME_CLASS_TABFOLDER,
        {},//_W_THEME_CLASS_TABITEM,
        {},//_W_THEME_CLASS_COMBO,
        {},//_W_THEME_CLASS_COOLBAR,
        {},//_W_THEME_CLASS_DATETIME,
        {},// _W_THEME_CLASS_EXPANDBAR,
        {},// _W_THEME_CLASS_GROUP,
        {},// _W_THEME_CLASS_SPINNER,
        {},// _W_THEME_CLASS_TABLE,
        {},// _W_THEME_CLASS_TOOLBAR,
        {},// _W_THEME_CLASS_TOOLITEM,
};
void w_theme_metal_dispose(w_theme *theme){
    free(theme);
}
const char* w_theme_metal_get_name(w_theme *theme){
    return "metal";
}
void w_theme_metal_compute_trim(w_theme *theme,w_themedata *data,w_graphics *gc, w_rect *result){

}
void w_theme_metal_draw_background(w_theme *theme,w_themedata *data, w_graphics *gc, w_rect *bounds){
    wuint clazz = w_theme_internal_class_id(data->clazz);
    draw_background drawBackground = metal_clazz_info[clazz].drawBackground;
    if(drawBackground != 0){
        drawBackground(theme,data,gc,bounds);
    }
}
void w_theme_metal_draw_focus(w_theme *theme,w_themedata *data, w_graphics *gc, w_rect *bounds){

}
void w_theme_metal_draw_image(w_theme *theme,w_themedata *data, w_graphics *gc, w_rect *bounds,
                              w_image *image, int flags){

}
void w_theme_metal_draw_text(w_theme *theme,w_themedata *data, w_graphics *gc, w_rect *bounds, const char *text, int length, int flags){
    w_size sz;
    w_rect r;
    //w_graphics_set_font(gc,((w_theme_metal*)data->theme)->font);
    w_graphics_text_extent(gc, text, length, &sz,
                           W_DRAW_DELIMITER | W_DRAW_TAB);
    int padding = 0;
    if(data->style & W_CHECK || data->style & W_RADIO){
        padding = 9;
    }
    r.x = (bounds->width - sz.width) / 2 + padding;
    r.y = (bounds->height - sz.height) / 2;
    r.width = -1;
    r.height = -1;
    w_graphics_set_foreground(gc, W_COLOR_BLACK);
    w_graphics_draw_text(gc, text, length, &r,
                         W_DRAW_DELIMITER | W_DRAW_TAB | W_DRAW_TRANSPARENT);
}
void w_theme_metal_get_bounds(w_theme *theme,w_themedata *data, int part,
                              w_rect *bounds, w_rect *result){

}
int w_theme_metal_get_selection(w_theme *theme,w_themedata *data, w_point *offset, w_rect *bounds){

}
int w_theme_metal_hit_background(w_theme *theme,w_themedata *data, w_point *position, w_rect *bounds){

}
void w_theme_metal_measure_text(w_theme *theme,w_themedata *data,w_graphics *gc, w_rect *bounds, w_rect *result,const char *text, size_t length, int flags){
    //w_graphics_set_font(gc,((w_theme_metal*)data->theme)->font);
    w_graphics_text_extent(gc,text,length,&result->sz,W_DRAW_DELIMITER | W_DRAW_TAB);
}
w_color  w_theme_metal_get_color(w_theme *theme,wuint colorid){

}
w_font* w_theme_metal_get_font(w_theme *theme){
    return ((w_theme_metal*)theme)->font;
}
w_cursor* w_theme_metal_get_cursor(w_theme *theme, wuint id){
    return w_toolkit_get_system_cursor(w_toolkit_get_platform(),id);
}
w_image* w_theme_metal_get_image(w_theme *theme, wuint id){
    return 0;// w_toolkit_get_system_image(w_toolkit_get_platform(),id);
}
struct _w_theme_class metal_clazz = { //
        w_theme_metal_dispose,
        w_theme_metal_get_name,
        w_theme_metal_compute_trim,
        w_theme_metal_draw_background,
        w_theme_metal_draw_focus,
        w_theme_metal_draw_image,
        w_theme_metal_draw_text,
        w_theme_metal_get_bounds,
        w_theme_metal_get_selection,
        w_theme_metal_hit_background,
        w_theme_metal_measure_text,
        w_theme_metal_get_color,
        w_theme_metal_get_font,
        w_theme_metal_get_cursor,
        w_theme_metal_get_image
		};//
w_theme* w_theme_metal_create() {
	w_theme_metal *metal = malloc(sizeof(w_theme_metal));
	if (metal == 0)
		return 0;
	metal->font = w_toolkit_get_system_font(w_toolkit_get_platform());
	metal->clazz = &metal_clazz;
	metal->ratio1 = 0.3;
	metal->ratio2 = 0;
	metal->primary1 = 0xFFBF8263;
	metal->primary2 = 0xFFCCB8A3;
	metal->primary3 = 0xFFE5CFB8;
	metal->secondary1 = 0xFF998A7A;
	metal->secondary2 = 0xFFE5CFB8;
	metal->secondary3 = 0xFFEEEEEE;
	metal->gradiant = 0xFFF3E8DD;
	return (w_theme*) metal;
}

