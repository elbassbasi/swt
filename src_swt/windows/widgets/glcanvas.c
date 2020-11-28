/*
 * glcanvas.c
 *
 *  Created on: 1 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "glcanvas.h"

wresult _w_glcanvas_init_gldata(w_glcanvas *glcanvas, w_gldata *data) {
	if (data == 0)
		return W_ERROR_NULL_ARGUMENT;
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd,0,sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.dwLayerMask = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	if (data->doubleBuffer)
		pfd.dwFlags |= PFD_DOUBLEBUFFER;
	if (data->stereo)
		pfd.dwFlags |= PFD_STEREO;
	pfd.cRedBits = data->redSize;
	pfd.cGreenBits = data->greenSize;
	pfd.cBlueBits = data->blueSize;
	pfd.cAlphaBits = data->alphaSize;
	pfd.cDepthBits = data->depthSize;
	pfd.cStencilBits = data->stencilSize;
	pfd.cAccumRedBits = data->accumRedSize;
	pfd.cAccumGreenBits = data->accumGreenSize;
	pfd.cAccumBlueBits = data->accumBlueSize;
	pfd.cAccumAlphaBits = data->accumAlphaSize;
	pfd.cAccumBits = (pfd.cAccumRedBits + pfd.cAccumGreenBits
			+ pfd.cAccumBlueBits + pfd.cAccumAlphaBits);
	pfd.cColorBits   = 32;

	//FIXME - use wglChoosePixelFormatARB
//	if (data->sampleBuffers > 0) {
//		wglAttrib [pos++] = WGL.WGL_SAMPLE_BUFFERS_ARB;
//		wglAttrib [pos++] = data->sampleBuffers;
//	}
//	if (data->samples > 0) {
//		wglAttrib [pos++] = WGL.WGL_SAMPLES_ARB;
//		wglAttrib [pos++] = data->samples;
//	}

	HDC hDC = GetDC(_W_WIDGET(glcanvas)->handle);
	_W_GLCANVAS(glcanvas)->pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (_W_GLCANVAS(glcanvas)->pixelFormat == 0
			|| !SetPixelFormat(hDC, _W_GLCANVAS(glcanvas)->pixelFormat, &pfd)) {
		ReleaseDC(_W_WIDGET(glcanvas)->handle, hDC);
		return W_ERROR_UNSUPPORTED_DEPTH;
	}
	_W_GLCANVAS(glcanvas)->context = wglCreateContext(hDC);
	if (_W_GLCANVAS(glcanvas)->context == 0) {
		ReleaseDC(_W_WIDGET(glcanvas)->handle, hDC);
		return W_ERROR_NO_HANDLES;
	}
	DescribePixelFormat(hDC, _W_GLCANVAS(glcanvas)->pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	ReleaseDC(_W_WIDGET(glcanvas)->handle, hDC);
	if (data->shareContext != 0) {
		wglShareLists(_W_GLCANVAS(data->shareContext)->context,
		_W_GLCANVAS(glcanvas)->context);
	}
	return W_TRUE;
}
wresult _w_glcanvas_get_gldata(w_glcanvas *glcanvas, w_gldata *data) {
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(pfd);
	HDC hDC = GetDC(_W_WIDGET(glcanvas)->handle);
	DescribePixelFormat(hDC, _W_GLCANVAS(glcanvas)->pixelFormat, sizeof(pfd),
			&pfd);
	ReleaseDC(_W_WIDGET(glcanvas)->handle, hDC);
	data->doubleBuffer = (pfd.dwFlags & PFD_DOUBLEBUFFER) != 0;
	data->stereo = (pfd.dwFlags & PFD_STEREO) != 0;
	data->redSize = pfd.cRedBits;
	data->greenSize = pfd.cGreenBits;
	data->blueSize = pfd.cBlueBits;
	data->alphaSize = pfd.cAlphaBits;
	data->depthSize = pfd.cDepthBits;
	data->stencilSize = pfd.cStencilBits;
	data->accumRedSize = pfd.cAccumRedBits;
	data->accumGreenSize = pfd.cAccumGreenBits;
	data->accumBlueSize = pfd.cAccumBlueBits;
	data->accumAlphaSize = pfd.cAccumAlphaBits;
	return W_TRUE;
}
wresult _w_glcanvas_is_current(w_glcanvas *glcanvas) {
	return wglGetCurrentContext() == _W_GLCANVAS(glcanvas)->context;
}
wresult _w_glcanvas_set_current(w_glcanvas *glcanvas) {
	if (wglGetCurrentContext() == _W_GLCANVAS(glcanvas)->context)
		return W_TRUE;
	HDC hDC = GetDC(_W_WIDGET(glcanvas)->handle);
	wglMakeCurrent(hDC, _W_GLCANVAS(glcanvas)->context);
	ReleaseDC(_W_WIDGET(glcanvas)->handle, hDC);
	return W_TRUE;
}
wresult _w_glcanvas_swap_buffers(w_glcanvas *glcanvas) {
	HDC hDC = GetDC(_W_WIDGET(glcanvas)->handle);
	SwapBuffers(hDC);
	ReleaseDC(_W_WIDGET(glcanvas)->handle, hDC);
	return W_TRUE;
}
int _GLCANVS_WM_DESTROY(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved){
	if (_W_GLCANVAS(widget)->context != 0)
					wglDeleteContext(_W_GLCANVAS(widget)->context);
}
void _w_glcanvas_class_init(struct _w_glcanvas_class *clazz) {
	_w_canvas_class_init(W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_GLCANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_glcanvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_glcanvas);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_glcanvas);
	/*
	 *
	 */
	clazz->init_gldata = _w_glcanvas_init_gldata;
	clazz->get_gldata = _w_glcanvas_get_gldata;
	clazz->is_current = _w_glcanvas_is_current;
	clazz->set_current = _w_glcanvas_set_current;
	clazz->swap_buffers = _w_glcanvas_swap_buffers;
}
#endif
