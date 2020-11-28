/*
 * tabfolderrenderer.c
 *
 *  Created on: 1 sept. 2020
 *      Author: azeddine
 */
#include "tabfolder.h"
#define BUTTON_SIZE  16
#define BUTTON_TRIM  1

#define ITEM_TOP_MARGIN  2
#define ITEM_BOTTOM_MARGIN  2
#define ITEM_LEFT_MARGIN  4
#define ITEM_RIGHT_MARGIN  4
#define INTERNAL_SPACING  4
#define FLAGS  (W_DRAW_TRANSPARENT | W_DRAW_MNEMONIC)
char TOP_LEFT_CORNER_HILITE[] = { 5, 2, 4, 2, 3, 3, 2, 4, 2, 5, 1, 6 };

char TOP_LEFT_CORNER[] = { 0, 6, 1, 5, 1, 4, 4, 1, 5, 1, 6, 0 };
char TOP_RIGHT_CORNER[] = { -6, 0, -5, 1, -4, 1, -1, 4, -1, 5, 0, 6 };
char BOTTOM_LEFT_CORNER[] = { 0, -6, 1, -5, 1, -4, 4, -1, 5, -1, 6, 0 };
char BOTTOM_RIGHT_CORNER[] = { -6, 0, -5, -1, -4, -1, -1, -4, -1, -5, 0, -6 };

char SIMPLE_TOP_LEFT_CORNER[] = { 0, 2, 1, 1, 2, 0 };
char SIMPLE_TOP_RIGHT_CORNER[] = { -2, 0, -1, 1, 0, 2 };
char SIMPLE_BOTTOM_LEFT_CORNER[] = { 0, -2, 1, -1, 2, 0 };
char SIMPLE_BOTTOM_RIGHT_CORNER[] = { -2, 0, -1, -1, 0, -2 };
char SIMPLE_UNSELECTED_INNER_CORNER[] = { 0, 0 };

char TOP_LEFT_CORNER_BORDERLESS[] = { 0, 6, 1, 5, 1, 4, 4, 1, 5, 1, 6, 0 };
char TOP_RIGHT_CORNER_BORDERLESS[] =
		{ -7, 0, -6, 1, -5, 1, -2, 4, -2, 5, -1, 6 };
char BOTTOM_LEFT_CORNER_BORDERLESS[] = { 0, -6, 1, -6, 1, -5, 2, -4, 4, -2, 5,
		-1, 6, -1, 6, 0 };
char BOTTOM_RIGHT_CORNER_BORDERLESS[] = { -7, 0, -7, -1, -6, -1, -5, -2, -3, -4,
		-2, -5, -2, -6, -1, -6 };

char SIMPLE_TOP_LEFT_CORNER_BORDERLESS[] = { 0, 2, 1, 1, 2, 0 };
char SIMPLE_TOP_RIGHT_CORNER_BORDERLESS[] = { -3, 0, -2, 1, -1, 2 };
char SIMPLE_BOTTOM_LEFT_CORNER_BORDERLESS[] = { 0, -3, 1, -2, 2, -1, 3, 0 };
char SIMPLE_BOTTOM_RIGHT_CORNER_BORDERLESS[] = { -4, 0, -3, -1, -2, -2, -1, -3 };
char CURVE_BOTTOM[] = { 0, 13, 0, 12, 2, 12, 3, 11, 5, 11, 6, 10, 7, 10, 9, 8,
		10, 8, 11, 7, 11, 7, 12, 6, 13, 6, 15, 4, 16, 4, 17, 3, 19, 3, 20, 2,
		22, 2, 23, 1 };
char CURVE_NOT_BOTTOM[] = { 0, 0, 0, 1, 2, 1, 3, 2, 5, 2, 6, 3, 7, 3, 9, 5, 10,
		5, 11, 6, 11, 6, 12, 7, 13, 7, 15, 9, 16, 9, 17, 10, 19, 10, 20, 11, 22,
		11, 23, 12 };

#define BUTTON_BORDER  W_COLOR_WIDGET_DARK_SHADOW
#define BUTTON_FILL  W_COLOR_LIST_BACKGROUND
#define BORDER1_COLOR W_COLOR_WIDGET_NORMAL_SHADOW

#define CLOSE_FILL W_RGB(252, 160, 160)
w_color COLOR_BUTTON_BORDER(w_toolkit *toolkit) {
	return w_toolkit_get_system_color(toolkit, BUTTON_BORDER);
	//return W_COLOR_DARK_MAGENTA;
}
w_color COLOR_BUTTON_FILL(w_toolkit *toolkit) {
	return w_toolkit_get_system_color(toolkit, BUTTON_FILL);
	//return W_COLOR_DARK_MAGENTA;
}
w_color COLOR_BORDER1_COLOR(w_toolkit *toolkit) {
	return w_toolkit_get_system_color(toolkit, BORDER1_COLOR);
	//return W_COLOR_BLUE;
}
/*
 * PlatformRenderer
 */
void wc_tabfolder_renderer_init(w_tabfolder *tabfolder) {
}

void wc_tabfolder_renderer_dispose(w_tabfolder *tabfolder) {
}

