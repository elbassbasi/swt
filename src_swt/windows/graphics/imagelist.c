/*
 * imagelist.c
 *
 *  Created on: 26 mars 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "graphics.h"
void w_imagelist_init(w_imagelist *imagelist) {
	_W_IMAGELIST(imagelist)->imagelist = 0;
}
void w_imagelist_dispose(w_imagelist *imagelist) {
	if (imagelist != 0 && _W_IMAGELIST(imagelist)->imagelist != 0) {
		ImageList_Destroy(_W_IMAGELIST(imagelist)->imagelist);
		_W_IMAGELIST(imagelist)->imagelist = 0;
	}
}
wresult w_imagelist_is_ok(w_imagelist *imagelist) {
	if (imagelist != 0 && _W_IMAGELIST(imagelist)->imagelist != 0) {
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult w_imagelist_create(w_imagelist *imagelist, w_size *size,
		int initialCount) {
	w_imagelist_dispose(imagelist);
	w_toolkit_get_platform();
	_W_IMAGELIST(imagelist)->imagelist = ImageList_Create(size->width,
			size->height, ILC_COLOR32, initialCount, 4);
	if (_W_IMAGELIST(imagelist)->imagelist == 0) {
		return W_FALSE;
	} else
		return W_TRUE;
}
wresult w_imagelist_replace_0(w_imagelist *imagelist, int index, int *newindex,
		w_image *image) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	HBITMAP img;
	w_image newimage;
	int cx, cy;
	w_size sz;
	int flags = 0;
	if (!w_image_is_ok(image))
		return W_ERROR_INVALID_ARGUMENT;
	w_image_init(&newimage);
	ImageList_GetIconSize(_W_IMAGELIST(imagelist)->imagelist, &cx, &cy);
	w_image_get_size(image, &sz);
	img = 0;
	if (sz.width != cx || sz.height != cy) {
		sz.width = cx;
		sz.height = cy;
		w_image_resize(image, &sz, &newimage);
		img = _w_image_get_HBITMAP(&newimage);
		flags = _W_IMAGE(&newimage)->flags;
	} else {
		img = _w_image_get_HBITMAP(image);
		flags = _W_IMAGE(image)->flags;
	}
	if (flags & IMAGE_FLAGS_ICON) {
		*newindex = ImageList_ReplaceIcon(_W_IMAGELIST(imagelist)->imagelist,
				index, (HICON) img);
	} else {
		if (index == -1) {
			*newindex = ImageList_Add(_W_IMAGELIST(imagelist)->imagelist, img,
			NULL);
		} else {
			*newindex = ImageList_Replace(_W_IMAGELIST(imagelist)->imagelist,
					index, img,
					NULL);
		}
	}
	w_image_dispose(&newimage);
	return W_TRUE;
}
int w_imagelist_add(w_imagelist *imagelist, w_image *image) {
	int index = -1;
	w_imagelist_replace_0(imagelist, -1, &index, image);
	return index;
}
wresult w_imagelist_replace(w_imagelist *imagelist, int index, w_image *image) {
	int newindex = -1;
	return w_imagelist_replace_0(imagelist, index, &newindex, image);
}

wresult w_imagelist_remove(w_imagelist *imagelist, int index) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	return ImageList_Remove(_W_IMAGELIST(imagelist)->imagelist, index);
}
wresult w_imagelist_remove_all(w_imagelist *imagelist) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	return ImageList_Remove(_W_IMAGELIST(imagelist)->imagelist, -1);
}
wresult w_imagelist_get_image(w_imagelist *imagelist, int index, int copy,
		w_image *image) {
	if (image != 0)
		w_image_dispose(image);
	else
		return W_ERROR_INVALID_ARGUMENT;
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	if (copy) {
		HICON icon = ImageList_GetIcon(_W_IMAGELIST(imagelist)->imagelist,
				index, ILD_NORMAL | ILD_TRANSPARENT);
		if (icon != 0) {
			_W_IMAGE(image)->image = (HBITMAP) icon;
			_W_IMAGE(image)->flags = IMAGE_FLAGS_ICON;
			return W_TRUE;
		} else
			return W_FALSE;
	} else {
		IMAGEINFO info;
		if (ImageList_GetImageInfo(_W_IMAGELIST(imagelist)->imagelist, index,
				&info)) {
			_W_IMAGE(image)->image = info.hbmImage;
			_W_IMAGE(image)->flags = IMAGE_FLAGS_NODISPOSE_HBITMAP;
			return W_TRUE;
		} else {
			return W_FALSE;
		}
	}
}
wresult w_imagelist_get_size(w_imagelist *imagelist, w_size *size) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	int cx, cy;
	ImageList_GetIconSize(_W_IMAGELIST(imagelist)->imagelist, &cx, &cy);
	size->width = cx;
	size->height = cy;
	return W_TRUE;
}
int w_imagelist_get_count(w_imagelist *imagelist) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return 0;
	return ImageList_GetImageCount(_W_IMAGELIST(imagelist)->imagelist);
}
wresult w_imagelist_draw(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int flags) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	UINT fStyle;
	switch (flags) {
	case W_IMAGELIST_DRAW_TRANSPARENT:
		fStyle = ILD_NORMAL | ILD_TRANSPARENT;
		break;
	case W_IMAGELIST_DRAW_SELECTED:
		fStyle = ILD_SELECTED;
		break;
	case W_IMAGELIST_DRAW_FOCUSED:
		fStyle = ILD_FOCUS;
		break;
	case W_IMAGELIST_DRAW_NORMAL:
	default:
		fStyle = ILD_NORMAL;
		break;
	}
	return ImageList_Draw(_W_IMAGELIST(imagelist)->imagelist, index,
	_W_GRAPHICS(graphics)->handle, pt->x, pt->y, fStyle);
}
#endif
