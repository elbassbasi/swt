/*
 * image.c
 *
 *  Created on: 8 mars 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "graphics.h"
#ifdef __cplusplus
extern "C" {
#endif
void w_image_init(w_image *image) {
	_W_IMAGE(image)->image = 0;
	_W_IMAGE(image)->gimage = 0;
	_W_IMAGE(image)->flags = 0;
}
void w_image_dispose(w_image *image) {
	if (_W_IMAGE(image)->image != 0) {
		if (!(_W_IMAGE(image)->flags & IMAGE_FLAGS_NODISPOSE_HBITMAP)) {
			if (_W_IMAGE(image)->flags & IMAGE_FLAGS_ICON) {
				DestroyIcon((HICON) _W_IMAGE(image)->image);
			} else {
				DeleteObject(_W_IMAGE(image)->image);
			}
		}
		_W_IMAGE(image)->image = 0;
	}
	if (_W_IMAGE(image)->gimage != 0) {
		if (!(_W_IMAGE(image)->flags & IMAGE_FLAGS_NODISPOSE_GPBITMAP)) {
			GdipDisposeImage(_W_IMAGE(image)->gimage);
		}
		_W_IMAGE(image)->gimage = 0;
	}
	_W_IMAGE(image)->flags = 0;
}
wresult w_image_is_ok(w_image *image) {
	return _W_IMAGE(image)->image != 0 || _W_IMAGE(image)->gimage != 0;
}
HBITMAP _w_image_get_HBITMAP(w_image *image) {
	if (w_image_is_ok(image)) {
		if (_W_IMAGE(image)->image == 0) {
			ARGB argb = 0;
			GdipCreateHBITMAPFromBitmap(_W_IMAGE(image)->gimage,
					&_W_IMAGE(image)->image, argb);
		}
		return _W_IMAGE(image)->image;
	} else
		return 0;
}
wresult w_image_create_from_file(w_image *image, const char *file) {
	w_image_dispose(image);
	struct UnicodeString str;
	unicode_set(&str, file, -1);
	wresult result = FALSE;
	if (str.str != 0) {
		GpBitmap *bitmap = 0;
		for (int i = 0; i < str.length; i++) {
			if (str.str[i] == '/')
				str.str[i] = '\\';
		}
		GdipCreateBitmapFromFile(str.str, &bitmap);
		if (bitmap != 0) {
			_W_IMAGE(image)->gimage = bitmap;
			result = TRUE;
		}
	}
	return result;
}
wresult w_image_create_from_colors(w_image *image, w_size *size,
		w_color *colors, size_t length) {
}
wresult w_image_create_from_buffer(w_image *image, const void *buffer,
		size_t size) {
}
wresult w_image_get_size(w_image *image, w_size *size) {
	if (w_image_is_ok(image)) {
		if (_W_IMAGE(image)->image != 0) {
			BITMAP bm;
			GetObjectW(_W_IMAGE(image)->image, sizeof(BITMAP), &bm);
			size->width = bm.bmWidth;
			size->height = bm.bmHeight;
			return W_TRUE;
		} else {
			UINT t;
			GdipGetImageWidth(_W_IMAGE(image)->gimage, &t);
			size->width = t;
			GdipGetImageHeight(_W_IMAGE(image)->gimage, &t);
			size->height = t;
		}
		return W_TRUE;
	} else {
		size->width = 0;
		size->height = 0;
		return W_ERROR_NO_HANDLES;
	}
}
wresult w_image_get_pixels(w_image *image, w_color *colors, size_t length) {
}
wresult w_image_save_to_file(w_image *image, const char *file,
		const char *type) {
}
wresult w_image_save_to_buffer(w_image *image,w_alloc alloc,void* user_data, const char *type) {
}
wresult w_image_copy_area(w_image *image, w_rect *rect, w_image *destimage) {
}
wresult w_image_resize(w_image *image, w_size *size, w_image *destimage) {
	w_rect r;
	w_image_dispose(destimage);
	if (w_image_is_ok(image)) {
		w_image_get_size(image, &r.sz);
		if (r.sz.width == size->width && r.sz.height == size->height) {
			if (_W_IMAGE(image)->gimage != 0) {
				GdipCloneImage(_W_IMAGE(image)->gimage,
						&_W_IMAGE(destimage)->gimage);
				return W_TRUE;
			} else {
				_W_IMAGE(destimage)->image = CopyImage(_W_IMAGE(image)->image,
				IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
			}
		} else {
			GpBitmap *bitmap = 0, *bm2 = 0;
			GpGraphics *gc = 0;
			if (_W_IMAGE(image)->gimage != 0) {
				bitmap = _W_IMAGE(image)->gimage;
			} else {
				GdipCreateBitmapFromHBITMAP(_W_IMAGE(image)->image, NULL,
						&bitmap);
			}
			if (bitmap != 0) {
				GdipCreateBitmapFromScan0(size->width, size->height, 0,
				PixelFormat32bppPARGB,
				NULL, &bm2);
				if (bm2 != 0) {
					GdipGetImageGraphicsContext(bm2, &gc);
					if (gc != 0) {
						GdipSetInterpolationMode(gc,
								InterpolationModeHighQuality);
						GdipDrawImageRectI(gc, bitmap, 0, 0, size->width,
								size->height);
						GdipDeleteGraphics(gc);
					}
				}
				if (bitmap != _W_IMAGE(image)->gimage)
					GdipDisposeImage(bitmap);
			}
			_W_IMAGE(destimage)->gimage = bm2;
		}
		return W_TRUE;
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
wresult w_image_to_greyscale(w_image *image, w_image *destimage) {
}
wresult w_image_to_disabled(w_image *image, w_color background,
		w_color shadow, w_image *destimage) {
}
void w_surface_init(w_surface *surface){
	w_image_init(W_IMAGE(surface));
}
void w_surface_dispose(w_surface *surface){
	w_image_dispose(W_IMAGE(surface));
}
wresult w_surface_is_ok(w_surface *surface){

}
HBITMAP w_surface_create_DIB(int width, int height, int depth) {
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = (short) depth;
	bmi.bmiHeader.biCompression = BI_RGB;
	VOID *pBits;
	return CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
}
wresult w_surface_create(w_surface *surface, w_size *size){
	w_surface_dispose(surface);
	HDC hdc = 0;
	//_W_IMAGE(bitmap)->image = CreateCompatibleBitmap(hdc, size->width,
	//size->height);
	_W_IMAGE(surface)->image = CreateBitmap(size->width, size->height, 1, 32,
	NULL);
	if (_W_IMAGE(surface)->image == 0) {
		hdc = CreateCompatibleDC(NULL);
		int bits = GetDeviceCaps(hdc, BITSPIXEL);
		int planes = GetDeviceCaps(hdc, PLANES);
		int depth = bits * planes;
		if (depth < 16)
			depth = 16;
		_W_IMAGE(surface)->image = w_surface_create_DIB(size->width, size->height,
				depth);
	}
	if (_W_IMAGE(surface)->image != 0) {
		if (hdc == 0)
			hdc = CreateCompatibleDC(NULL);
		HBITMAP hOldBitmap = SelectObject(hdc, _W_IMAGE(surface)->image);
		PatBlt(hdc, 0, 0, size->width, size->height, PATCOPY);
		SelectObject(hdc, hOldBitmap);
	}
	if (hdc != 0)
		DeleteDC(hdc);
	if (_W_IMAGE(surface)->image == 0)
		return W_ERROR_NO_HANDLES;
	else
		return W_TRUE;
}
wresult w_surface_get_image(w_surface *surface, w_image *destimage){
	if (destimage == 0)
		return W_ERROR_INVALID_ARGUMENT;
	w_image_dispose(destimage);
	if (w_image_is_ok(W_IMAGE(surface))) {
		_W_IMAGE(destimage)->image = CopyImage(_W_IMAGE(surface)->image,
		IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_surface_get_graphics(w_surface *surface,
		w_graphics *graphics){
	if (graphics == 0)
		return W_ERROR_INVALID_ARGUMENT;
	w_graphics_dispose(graphics);
	if (w_image_is_ok(W_IMAGE(surface))) {
		HDC hdc = CreateCompatibleDC(NULL);
		HBITMAP hOldBitmap = SelectObject(hdc, _W_IMAGE(surface)->image);
		_w_graphics_init(graphics, hdc);
		_W_GRAPHICS(graphics)->oldbitmap = hOldBitmap;
		_W_GRAPHICS(graphics)->state |= W_GRAPHICS_DELETEDC;
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
#ifdef __cplusplus
}
#endif
#endif