void wc_tabfolder_renderer_compute_trim(w_tabfolder *parent, wc_tabitem *item,
		int part, int state, w_rect *result, const w_rect *r) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	int style = w_widget_get_style(W_WIDGET(parent));
	int borderLeft = priv->borderVisible ? 1 : 0;
	int borderRight = borderLeft;
	int borderTop = (style & W_BOTTOM) ? borderLeft : 0;
	int borderBottom = (style & W_BOTTOM) ? 0 : borderLeft;
	int tabHeight = priv->tabHeight;
	memcpy(&result, &r, sizeof(w_rect));
	if (item == 0) {
		switch (part) {
		case WC_TABFOLDER_PART_BODY: {
			int highlight_header = (style & W_FLAT) != 0 ? 1 : 3;
			int highlight_margin = (style & W_FLAT) != 0 ? 0 : 2;
			if (priv->fixedTabHeight == 0 && (style & W_FLAT) != 0
					&& (style & W_BORDER) == 0) {
				highlight_header = 0;
			}
			int marginWidth = priv->marginWidth;
			int marginHeight = priv->marginHeight;
			result->x = result->x - marginWidth - highlight_margin - borderLeft;
			result->width = result->width + borderLeft + borderRight
					+ 2 * marginWidth + 2 * highlight_margin;
			if (priv->minimized) {
				result->y =
						(style & W_BOTTOM) ?
								result->y - borderTop :
								result->y - highlight_header - tabHeight
										- borderTop;
				result->height = borderTop + borderBottom + tabHeight
						+ highlight_header;
			} else {
				result->y =
						(style & W_BOTTOM) ?
								result->y - marginHeight - highlight_margin
										- borderTop :
								result->y - marginHeight - highlight_header
										- tabHeight - borderTop;
				result->height = result->height + borderTop + borderBottom
						+ 2 * marginHeight + tabHeight + highlight_header
						+ highlight_margin;
			}
		}
			break;
		case WC_TABFOLDER_PART_HEADER:
			//no trim
			break;
		case WC_TABFOLDER_PART_MAX_BUTTON:
		case WC_TABFOLDER_PART_MIN_BUTTON:
		case WC_TABFOLDER_PART_CLOSE_BUTTON:
		case WC_TABFOLDER_PART_CHEVRON_BUTTON:
			result->x -= BUTTON_TRIM;
			result->y -= BUTTON_TRIM;
			result->width += BUTTON_TRIM * 2;
			result->height += BUTTON_TRIM * 2;
			break;
		case WC_TABFOLDER_PART_BORDER:
			result->x = result->x - borderLeft;
			result->width = result->width + borderLeft + borderRight;
			if (!priv->simple)
				result->width += 2; // TOP_RIGHT_CORNER needs more space
			result->y = result->y - borderTop;
			result->height = result->height + borderTop + borderBottom;
			break;
		}
	} else {
		result->x = result->x - ITEM_LEFT_MARGIN;
		result->width = result->width + ITEM_LEFT_MARGIN + ITEM_RIGHT_MARGIN;
		result->y = result->y - ITEM_TOP_MARGIN;
		result->height = result->height + ITEM_TOP_MARGIN + ITEM_BOTTOM_MARGIN;
	}
}
void wc_tabfolder_renderer_compute_size(w_tabfolder *parent, wc_tabitem *item,
		int part, int state, w_graphics *gc, w_size *result, int wHint,
		int hHint) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	w_size tmp;
	memset(result, 0, sizeof(w_size));
	if (item == 0) {
		switch (part) {
		case WC_TABFOLDER_PART_HEADER:
			if (priv->fixedTabHeight != W_DEFAULT) {
				result->height =
						priv->fixedTabHeight == 0 ?
								0 : priv->fixedTabHeight + 1; // +1 for line drawn across top of tab
			} else {
				if (priv->items == 0) {
					w_graphics_text_extent(gc, "Default", -1, result, FLAGS);
					result->height += ITEM_TOP_MARGIN + ITEM_BOTTOM_MARGIN;
				} else {
					wc_tabitem *i = priv->items;
					while (i != 0) {
						wc_tabfolder_renderer_compute_size(parent, i, 0, W_NONE,
								gc, &tmp, wHint, hHint);
						result->height = w_max(result->height, tmp.height);
						i = i->next;
					}

				}
			}
			break;
		case WC_TABFOLDER_PART_MAX_BUTTON:
		case WC_TABFOLDER_PART_MIN_BUTTON:
		case WC_TABFOLDER_PART_CLOSE_BUTTON:
			result->width = result->height = BUTTON_SIZE;
			break;
		case WC_TABFOLDER_PART_CHEVRON_BUTTON:
			result->width = 3 * BUTTON_SIZE / 2;
			result->height = BUTTON_SIZE;
			break;
		}
	} else {
		w_imagelist *imagelist = priv->imagelist;
		if (w_imagelist_is_ok(imagelist)) {
			w_imagelist_get_size(imagelist, result);
			if ((state & W_SELECTED) == 0 && !priv->showUnselectedImage) {
				result->width = 0;
			}
		}
		const char *text = 0;
		size_t text_length = -1;
		if ((state & WC_TABFOLDER_MINIMUM_SIZE) != 0) {
			text = item->text;
			/*int minChars = parent.minChars;
			 text = minChars == 0 ? null : item.getText();
			 if (text != null && text.length() > minChars) {
			 if (useEllipses()) {
			 int end =
			 minChars < ELLIPSIS.length() + 1 ?
			 minChars : minChars - ELLIPSIS.length();
			 text = text.substring(0, end);
			 if (minChars > ELLIPSIS.length() + 1)
			 text += ELLIPSIS;
			 } else {
			 int end = minChars;
			 text = text.substring(0, end);
			 }
			 }*/
		} else {
			text = item->text;
		}
		if (text != 0) {
			if (result->width > 0)
				result->width += INTERNAL_SPACING;
			if (item->font == 0) {
				w_graphics_text_extent(gc, text, text_length, &tmp, FLAGS);
				result->width += tmp.width;
				result->height = w_max(result->height, tmp.height);
			} else {
				w_font *gcFont = w_graphics_get_font(gc);
				w_graphics_set_font(gc, item->font);
				w_graphics_text_extent(gc, text, text_length, &tmp, FLAGS);
				result->width += tmp.width;
				result->height = w_max(result->height, tmp.height);
				w_graphics_set_font(gc, gcFont);
			}
		}
		int style = w_widget_get_style(W_WIDGET(parent));
		if ((style & W_CLOSE) || item->showClose) {
			if ((state & W_SELECTED) != 0 || priv->showUnselectedClose) {
				if (result->width > 0)
					result->width += INTERNAL_SPACING;
				wc_tabfolder_renderer_compute_size(parent, 0,
						WC_TABFOLDER_PART_CLOSE_BUTTON, W_NONE, gc, &tmp,
						W_DEFAULT, W_DEFAULT);
				result->width += tmp.width;
			}
		}
	}
	w_rect trim, r;
	r.x = 0;
	r.y = 0;
	r.width = result->width;
	r.height = result->height;
	wc_tabfolder_renderer_compute_trim(parent, item, part, state, &trim, &r);
	result->width = trim.width;
	result->height = trim.height;
}

void wc_tabfolder_renderer_draw(w_tabfolder *parent, wc_tabitem *item, int part,
		int state, w_rect *bounds, w_graphics *gc) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	int style = w_widget_get_style(W_WIDGET(parent));
	w_toolkit *toolkit = w_widget_get_toolkit(W_WIDGET(parent));
	w_theme *theme = w_toolkit_get_theme(toolkit);
	w_rect trim, r;
	w_themedata data;
	if (item != 0) {
		memset(&data, 0, sizeof(data));
		data.clazz = W_THEME_CLASS_TABITEM;
		data.state = state;
		data.style = style;
		w_theme_draw_background(theme, &data, gc, bounds);
		memset(&r, 0, sizeof(r));
		wc_tabfolder_renderer_compute_trim(parent, item, 0, W_NONE, &trim, &r);
		trim.x = bounds->x - trim.x;
		trim.y = bounds->y;
		trim.width = bounds->width;
		trim.height = bounds->height;
		w_theme_draw_text(theme, &data, gc, bounds, item->text, -1,
				W_THEME_DRAW_LEFT | W_THEME_DRAW_VCENTER);
		if ((style & W_CLOSE) || item->showClose) {
			/*WRect closeRect;
			 item->GetCloseRect(gc, closeRect);
			 CustomRendererDrawClose(parent, gc, closeRect,
			 item->closeImageState);*/
		}
	} else {
		data.clazz = W_THEME_CLASS_TABFOLDER;
		switch (part) {
		case WC_TABFOLDER_PART_BACKGROUND:
			w_theme_draw_background(theme, &data, gc, bounds);
			//this.drawBackground(gc, bounds, state);
			break;
		case WC_TABFOLDER_PART_BODY:
			/*gc.SetForeground(toolkit->GetSystemColor(W_COLOR_WIDGET_NORMAL_SHADOW));
			 gc.DrawRectangle(bounds);*/
			data.state = state;
			data.style = style;
			w_theme_draw_background(theme, &data, gc, bounds);
			//drawBody(gc, bounds, state);
			break;
		case WC_TABFOLDER_PART_HEADER:
			//drawTabArea(gc, bounds, state);
			break;
		case WC_TABFOLDER_PART_MAX_BUTTON:
			//drawMaximize(gc, bounds, state);
			break;
		case WC_TABFOLDER_PART_MIN_BUTTON:
			//drawMinimize(gc, bounds, state);
			break;
		case WC_TABFOLDER_PART_CHEVRON_BUTTON:
			//drawChevron(gc, bounds, state);
			break;
		}
	}
}

struct wc_tabfolder_renderer default_renderer = {	//
		wc_tabfolder_renderer_init,	//
				wc_tabfolder_renderer_dispose,	//
				wc_tabfolder_renderer_compute_size,	//
				wc_tabfolder_renderer_compute_trim,	//
				wc_tabfolder_renderer_draw	//
		};//
/*
 *
 */
