/*
 * browser_ie.c
 *
 *  Created on: 12 août 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "browser_ie.h"
wresult _w_browser_ie_back(w_browser *browser) {
	HRESULT hr = _W_BROWSER_IE(browser)->site.browser->lpVtbl->GoBack(
	_W_BROWSER_IE(browser)->site.browser);
	if (hr == S_OK) {
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_browser_ie_execute(w_browser *browser, const char *script) {
}
wresult _w_browser_ie_close(w_browser *browser) {
}
wresult _w_browser_ie_evaluate(w_browser *browser, const char *script,
		w_value *result) {
}
wresult _w_browser_ie_forward(w_browser *browser) {
	HRESULT hr = _W_BROWSER_IE(browser)->site.browser->lpVtbl->GoForward(
	_W_BROWSER_IE(browser)->site.browser);
	if (hr == S_OK) {
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
w_string_ref* _w_browser_ie_get_browser_type(w_browser *browser) {
	w_string_ref *ref;
	w_string_init_from_chars(&ref, "ie", W_FALSE);
	return ref;
}
wresult _w_browser_ie_get_javascript_enabled(w_browser *browser) {
}
w_string_ref* _w_browser_ie_get_text(w_browser *browser) {
	IDispatch *document = 0, *element;
	DISPID rgdispid;
	DISPPARAMS rgvarg;
	VARIANT varResult;
	UINT puArgErr;
	w_string_ref *ref = 0;
	struct UnicodeString str;
	HRESULT hr = _W_BROWSER_IE(browser)->site.browser->lpVtbl->get_Document(
	_W_BROWSER_IE(browser)->site.browser, &document);
	if (document != 0) {
		unicode_set(&str, "documentElement", -1);
		hr = document->lpVtbl->GetIDsOfNames(document, &GUID_NULL, &str.str, 1,
		LOCALE_USER_DEFAULT, &rgdispid);
		unicode_free(&str);
		if (hr == S_OK) {
			memset(&rgvarg, 0, sizeof(rgvarg));
			VariantInit(&varResult);
			hr = document->lpVtbl->Invoke(document, rgdispid, &GUID_NULL,
			LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &rgvarg, &varResult, 0,
					&puArgErr);
			if (varResult.vt == VT_DISPATCH && varResult.pdispVal != 0) {
				element = varResult.pdispVal;
				unicode_set(&str, "outerHTML", -1);
				hr = element->lpVtbl->GetIDsOfNames(element, &GUID_NULL,
						&str.str, 1,
						LOCALE_USER_DEFAULT, &rgdispid);
				unicode_free(&str);
				if (hr == S_OK) {
					memset(&rgvarg, 0, sizeof(rgvarg));
					VariantInit(&varResult);
					hr = element->lpVtbl->Invoke(element, rgdispid, &GUID_NULL,
					LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &rgvarg,
							&varResult, 0, 0);
					if (varResult.vt == VT_BSTR) {
						ref = unicode_to_ref(varResult.bstrVal, -1);
					}
				}
				element->lpVtbl->Release(element);
			}
		}
		document->lpVtbl->Release(document);
	}
	return ref;
}
w_string_ref* _w_browser_ie_get_url(w_browser *browser) {
	BSTR url = 0;
	w_string_ref *ref = 0;
	_W_BROWSER_IE(browser)->site.browser->lpVtbl->get_LocationURL(
	_W_BROWSER_IE(browser)->site.browser, &url);
	if (url != 0) {
		ref = unicode_to_ref(url, -1);
	}
	return ref;
}
wresult _w_browser_ie_is_back_enabled(w_browser *browser) {
}
wresult _w_browser_ie_is_forward_enabled(w_browser *browser) {
}
wresult _w_browser_ie_refresh(w_browser *browser) {
}
wresult _w_browser_ie_set_javascript_enabled(w_browser *browser,
		wresult enabled) {
}

wresult _w_browser_ie_set_url(w_browser *browser, const char *url,
		const char *postData, const char *headers) {
	VARIANT vEmpty;
	struct UnicodeString _url;
	VariantInit(&vEmpty);
	unicode_set(&_url, url, -1);
	HRESULT hr = _W_BROWSER_IE(browser)->site.browser->lpVtbl->Navigate(
	_W_BROWSER_IE(browser)->site.browser, _url.str, &vEmpty, &vEmpty, &vEmpty,
			&vEmpty);
	if (hr == S_OK) {
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_browser_ie_set_text(w_browser *browser, const char *html,
		wresult trusted) {
	wresult result = W_FALSE;
	_w_browser_ie_set_url(browser, "about:blank", 0, 0);
	IDispatch *document;
	_W_BROWSER_IE(browser)->site.browser->lpVtbl->get_Document(
	_W_BROWSER_IE(browser)->site.browser, &document);
	if (document != 0) {
		IPersistStreamInit *psi = 0;
		document->lpVtbl->QueryInterface(document, &IID_IPersistStreamInit,
				(void**) &psi);
		if (psi != 0) {
			// allocate memory
			int l = strlen(html);
			HGLOBAL hMem = GlobalAlloc(GPTR, l + 4);
			char *mem = (char*) GlobalLock(hMem);
			mem[0] = 0xEF;
			mem[1] = 0xBB;
			mem[2] = 0xBF;
			strcpy(&mem[3], html);
			// create stream
			IStream *stream = NULL;
			HRESULT hr = CreateStreamOnHGlobal(hMem, FALSE, &stream);
			if (SUCCEEDED(hr)) {
				// load html string
				if (psi->lpVtbl->Load(psi, stream) == S_OK) {
					result = W_TRUE;
				}
				stream->lpVtbl->Release(stream);
			}
			// free memory
			GlobalUnlock(hMem);
			GlobalFree(hMem);
		}
	}
	return result;
}
wresult _w_browser_ie_stop(w_browser *browser) {
}
/*
 *
 */