void wc_tabfolder_custom_init(w_tabfolder *parent) {
}
void wc_tabfolder_custom_dispose(w_tabfolder *parent) {

}
int wc_tabfolder_custom__curve_width(w_tabfolder *parent) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	return priv->tabHeight + 26 - 12;
}
void wc_tabfolder_custom_compute_trim(w_tabfolder *parent, wc_tabitem *item,
		int part, int state, w_rect *result, const w_rect *r) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	int style = w_widget_get_style(W_WIDGET(parent));
	int borderLeft = priv->borderVisible ? 1 : 0;
	int borderRight = borderLeft;
	int borderTop = (style & W_BOTTOM) ? borderLeft : 0;
	int borderBottom = (style & W_BOTTOM) ? 0 : borderLeft;
	int tabHeight = priv->tabHeight;
	memcpy(result, r, sizeof(w_rect));
	if (item == 0) {
		switch (part) {
		case WC_TABFOLDER_PART_BODY: {
			int highlight_header = (style & W_FLAT) != 0 ? 1 : 3;
			int highlight_margin = (style & W_FLAT) != 0 ? 0 : 2;
			if (priv->fixedTabHeight == 0 && (style & W_FLAT) != 0
					&& (style & W_BORDER) == 0) {
				highlight_header = 0;
			}
			int marginWidth = priv->marginWidth;
			int marginHeight = priv->marginHeight;
			result->x = result->x - marginWidth - highlight_margin - borderLeft;
			result->width = result->width + borderLeft + borderRight
					+ 2 * marginWidth + 2 * highlight_margin;
			if (priv->minimized) {
				result->y =
						(style & W_BOTTOM) ?
								result->y - borderTop :
								result->y - highlight_header - tabHeight
										- borderTop;
				result->height = borderTop + borderBottom + tabHeight
						+ highlight_header;
			} else {
				result->y =
						(style & W_BOTTOM) ?
								result->y - marginHeight - highlight_margin
										- borderTop :
								result->y - marginHeight - highlight_header
										- tabHeight - borderTop;
				result->height = result->height + borderTop + borderBottom
						+ 2 * marginHeight + tabHeight + highlight_header
						+ highlight_margin;
			}
		}
			break;
		case WC_TABFOLDER_PART_HEADER:
			//no trim
			break;
		case WC_TABFOLDER_PART_MAX_BUTTON:
		case WC_TABFOLDER_PART_MIN_BUTTON:
		case WC_TABFOLDER_PART_CLOSE_BUTTON:
		case WC_TABFOLDER_PART_CHEVRON_BUTTON:
			result->x -= BUTTON_TRIM;
			result->y -= BUTTON_TRIM;
			result->width += BUTTON_TRIM * 2;
			result->height += BUTTON_TRIM * 2;
			break;
		case WC_TABFOLDER_PART_BORDER:
			result->x = result->x - borderLeft;
			result->width = result->width + borderLeft + borderRight;
			if (!priv->simple)
				result->width += 2; // TOP_RIGHT_CORNER needs more space
			result->y = result->y - borderTop;
			result->height = result->height + borderTop + borderBottom;
			break;
		}
	} else {
		//updateCurves();
		result->x = result->x - ITEM_LEFT_MARGIN;
		result->width = result->width + ITEM_LEFT_MARGIN + ITEM_RIGHT_MARGIN;
		if (!priv->simple && !priv->single && !priv->platform
				&& (state & W_SELECTED) != 0) {
			int curveWidth = wc_tabfolder_custom__curve_width(parent);
			int curveIndent = curveWidth / 3;
			result->width += curveWidth - curveIndent;
		}
		result->y = result->y - ITEM_TOP_MARGIN;
		result->height = result->height + ITEM_TOP_MARGIN + ITEM_BOTTOM_MARGIN;
	}
}
void wc_tabfolder_custom_compute_size(w_tabfolder *parent, wc_tabitem *item,
		int part, int state, w_graphics *gc, w_size *result, int wHint,
		int hHint) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	w_size tmp;
	result->width = result->height = 0;
	if (item == 0) {
		switch (part) {
		case WC_TABFOLDER_PART_HEADER:
			if (priv->fixedTabHeight != W_DEFAULT) {
				result->height =
						priv->fixedTabHeight == 0 ?
								0 : priv->fixedTabHeight + 1; // +1 for line drawn across top of tab
			} else {
				if (priv->items == 0) {
					w_graphics_text_extent(gc, "Default", -1, result, FLAGS);
					result->height += ITEM_TOP_MARGIN + ITEM_BOTTOM_MARGIN;
				} else {
					wc_tabitem *i = priv->items;
					while (i != 0) {
						wc_tabfolder_custom_compute_size(parent, i, 0, W_NONE,
								gc, &tmp, wHint, hHint);
						result->height = w_max(result->height, tmp.height);
						i = i->next;
					}

				}
			}
			break;
		case WC_TABFOLDER_PART_MAX_BUTTON:
		case WC_TABFOLDER_PART_MIN_BUTTON:
		case WC_TABFOLDER_PART_CLOSE_BUTTON:
			result->width = result->height = BUTTON_SIZE;
			break;
		case WC_TABFOLDER_PART_CHEVRON_BUTTON:
			result->width = 3 * BUTTON_SIZE / 2;
			result->height = BUTTON_SIZE;
			break;
		}
	} else {
		w_imagelist *imagelist = priv->imagelist;
		if (imagelist != 0 && w_imagelist_is_ok(imagelist)) {
			w_imagelist_get_size(imagelist, result);
			if ((state & W_SELECTED) == 0 && !priv->showUnselectedImage) {
				result->width = 0;
			}
		}
		const char *text = 0;
		if ((state & WC_TABFOLDER_MINIMUM_SIZE) != 0) {
			text = item->text;
			/*int minChars = parent.minChars;
			 text = minChars == 0 ? null : item.getText();
			 if (text != null && text.length() > minChars) {
			 if (useEllipses()) {
			 int end =
			 minChars < ELLIPSIS.length() + 1 ?
			 minChars : minChars - ELLIPSIS.length();
			 text = text.substring(0, end);
			 if (minChars > ELLIPSIS.length() + 1)
			 text += ELLIPSIS;
			 } else {
			 int end = minChars;
			 text = text.substring(0, end);
			 }
			 }*/
		} else {
			text = item->text;
		}
		if (text != 0) {
			if (result->width > 0)
				result->width += INTERNAL_SPACING;
			if (item->font == 0) {
				w_graphics_text_extent(gc, text, -1, &tmp, FLAGS);
				result->width += tmp.width;
				result->height = w_max(result->height, tmp.height);
			} else {
				w_font *gcFont = w_graphics_get_font(gc);
				w_graphics_set_font(gc, item->font);
				w_graphics_text_extent(gc, text, -1, &tmp, FLAGS);
				result->width += tmp.width;
				result->height = w_max(result->height, tmp.height);
				w_graphics_set_font(gc, gcFont);
			}
		}
		int style = w_widget_get_style(W_WIDGET(parent));
		if ((style & W_CLOSE) || item->showClose) {
			if ((state & W_SELECTED) != 0 || priv->showUnselectedClose) {
				if (result->width > 0)
					result->width += INTERNAL_SPACING;
				wc_tabfolder_custom_compute_size(parent, 0,
						WC_TABFOLDER_PART_CLOSE_BUTTON, W_NONE, gc, &tmp,
						W_DEFAULT, W_DEFAULT);
				result->width += tmp.width;
			}
		}
	}
	w_rect trim, r;
	r.x = r.y = 0;
	r.width = result->width;
	r.height = result->height;
	wc_tabfolder_custom_compute_trim(parent, item, part, state, &trim, &r);
	result->width = trim.width;
	result->height = trim.height;
}

void wc_tabfolder_custom_draw_right_unselected_border(w_tabfolder *parent,
		int state, w_rect *bounds, w_graphics *gc) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	int style = w_widget_get_style(W_WIDGET(parent));
	int x = bounds->x;
	int y = bounds->y;
	int width = bounds->width;
	int height = bounds->height;

	int shape[20];
	int shape_size;
	char *right;
	int startX = x + width - 1;

	if (style & W_BOTTOM) {
		if (priv->simple) {
			right = SIMPLE_UNSELECTED_INNER_CORNER;
			shape_size = sizeof(SIMPLE_UNSELECTED_INNER_CORNER) / 2;
		} else {
			right = BOTTOM_RIGHT_CORNER;
			shape_size = sizeof(BOTTOM_RIGHT_CORNER) / 2;
		}
		int index = 0;

		for (int i = 0; i < shape_size; i++) {
			shape[index++] = startX + right[2 * i];
			shape[index++] = y + height + right[2 * i + 1] - 1;
		}
		shape[index++] = startX;
		shape[index++] = y - 1;
	} else {
		if (priv->simple) {
			right = SIMPLE_UNSELECTED_INNER_CORNER;
			shape_size = sizeof(SIMPLE_UNSELECTED_INNER_CORNER) / 2;
		} else {
			right = TOP_RIGHT_CORNER;
			shape_size = sizeof(TOP_RIGHT_CORNER) / 2;
		}
		int index = 0;

		for (int i = 0; i < shape_size; i++) {
			shape[index++] = startX + right[2 * i];
			shape[index++] = y + right[2 * i + 1];
		}

		shape[index++] = startX;
		shape[index++] = y + height;

	}
	w_graphics_set_foreground(gc,
			COLOR_BORDER1_COLOR(w_widget_get_toolkit(W_WIDGET(parent))));
	w_graphics_draw_polyline(gc, (w_point*) shape, shape_size + 1);
}

void wc_tabfolder_custom_draw_background_0(w_tabfolder *parent, w_graphics *gc,
		const w_rect *bounds, w_point *shape, int shape_count,
		w_color defaultBackground, w_image *image, int selected) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	/*WRegion clipping, region;
	 if (shape != 0) {
	 gc.GetClipping(clipping);
	 region.Create();
	 region.Add(shape, shape_count);
	 //region.Intersect(clipping);
	 gc.SetClipping(region);
	 }*/
	w_color *colors = 0;
	/*if (priv->gradientBackground != 0) {
	 if (selected) {
	 colors =
	 priv->gradientBackground->selectionGradientColors;
	 } else {
	 colors = priv->gradientBackground->gradientColors;
	 }
	 }*/
	if (image != 0 && w_image_is_ok(image)) {
		// draw the background image in shape
		/*gc.setBackground(defaultBackground);
		 gc.fillRectangle(x, y, width, height);
		 Rectangle imageRect = image.getBounds();
		 gc.drawImage(image, imageRect.x, imageRect.y, imageRect.width, imageRect.height, x, y, width, height);*/
	} else if (colors != 0) {
		// draw gradient
		/*if (colors.length == 1) {
		 Color background = colors[0] != null ? colors[0] : defaultBackground;
		 gc.setBackground(background);
		 gc.fillRectangle(x, y, width, height);
		 } else {
		 if (vertical) {
		 if (parent.onBottom) {
		 int pos = 0;
		 if (percents[percents.length - 1] < 100) {
		 pos = (100 - percents[percents.length - 1]) * height / 100;
		 gc.setBackground(defaultBackground);
		 gc.fillRectangle(x, y, width, pos);
		 }
		 Color lastColor = colors[colors.length-1];
		 if (lastColor == null) lastColor = defaultBackground;
		 for (int i = percents.length-1; i >= 0; i--) {
		 gc.setForeground(lastColor);
		 lastColor = colors[i];
		 if (lastColor == null) lastColor = defaultBackground;
		 gc.setBackground(lastColor);
		 int percentage = i > 0 ? percents[i] - percents[i-1] : percents[i];
		 int gradientHeight = percentage * height / 100;
		 gc.fillGradientRectangle(x, y+pos, width, gradientHeight, true);
		 pos += gradientHeight;
		 }
		 } else {
		 Color lastColor = colors[0];
		 if (lastColor == null) lastColor = defaultBackground;
		 int pos = 0;
		 for (int i = 0; i < percents.length; i++) {
		 gc.setForeground(lastColor);
		 lastColor = colors[i + 1];
		 if (lastColor == null) lastColor = defaultBackground;
		 gc.setBackground(lastColor);
		 int percentage = i > 0 ? percents[i] - percents[i-1] : percents[i];
		 int gradientHeight = percentage * height / 100;
		 gc.fillGradientRectangle(x, y+pos, width, gradientHeight, true);
		 pos += gradientHeight;
		 }
		 if (pos < height) {
		 gc.setBackground(defaultBackground);
		 gc.fillRectangle(x, pos, width, height-pos+1);
		 }
		 }
		 } else { //horizontal gradient
		 y = 0;
		 height = parent.getSize().y;
		 Color lastColor = colors[0];
		 if (lastColor == null) lastColor = defaultBackground;
		 int pos = 0;
		 for (int i = 0; i < percents.length; ++i) {
		 gc.setForeground(lastColor);
		 lastColor = colors[i + 1];
		 if (lastColor == null) lastColor = defaultBackground;
		 gc.setBackground(lastColor);
		 int gradientWidth = (percents[i] * width / 100) - pos;
		 gc.fillGradientRectangle(x+pos, y, gradientWidth, height, false);
		 pos += gradientWidth;
		 }
		 if (pos < width) {
		 gc.setBackground(defaultBackground);
		 gc.fillRectangle(x+pos, y, width-pos, height);
		 }
		 }
		 }*/
	} else {
		// draw a solid background using default background in shape
		/*if ((parent->GetStyle() & W_NO_BACKGROUND) != 0
		 || defaultBackground != (parent->GetBackground())) {*/
		w_graphics_set_background(gc, defaultBackground);
		if (shape != 0) {
			w_graphics_fill_polygon(gc, shape, shape_count);
		} else {
			w_graphics_fill_rectangle(gc, (w_rect*) bounds);
		}

		//}
	}
	/*if (shape != 0) {
	 gc.SetClipping(clipping);
	 }*/
}

void wc_tabfolder_custom_draw_background_1(w_tabfolder *parent, w_graphics *gc,
		const w_rect *bounds, w_point *shape, int shape_count, int selected) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	int style = w_widget_get_style(W_WIDGET(parent));
	w_color bg;
	w_control_get_background(W_CONTROL(parent), &bg);
	w_color defaultBackground = selected ? priv->selectionBackground : bg;
	w_image *image = 0; //selected ? parent.selectionBgImage : null;
	w_color *colors = 0; //selected ? parent.selectionGradientColors : parent.gradientColors;
	int *percents = 0; //selected ? parent.selectionGradientPercents : parent.gradientPercents;
	int vertical = 0; // selected ? parent.selectionGradientVertical : parent.gradientVertical;
	w_rect size;
	w_control_get_bounds(W_CONTROL(parent), &size.pt, &size.sz);
	int width = size.width;
	int height = priv->tabHeight + ((style & W_FLAT) != 0 ? 1 : 3);
	int x = 0;

	int borderLeft = priv->borderVisible ? 1 : 0;
	int borderTop = (style & W_BOTTOM) ? borderLeft : 0;
	int borderBottom = (style & W_BOTTOM) ? 0 : borderLeft;

	if (borderLeft > 0) {
		x += 1;
		width -= 2;
	}
	int y = (style & W_BOTTOM) ? size.y - borderBottom - height : borderTop;
	wc_tabfolder_custom_draw_background_0(parent, gc, bounds, shape,
			shape_count, defaultBackground, image, selected);
}
void wc_tabfolder_custom_draw_background(w_tabfolder *parent, w_graphics *gc,
		w_rect *bounds, int state) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	int style = w_widget_get_style(W_WIDGET(parent));
	int selected = (state & W_SELECTED) != 0;
	w_color bg;
	w_control_get_background(W_CONTROL(parent), &bg);
	w_color defaultBackground = selected ? priv->selectionBackground : bg;
	w_image *image = 0; //selected ? parent.selectionBgImage : 0;
	w_color *colors = 0; // selected ? parent.selectionGradientColors : parent.gradientColors;
	int *percents = 0; //selected ? parent.selectionGradientPercents : parent.gradientPercents;
	int vertical = 0; //selected ? parent.selectionGradientVertical : parent.gradientVertical;

	wc_tabfolder_custom_draw_background_0(parent, gc, bounds, 0, 0,
			defaultBackground, image, selected);
}
char closebtnshape[] = { 0, 0, 2, 0, 4, 2, 5, 2, 7, 0, 9, 0, 9, 2, 7, 4, 7, 5,
		9, 7, 9, 9, 7, 9, 5, 7, 4, 7, 2, 9, 0, 9, 0, 7, 2, 5, 2, 4, 0, 2 };