wresult _w_browser_ie_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	HRESULT result;
	wresult res = W_ERROR_NO_HANDLES;
	IWebBrowser *browser = 0;
	IOleObject *oleObject = 0;
	result = CoCreateInstance(&CLSID_WebBrowser, NULL, CLSCTX_INPROC,
			&IID_IWebBrowser, (void**) &browser);
	if (browser != 0) {
		res = _w_control_create(widget, parent, style, post_event);
		if (res > 0) {
			result = browser->lpVtbl->QueryInterface(browser, &IID_IOleObject,
					(void**) &oleObject);
			if (oleObject != 0) {
				_w_ie_web_site_init(&_W_BROWSER_IE(widget)->site);
				_W_BROWSER_IE(widget)->site.browser = browser;
				result = oleObject->lpVtbl->SetClientSite(oleObject,
						(IOleClientSite*) &_W_BROWSER_IE(widget)->site);
				if (result == S_OK) {
					RECT rc = { 0, 0, 200, 200 };
					oleObject->lpVtbl->DoVerb(oleObject, OLEIVERB_SHOW,
					NULL, (IOleClientSite*) &_W_BROWSER_IE(widget)->site, -1,
					_W_WIDGET(widget)->handle, &rc);
					_W_WIDGET(widget)->style = style;
					_W_WIDGET(widget)->post_event = post_event;
					return W_TRUE;
				}
			}
		}
		browser->lpVtbl->Release(browser);
	}
	return res;

}
int _BROWSER_IE_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (_W_BROWSER_IE(widget)->site.browser != 0) {
		IOleInPlaceObject *in_place = 0;
		RECT rc;
		_W_BROWSER_IE(widget)->site.browser->lpVtbl->QueryInterface(
		_W_BROWSER_IE(widget)->site.browser, &IID_IOleInPlaceObject,
				(void**) &in_place);
		if (in_place != 0) {
			GetClientRect(_W_WIDGET(widget)->handle, &rc);
			//rc.left -= 1;
			//rc.top -=1;
			in_place->lpVtbl->SetObjectRects(in_place, &rc, &rc);
		}
	}
}
int _BROWSER_IE_WM_DESTROY(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	IWebBrowser* browser= _W_BROWSER_IE(widget)->site.browser;
	int ret = _CONTROL_WM_DESTROY(widget, e,reserved);
	ret = browser->lpVtbl->Release(browser);
	return ret;
}
int _w_browser_ie_post_event(w_widget *widget, w_event *e) {
	return _w_control_post_event(widget, e);
}
void _w_browser_class_init(struct _w_browser_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BROWSER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_browser_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_browser);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_browser);
	//W_RESOURCE_CLASS(clazz)->dispose = _w_browser_ie_dispose;
	//
	W_WIDGET_CLASS(clazz)->create = _w_browser_ie_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_browser_ie_post_event;
	clazz->back = _w_browser_ie_back;
	clazz->execute = _w_browser_ie_execute;
	clazz->close = _w_browser_ie_close;
	clazz->evaluate = _w_browser_ie_evaluate;
	clazz->forward = _w_browser_ie_forward;
	clazz->get_browser_type = _w_browser_ie_get_browser_type;
	clazz->get_javascript_enabled = _w_browser_ie_get_javascript_enabled;
	clazz->get_text = _w_browser_ie_get_text;
	clazz->get_url = _w_browser_ie_get_url;
	clazz->is_back_enabled = _w_browser_ie_is_back_enabled;
	clazz->is_forward_enabled = _w_browser_ie_is_forward_enabled;
	clazz->refresh = _w_browser_ie_refresh;
	clazz->set_javascript_enabled = _w_browser_ie_set_javascript_enabled;
	clazz->set_text = _w_browser_ie_set_text;
	clazz->set_url = _w_browser_ie_set_url;
	clazz->stop = _w_browser_ie_stop;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	//messages
	struct _w_control_reserved *msg = reserved;
	msg->messages[_WM_SIZE] = _BROWSER_IE_WM_SIZE;
	msg->messages[_WM_DESTROY] = _BROWSER_IE_WM_DESTROY;
}
#endif