char closebtnshapeselected[] = { 1, 1, 3, 1, 5, 3, 6, 3, 8, 1, 10, 1, 10, 3, 8,
		5, 8, 6, 10, 8, 10, 10, 8, 10, 6, 8, 5, 8, 3, 10, 1, 10, 1, 8, 3, 6, 3,
		5, 1, 3 };
char closebtnshapebackground[] = { 0, 0, 10, 0, 10, 10, 0, 10 };
void wc_tabfolder_custom_draw_close(w_tabfolder *parent, w_graphics *gc,
		const w_rect *closeRect, int closeImageState) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	if (closeRect->width == 0 || closeRect->height == 0)
		return;
	// draw X 9x9
	wuint64 style = w_widget_get_style(W_WIDGET(parent));
	int x = closeRect->x + w_max(1, (closeRect->width - 9) / 2);
	int y = closeRect->y + w_max(1, (closeRect->height - 9) / 2);
	y += (style & W_BOTTOM) ? -1 : 1;
	w_color closeBorder = COLOR_BUTTON_BORDER(
			w_widget_get_toolkit(W_WIDGET(parent)));
	w_point shape[25];
	int index = 0;
	switch (closeImageState & (W_HOT | W_SELECTED | W_BACKGROUND)) {
	case W_NONE: {
		for (int i = 0; i < sizeof(closebtnshape) / 2; i++) {
			shape[i].x = x + closebtnshape[2 * i];
			shape[i].y = y + closebtnshape[2 * i + 1];
		}
		w_graphics_set_background(gc,
				COLOR_BUTTON_FILL(w_widget_get_toolkit(W_WIDGET(parent))));
		w_graphics_fill_polygon(gc, shape, sizeof(closebtnshape) / 2);
		w_graphics_set_foreground(gc, closeBorder);
		w_graphics_draw_polygon(gc, shape, sizeof(closebtnshape) / 2);
		break;
	}
	case W_HOT: {
		for (int i = 0; i < sizeof(closebtnshape) / 2; i++) {
			shape[i].x = x + closebtnshape[2 * i];
			shape[i].y = y + closebtnshape[2 * i + 1];
		}
		w_graphics_set_background(gc, CLOSE_FILL);
		w_graphics_fill_polygon(gc, shape, sizeof(closebtnshape) / 2);
		w_graphics_set_foreground(gc, closeBorder);
		w_graphics_draw_polygon(gc, shape, sizeof(closebtnshape) / 2);
		break;
	}
	case W_SELECTED: {
		for (int i = 0; i < sizeof(closebtnshapeselected) / 2; i++) {
			shape[i].x = x + closebtnshapeselected[2 * i];
			shape[i].y = y + closebtnshapeselected[2 * i + 1];
		}
		w_graphics_set_background(gc, CLOSE_FILL);
		w_graphics_fill_polygon(gc, shape, sizeof(closebtnshapeselected) / 2);
		w_graphics_set_foreground(gc, closeBorder);
		w_graphics_draw_polygon(gc, shape, sizeof(closebtnshapeselected) / 2);
		break;
	}
	case W_BACKGROUND: {
		for (int i = 0; i < sizeof(closebtnshapebackground) / 2; i++) {
			shape[i].x = x + closebtnshapebackground[2 * i];
			shape[i].y = y + closebtnshapebackground[2 * i + 1];
		}
		wc_tabfolder_custom_draw_background_1(parent, gc, closeRect, shape,
				sizeof(closebtnshapebackground) / 2, W_FALSE);
		break;
	}
	}
}

void wc_tabfolder_custom_draw_selected(w_tabfolder *parent, wc_tabitem *item,
		int itemIndex, int state, w_rect *bounds, w_graphics *gc) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	wuint64 style = w_widget_get_style(W_WIDGET(parent));
	int x = bounds->x;
	int y = bounds->y;
	int height = bounds->height;
	int width = bounds->width;
	int curveWidth = wc_tabfolder_custom__curve_width(parent);
	int curveIndent = curveWidth / 3;
	if (!priv->simple && !priv->single)
		width -= (curveWidth - curveIndent);
	int borderLeft = priv->borderVisible ? 1 : 0;
	int borderRight = borderLeft;
	int borderTop = (style & W_BOTTOM) ? borderLeft : 0;
	int borderBottom = (style & W_BOTTOM) ? 0 : borderLeft;

	w_rect b, tmp, closeRect, r;
	w_control_get_bounds(W_CONTROL(parent), &b.pt, &b.sz);
	wc_tabitem_get_close_rect(parent, item, gc, &closeRect);
	int rightEdge = w_min(x + width,
			wc_tabfolder_get_right_item_edge(parent, gc));
	//	 Draw selection border across all tabs

	w_point shape[50];
	int index = 0;

	if ((state & W_BACKGROUND) != 0) {
		int highlight_header = (style & W_FLAT) != 0 ? 1 : 3;
		int xx = borderLeft;
		int yy =
				(style & W_BOTTOM) ?
						b.height - borderBottom - priv->tabHeight
								- highlight_header :
						borderTop + priv->tabHeight + 1;
		int ww = b.width - borderLeft - borderRight;
		int hh = highlight_header - 1;
		shape[index].x = xx;
		shape[index++].y = yy;
		shape[index].x = xx + ww;
		shape[index++].y = yy;
		shape[index].x = xx + ww;
		shape[index++].y = yy + hh;
		shape[index].x = xx;
		shape[index++].y = yy + hh;
		/*if (priv->gradientBackground != 0
		 && priv->gradientBackground->selectionGradientColors
		 != 0
		 && !priv->gradientBackground->selectionGradientVertical) {
		 wc_tabfolder_custom_draw_background_1(parent, gc, bounds, shape, index,
		 true);
		 } else {*/
		w_graphics_set_background(gc, priv->selectionBackground);
		r.x = xx;
		r.y = yy;
		r.width = ww;
		r.height = hh;
		w_graphics_fill_rectangle(gc, &r);
		//}

		if (priv->single) {
			if (!item->showing)
				return;
		} else {
			// if selected tab scrolled out of view or partially out of view
			// just draw bottom line
			if (!item->showing) {
				int x1 = w_max(0, borderLeft - 1);
				int y1 = (style & W_BOTTOM) ? y - 1 : y + height;
				int x2 = b.width - borderRight;
				w_graphics_set_foreground(gc,
						COLOR_BORDER1_COLOR(
								w_widget_get_toolkit(W_WIDGET(parent))));
				r.x = x1;
				r.y = y1;
				r.width = x2;
				r.height = y1;
				w_graphics_draw_line(gc, &r.pt, (w_point*) &r.sz);
				return;
			}

			// draw selected tab background and outline
			index = 0;
			if ((style & W_BOTTOM)) {
				shape[index].x = x; // first point repeated here because below we reuse shape to draw outline
				shape[index++].y = y - 1;
				shape[index].x = x;
				shape[index++].y = y - 1;
				if (priv->simple) {
					if (borderLeft == 0 && item == priv->firstItem) {
						shape[index].x = x + x;
						shape[index++].y = y + height + y + height - 1;
					} else {
						for (int i = 0;
								i < sizeof(SIMPLE_BOTTOM_LEFT_CORNER) / 2;
								i++) {
							shape[index].x = x
									+ SIMPLE_BOTTOM_LEFT_CORNER[2 * i];
							shape[index++].y = y + height
									+ SIMPLE_BOTTOM_LEFT_CORNER[2 * i + 1] - 1;
						}
					}
					for (int i = 0; i < sizeof(SIMPLE_BOTTOM_RIGHT_CORNER) / 2;
							i++) {
						shape[index].x = rightEdge - 1
								+ SIMPLE_BOTTOM_RIGHT_CORNER[2 * i];
						shape[index++].y = y + height
								+ SIMPLE_BOTTOM_RIGHT_CORNER[2 * i + 1] - 1;
					}
					shape[index].x = rightEdge - 1;
					shape[index++].y = y - 1;
					shape[index].x = rightEdge - 1;
					shape[index++].y = y - 1;
				} else {
					if (borderLeft == 0 && item == priv->firstItem) {
						shape[index].x = x + x;
						shape[index++].y = y + height + y + height - 1;
					} else {
						for (int i = 0; i < sizeof(BOTTOM_LEFT_CORNER) / 2;
								i++) {
							shape[index].x = x + BOTTOM_LEFT_CORNER[2 * i];
							shape[index++].y = y + height
									+ BOTTOM_LEFT_CORNER[2 * i + 1] - 1;
						}
					}
					int d = priv->tabHeight - 12;
					for (int i = 0; i < sizeof(CURVE_BOTTOM) / 2; i++) {
						shape[index].x = rightEdge - 1 + CURVE_BOTTOM[2 * i];
						shape[index++].y = y + height + CURVE_BOTTOM[2 * i + 1]
								+ d - 1;
					}
					shape[index].x = rightEdge + curveWidth - curveIndent;
					shape[index++].y = y - 1;
					shape[index].x = rightEdge + curveWidth - curveIndent;
					shape[index++].y = y - 1;
				}
				shape[index].x =
						priv->simple ?
								rightEdge - 1 :
								rightEdge + curveWidth - curveIndent;
				shape[index++].y = y - 1;
				shape[index].x =
						priv->simple ?
								rightEdge - 1 :
								rightEdge + curveWidth - curveIndent;
				shape[index++].y = y - 1;
			} else {
				shape[index].x = x; // first point repeated here because below we reuse shape to draw outline
				shape[index++].y = y + height + 1;
				shape[index].x = x;
				shape[index++].y = y + height + 1;
				if (priv->simple) {
					for (int i = 0; i < sizeof(SIMPLE_TOP_LEFT_CORNER) / 2;
							i++) {
						shape[index].x = x + SIMPLE_TOP_LEFT_CORNER[2 * i];
						shape[index++].y = y
								+ SIMPLE_TOP_LEFT_CORNER[2 * i + 1];
					}
					for (int i = 0; i < sizeof(SIMPLE_TOP_RIGHT_CORNER) / 2;
							i++) {
						shape[index].x = rightEdge - 1
								+ SIMPLE_TOP_RIGHT_CORNER[2 * i];
						shape[index++].y = y
								+ SIMPLE_TOP_RIGHT_CORNER[2 * i + 1];
					}
					shape[index].x = rightEdge - 1;
					shape[index++].y = y + height + 1;
					shape[index].x = rightEdge - 1;
					shape[index++].y = y + height + 1;
				} else {
					for (int i = 0; i < sizeof(TOP_LEFT_CORNER) / 2; i++) {
						shape[index].x = x + TOP_LEFT_CORNER[2 * i];
						shape[index++].y = y + TOP_LEFT_CORNER[2 * i + 1];
					}
					for (int i = 0; i < sizeof(CURVE_NOT_BOTTOM) / 4; i++) {
						shape[index].x = rightEdge - curveIndent
								+ CURVE_NOT_BOTTOM[2 * i];
						shape[index++].y = y + CURVE_NOT_BOTTOM[2 * i + 1];
					}
					int d = priv->tabHeight - 12;
					for (int i = sizeof(CURVE_NOT_BOTTOM) / 4;
							i < sizeof(CURVE_NOT_BOTTOM) / 2; i++) {
						shape[index].x = rightEdge - curveIndent
								+ CURVE_NOT_BOTTOM[2 * i] + d;
						shape[index++].y = y + CURVE_NOT_BOTTOM[2 * i + 1] + d;
					}
					shape[index].x = rightEdge + curveWidth - curveIndent;
					shape[index++].y = y + height + 1;
					shape[index].x = rightEdge + curveWidth - curveIndent;
					shape[index++].y = y + height + 1;
				}
			}

			w_rect clipping, clipBounds;
			w_graphics_get_clipping_rect(gc, &clipping);
			memcpy(&clipBounds, &item->bounds, sizeof(w_rect));
			clipBounds.height += 1;
			if (style & W_BOTTOM)
				clipBounds.y -= 1;
			int tabInPaint = W_TRUE; //clipping.Intersects(clipBounds);

			if (tabInPaint) {
				// fill in tab background
				/*if (priv->gradientBackground != 0
				 && priv->gradientBackground->selectionGradientColors
				 != 0
				 && !priv->gradientBackground->selectionGradientVertical) {
				 wc_tabfolder_custom_draw_background_1(parent, gc, bounds,
				 shape, index, true);
				 } else {*/
				w_color defaultBackground = priv->selectionBackground;
				w_image *image = &priv->selectionBgImage;
				tmp.x = x;
				tmp.y = (style & W_BOTTOM) ? y - 1 : y + 1;
				tmp.width = width;
				tmp.height = height;
				if (!priv->single && !priv->simple)
					tmp.width += curveWidth - curveIndent;
				wc_tabfolder_custom_draw_background_0(parent, gc, &tmp, shape,
						index, defaultBackground, image, W_TRUE);
				//}
			}

			//Highlight MUST be drawn before the outline so that outline can cover it in the right spots (start of swoop)
			//otherwise the curve looks jagged
			//drawHighlight(gc, bounds, state, rightEdge);
			// draw outline
			shape[0].x = w_max(0, borderLeft - 1);
			if (borderLeft == 0 && item == priv->firstItem) {
				shape[0].y = (style & W_BOTTOM) ? y + height - 1 : y;
				shape[2].y = shape[1].y = shape[0].y;
			}
			shape[index - 1].x = b.width - borderRight + 1;
			for (int i = 0; i < index; i++) {
				if (shape[i].y == y + height + 1)
					shape[i].y -= 1;
			}
			w_color borderColor = COLOR_BORDER1_COLOR(
					w_widget_get_toolkit(W_WIDGET(parent)));
			/*if (!borderColor.equals(lastBorderColor))
			 createAntialiasColors();
			 antialias(shape, selectedInnerColor, selectedOuterColor, gc);*/
			w_graphics_set_foreground(gc, borderColor);
			w_graphics_draw_polyline(gc, shape, index);
			if (!tabInPaint)
				return;
		}
	}

	if ((state & W_FOREGROUND) != 0) {
		// draw Image
		w_rect trim;
		memset(&r, 0, sizeof(w_rect));
		wc_tabfolder_custom_compute_trim(parent, item, 0, W_NONE, &trim, &r);
		int xDraw = x - trim.x;
		if (priv->single && ((style & W_CLOSE) || item->showClose))
			xDraw += closeRect.width;
		w_imagelist *imagelist = priv->imagelist;
		int image = item->image;
		if (w_imagelist_is_ok(imagelist)) {
			w_size imageBounds;
			w_imagelist_get_size(imagelist, &imageBounds);
			// only draw image if it won't overlap with close button
			int maxImageWidth = rightEdge - xDraw - (trim.width + trim.x);
			if (!priv->single && closeRect.width > 0)
				maxImageWidth -= closeRect.width + INTERNAL_SPACING;
			if (imageBounds.width < maxImageWidth) {
				int imageX = xDraw;
				int imageY = y + (height - imageBounds.height) / 2;
				imageY += (style & W_BOTTOM) ? -1 : 1;
				if (image >= 0) {
					w_point _p = { imageX, imageY };
					w_imagelist_draw(imagelist, gc, image, &_p,
							W_IMAGELIST_DRAW_NORMAL);
				}
				xDraw += imageBounds.width + INTERNAL_SPACING;
			}
		}

		// draw Text
		int textWidth = rightEdge - xDraw - (trim.width + trim.x);
		/*if (!parent->data.single && item.closeRect.width > 0)
		 textWidth -= item.closeRect.width + INTERNAL_SPACING;*/
		if (textWidth > 0) {
			w_font *gcFont = w_graphics_get_font(gc);
			w_font *_f;
			w_control_get_font(W_CONTROL(parent), &_f);
			w_graphics_set_font(gc, item->font == 0 ? _f : item->font);

			/*if (item.shortenedText == null
			 || item.shortenedTextWidth != textWidth) {
			 item.shortenedText = shortenText(gc, item.getText(), textWidth);
			 item.shortenedTextWidth = textWidth;
			 }*/
			w_size extent;
			w_graphics_text_extent(gc, item->text, item->shortenedTextWidth,
					&extent,
					FLAGS);
			int textY = y + (height - extent.height) / 2;
			textY += (style & W_BOTTOM) ? -1 : 1;
			w_graphics_set_foreground(gc, priv->selectionForeground);
			r.x = xDraw;
			r.y = textY;
			r.width = -1;
			r.height = -1;
			w_graphics_draw_text(gc, item->text, item->shortenedTextWidth, &r,
			FLAGS);
			w_graphics_set_font(gc, gcFont);

			// draw a Focus rectangle
			if (w_control_is_focus_control(W_CONTROL(parent))) {
				if (priv->simple || priv->single) {
					w_graphics_set_background(gc, W_COLOR_BLACK);
					w_graphics_set_foreground(gc, W_COLOR_WHITE);
					r.x = xDraw - 1;
					r.y = textY - 1;
					r.width = extent.width + 2;
					r.height = extent.height + 2;
					w_graphics_draw_focus(gc, &r);
				} else {
					w_graphics_set_foreground(gc,
							COLOR_BUTTON_BORDER(
									w_widget_get_toolkit(W_WIDGET(parent))));
					r.x = xDraw;
					r.y = textY + extent.width + 1;
					r.width = xDraw + extent.height + 1;
					r.height = textY + extent.width + 1;
					w_graphics_draw_line(gc, &r.pt, (w_point*) &r.sz);
				}
			}
		}
		if ((style & W_CLOSE) || item->showClose) {
			wc_tabfolder_custom_draw_close(parent, gc, &closeRect,
					item->closeImageState);
		}
	}
}
void wc_tabfolder_custom_draw_unselected(w_tabfolder *parent, wc_tabitem *item,
		int itemIndex, int state, w_rect *bounds, w_graphics *gc) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	wuint64 style = w_widget_get_style(W_WIDGET(parent));
	int x = bounds->x;
	int y = bounds->y;
	int height = bounds->height;
	int width = bounds->width;

	// Do not draw partial items
	if (!item->showing)
		return;

	/*WRect clipping;
	 gc.GetClipping(clipping);
	 if (!clipping.Intersects(bounds))
	 return;*/

	if ((state & W_BACKGROUND) != 0) {
		/*if (index > 0 && index < parent.selectedIndex)
		 drawLeftUnselectedBorder(gc, bounds, state);*/
		// If it is the last one then draw a line
		//if (index > parent.selectedIndex)
		wc_tabfolder_custom_draw_right_unselected_border(parent, state, bounds,
				gc);
	}

	if ((state & W_FOREGROUND) != 0) {
		// draw Image
		w_rect trim, r;
		memset(&r, 0, sizeof(r));
		wc_tabfolder_custom_compute_trim(parent, item, 0, W_NONE, &trim, &r);
		int xDraw = x - trim.x;
		int image = item->image;
		w_imagelist *imagelist = priv->imagelist;
		if (w_imagelist_is_ok(imagelist) && priv->showUnselectedImage) {
			w_size imageBounds;
			w_imagelist_get_size(imagelist, &imageBounds);
			// only draw image if it won't overlap with close button
			int maxImageWidth = x + width - xDraw - (trim.width + trim.x);
			if (priv->showUnselectedClose
					&& ((style & W_CLOSE) || item->showClose)) {
				w_rect closeRect;
				wc_tabitem_get_close_rect(parent, item, gc, &closeRect);
				maxImageWidth -= closeRect.width + INTERNAL_SPACING;
			}
			if (imageBounds.width < maxImageWidth) {
				int imageX = xDraw;
				int imageHeight = imageBounds.height;
				int imageY = y + (height - imageHeight) / 2;
				imageY += (style & W_BOTTOM) ? -1 : 1;
				int _h = imageBounds.height;
				if (_h == 0)
					_h = 1;
				int imageWidth = imageBounds.width * imageHeight / _h;
				if (image >= 0) {
					w_point _pt = { imageX, imageY };
					w_imagelist_draw(imagelist, gc, image, &_pt,
							W_IMAGELIST_DRAW_NORMAL);
					/*gc.drawImage(image, imageBounds.x, imageBounds.y,
					 imageBounds.width, imageBounds.height, imageX, imageY,
					 imageWidth, imageHeight);*/
				}
				xDraw += imageWidth + INTERNAL_SPACING;
			}
		}
		// draw Text
		int textWidth = x + width - xDraw - (trim.width + trim.x);
		if (priv->showUnselectedClose
				&& ((style & W_CLOSE) || item->showClose)) {
			//textWidth -= item.closeRect.width + INTERNAL_SPACING;
		}
		if (textWidth > 0) {
			w_font *gcFont = w_graphics_get_font(gc);
			w_font *_f;
			w_control_get_font(W_CONTROL(parent), &_f);
			w_graphics_set_font(gc, item->font == 0 ? _f : item->font);
			/*if (item->shortenedText == null || item.shortenedTextWidth != textWidth) {
			 item.shortenedText = shortenText(gc, item.getText(), textWidth);
			 item.shortenedTextWidth = textWidth;
			 }*/
			w_size extent;
			w_graphics_text_extent(gc, item->text, item->shortenedTextWidth,
					&extent,
					FLAGS);
			int textY = y + (height - extent.height) / 2;
			textY += (style & W_BOTTOM) ? -1 : 1;
			w_color fg;
			w_control_get_foreground(W_CONTROL(parent), &fg);
			w_graphics_set_foreground(gc, fg);
			r.x = xDraw;
			r.y = textY;
			r.width = -1;
			r.height = -1;
			w_graphics_draw_text(gc, item->text, item->shortenedTextWidth, &r,
			FLAGS);
			w_graphics_set_font(gc, gcFont);
		}
		// draw close
		if (priv->showUnselectedClose
				&& ((style & W_CLOSE) || item->showClose)) {
			if ((style & W_CLOSE) || item->showClose) {
				w_rect closeRect;
				wc_tabitem_get_close_rect(parent, item, gc, &closeRect);
				wc_tabfolder_custom_draw_close(parent, gc, &closeRect,
						item->closeImageState);
			}
		}
	}
}
void wc_tabfolder_custom_draw_body(w_tabfolder *parent, w_graphics *gc,
		const w_rect *bounds, int state) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	wuint64 style = w_widget_get_style(W_WIDGET(parent));
	w_size size;
	size.width = bounds->width;
	size.height = bounds->height;
	int tabHeight = priv->tabHeight;

	int borderLeft = priv->borderVisible ? 1 : 0;
	int borderRight = borderLeft;
	int borderTop = (style & W_BOTTOM) ? borderLeft : 0;
	int borderBottom = (style & W_BOTTOM) ? 0 : borderLeft;

	int highlight_header = (style & W_FLAT) != 0 ? 1 : 3;
	int highlight_margin = (style & W_FLAT) != 0 ? 0 : 2;

	// fill in body
	if (!priv->minimized) {
		int width = size.width - borderLeft - borderRight
				- 2 * highlight_margin;
		int height = size.width - borderTop - borderBottom - tabHeight
				- highlight_header - highlight_margin;
		// Draw highlight margin
		if (highlight_margin > 0) {
			int index = 0;
			w_point shape[20];
			if ((style & W_BOTTOM)) {
				int x1 = borderLeft;
				int y1 = borderTop;
				int x2 = size.width - borderRight;
				int y2 = size.width - borderBottom - tabHeight
						- highlight_header;
				int flags = shape[index].x = x1;
				shape[index++].y = y1;
				shape[index].x = x2;
				shape[index++].y = y1;
				shape[index].x = x2;
				shape[index++].y = y2;
				shape[index].x = x2 - highlight_margin;
				shape[index++].y = y2;
				shape[index].x = x2 - highlight_margin;
				shape[index++].y = y1 + highlight_margin;
				shape[index].x = x1 + highlight_margin;
				shape[index++].y = y1 + highlight_margin;
				shape[index].x = x1 + highlight_margin;
				shape[index++].y = y2;
				shape[index].x = x1;
				shape[index++].y = y2;
			} else {
				int x1 = borderLeft;
				int y1 = borderTop + tabHeight + highlight_header;
				int x2 = size.width - borderRight;
				int y2 = size.height - borderBottom;
				shape[index].x = x1;
				shape[index++].y = y1;
				shape[index].x = x1 + highlight_margin;
				shape[index++].y = y1;
				shape[index].x = x1 + highlight_margin;
				shape[index++].y = y2 - highlight_margin;
				shape[index].x = x2 - highlight_margin;
				shape[index++].y = y2 - highlight_margin;
				shape[index].x = x2 - highlight_margin;
				shape[index++].y = y1;
				shape[index].x = x2;
				shape[index++].y = y1;
				shape[index].x = x2;
				shape[index++].y = y2;
				shape[index].x = x1;
				shape[index++].y = y2;
			}
			// If horizontal gradient, show gradient across the whole area
			/*if (priv->selectedItem != 0
			 && priv->gradientBackground != 0
			 && priv->gradientBackground->selectionGradientColors
			 != 0
			 && priv->gradientBackground->selectionGradientColors_length
			 > 1
			 && !priv->gradientBackground->selectionGradientVertical) {
			 wc_tabfolder_custom_draw_background_1(parent, gc, bounds, shape, index,
			 true);
			 } else if (priv->selectedItem != 0
			 && priv->gradientBackground != 0
			 && priv->gradientBackground->gradientColors
			 != 0
			 && priv->gradientBackground->gradientColors_length
			 > 1
			 && !priv->gradientBackground->gradientVertical) {
			 wc_tabfolder_custom_draw_background_1(parent, gc, bounds, shape, index,
			 false);
			 } else {*/
			w_color bg;
			w_control_get_background(W_CONTROL(parent), &bg);
			w_graphics_set_background(gc,
					priv->selectedItem == 0 ? bg : priv->selectionBackground);
			//}
		}
		//Draw client area
		if ((style & W_NO_BACKGROUND) != 0) {
			w_color bg;
			w_control_get_background(W_CONTROL(parent), &bg);
			w_graphics_set_background(gc, bg);
			int marginWidth = priv->marginWidth;
			int marginHeight = priv->marginHeight;
			int xClient = borderLeft + marginWidth + highlight_margin, yClient;
			if (style & W_BOTTOM) {
				yClient = borderTop + highlight_margin + marginHeight;
			} else {
				yClient = borderTop + tabHeight + highlight_header
						+ marginHeight;
			}
			w_rect r;
			r.x = xClient - marginWidth;
			r.y = yClient - marginHeight;
			r.width = width;
			r.height = height;
			w_graphics_fill_rectangle(gc, &r);
		}
	} else {
		if ((style & W_NO_BACKGROUND) != 0) {
			int height = borderTop + tabHeight + highlight_header
					+ borderBottom;
			if (size.height > height) {
				w_color bg;
				w_control_get_background(W_CONTROL(parent), &bg);
				w_graphics_set_background(gc, bg);
				w_rect r;
				r.x = 0;
				r.y = height;
				r.width = size.width;
				r.height = size.height - height;
				w_graphics_fill_rectangle(gc, &r);
			}
		}
	}

	//draw 1 pixel border around outside
	if (borderLeft > 0) {
		w_graphics_set_foreground(gc,
				COLOR_BORDER1_COLOR(w_widget_get_toolkit(W_WIDGET(parent))));
		int x1 = borderLeft - 1;
		int x2 = size.width - borderRight;
		int y1 = (style & W_BOTTOM) ? borderTop - 1 : borderTop + tabHeight;
		int y2 =
				(style & W_BOTTOM) ?
						size.height - tabHeight - borderBottom - 1 :
						size.height - borderBottom;
		w_point p1, p2;
		p1.x = x1;
		p1.y = y1;
		p2.x = x1;
		p2.y = y2;
		w_graphics_draw_line(gc, &p1, &p2); // left
		p1.x = x2;
		p1.y = y1;
		p2.x = x2;
		p2.y = y2;
		w_graphics_draw_line(gc, &p1, &p2); // right
		if (style & W_BOTTOM) {
			p1.x = x1;
			p1.y = y1;
			p2.x = x2;
			p2.y = y1;
			w_graphics_draw_line(gc, &p1, &p2); // top
		} else {
			p1.x = x1;
			p1.y = y2;
			p2.x = x2;
			p2.y = y2;
			w_graphics_draw_line(gc, &p1, &p2); // bottom
		}
	}
}

void wc_tabfolder_custom_draw(w_tabfolder *parent, wc_tabitem *item, int part,
		int state, w_rect *bounds, w_graphics *gc) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(parent));
	if (item == 0) {
		switch (part) {
		case WC_TABFOLDER_PART_BACKGROUND: {
			//this.drawBackground(gc, bounds, state);
		}
			break;
		case WC_TABFOLDER_PART_BODY:
			/*if (priv->platform) {
			 WTheme *theme = parent->GetToolkit()->GetTheme();
			 WThemeData themedata;
			 theme->OpenTabFolderData(themedata);
			 themedata.state = state;
			 themedata.style = parent->GetStyle();
			 themedata.DrawBackground(gc, bounds);
			 } else {*/
			wc_tabfolder_custom_draw_body(parent, gc, bounds, state);
			//}
			break;
		case WC_TABFOLDER_PART_HEADER:
			//drawTabArea(gc, bounds, state);
			break;
		case WC_TABFOLDER_PART_MAX_BUTTON:
			//drawMaximize(gc, bounds, state);
			break;
		case WC_TABFOLDER_PART_MIN_BUTTON:
			//drawMinimize(gc, bounds, state);
			break;
		case WC_TABFOLDER_PART_CHEVRON_BUTTON:
			//drawChevron(gc, bounds, state);
			break;
		}
	} else {
		if (bounds->width == 0 || bounds->height == 0)
			return;
		if ((state & W_SELECTED) != 0) {
			wc_tabfolder_custom_draw_selected(parent, item, part, state, bounds,
					gc);
		} else {
			wc_tabfolder_custom_draw_unselected(parent, item, part, state,
					bounds, gc);
		}
	}
}

struct wc_tabfolder_renderer custom_renderer = {	//
		wc_tabfolder_custom_init,	//
				wc_tabfolder_custom_dispose,	//
				wc_tabfolder_custom_compute_size,	//
				wc_tabfolder_custom_compute_trim,	//
				wc_tabfolder_custom_draw	//
		};//
