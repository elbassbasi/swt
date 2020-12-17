/*
 * browser_ie_site.c
 *
 *  Created on: 13 août 2019
 *      Author: azeddine El Bassbasi
 */
#include "browser_ie.h"
#if defined( __WIN32__) || defined(__WIN64__)
const IID IID_IDocHostShowUI = { 0xc4d244b0, 0xd43e, 0x11cf, { 0x89, 0x3b, 0x00,
		0xaa, 0x00, 0xbd, 0xce, 0x1a } };
#define IE_IOleClientSite_SITE(This) ((_w_ie_web_site *)This)
HRESULT STDMETHODCALLTYPE IE_IOleClientSite_QueryInterface(IOleClientSite *This,
REFIID riid, void **ppvObject) {
	if (ppvObject == 0)
		return E_NOINTERFACE;
	_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	if (IsEqualGUID(riid, &IID_IUnknown)) {
		*ppvObject = site;
		return S_OK;
	}
	if (IsEqualGUID(riid, &IID_IOleClientSite)) {
		*ppvObject = &site->iOleClientSite;
		return S_OK;
	}
	if (IsEqualGUID(riid, &IID_IOleControlSite)) {
		*ppvObject = &site->iOleControlSite;
		return S_OK;
	}
	if (IsEqualGUID(riid, &IID_IOleInPlaceSite)) {
		*ppvObject = &site->iOleInPlaceSite;
		return S_OK;
	}
	if (IsEqualGUID(riid, &IID_IDispatch)) {
		*ppvObject = &site->iDispatch;
		return S_OK;
	}
	/*	if (IsEqualGUID(riid, &IID_IAdviseSink)) {
	 *ppvObject = &site->iAdviseSink;
	 return S_OK;
	 }
	 if (IsEqualGUID(riid, &IID_IOleDocumentSite)) {
	 *ppvObject = &site->iOleDocumentSite;
	 return S_OK;
	 }

	 if (IsEqualGUID(riid, &IID_IDocHostUIHandler)) {
	 *ppvObject = &site->iDocHostUIHandler;
	 return S_OK;
	 }*/
	if (IsEqualGUID(riid, &IID_IDocHostShowUI)) {
		*ppvObject = &site->iDocHostShowUI;
		return S_OK;
	}
	/*if (IsEqualGUID(riid, &IID_IServiceProvider)) {
	 *ppvObject = &site->iServiceProvider;
	 return S_OK;
	 }
	 if (IsEqualGUID(riid, &IID_IInternetSecurityManager)) {
	 *ppvObject = &site->iInternetSecurityManager;
	 return S_OK;
	 }
	 if (IsEqualGUID(riid, &IID_IOleCommandTarget)) {
	 *ppvObject = &site->iOleCommandTarget;
	 return S_OK;
	 }*/
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE IE_IOleClientSite_AddRef(IOleClientSite *This) {
	//_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	return 1;
}

ULONG STDMETHODCALLTYPE IE_IOleClientSite_Release(IOleClientSite *This) {
	//_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	return 1;
}

/*** IOleClientSite methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleClientSite_SaveObject(IOleClientSite *This) {
	//_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleClientSite_GetMoniker(IOleClientSite *This,
		DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk) {
	//_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleClientSite_GetContainer(IOleClientSite *This,
		IOleContainer **ppContainer) {
	//_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleClientSite_ShowObject(IOleClientSite *This) {
	//_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IOleClientSite_OnShowWindow(IOleClientSite *This,
		WINBOOL fShow) {
	//_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleClientSite_RequestNewObjectLayout(
		IOleClientSite *This) {
	//_w_ie_web_site *site = IE_IOleClientSite_SITE(This);
	return E_NOTIMPL;
}
IOleClientSiteVtbl IE_IOleClientSite_Vtbl =
		{ IE_IOleClientSite_QueryInterface, IE_IOleClientSite_AddRef,
				IE_IOleClientSite_Release, IE_IOleClientSite_SaveObject,
				IE_IOleClientSite_GetMoniker, IE_IOleClientSite_GetContainer,
				IE_IOleClientSite_ShowObject, IE_IOleClientSite_OnShowWindow,
				IE_IOleClientSite_RequestNewObjectLayout };
#define IE_IOleControlSite_SITE(This) ((_w_ie_web_site *) ((void*)This -1* sizeof(void*)))
/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleControlSite_QueryInterface(
		IOleControlSite *This,
		REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IOleControlSite_AddRef(IOleControlSite *This) {
	_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IOleControlSite_Release(IOleControlSite *This) {
	_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IOleControlSite methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleControlSite_OnControlInfoChanged(
		IOleControlSite *This) {
	//_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleControlSite_LockInPlaceActive(
		IOleControlSite *This, WINBOOL fLock) {
	//_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleControlSite_GetExtendedControl(
		IOleControlSite *This, IDispatch **ppDisp) {
	//_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleControlSite_TransformCoords(
		IOleControlSite *This, POINTL *pPtlHimetric, POINTF *pPtfContainer,
		DWORD dwFlags) {
	//_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleControlSite_TranslateAccelerator(
		IOleControlSite *This, MSG *pMsg, DWORD grfModifiers) {
	//_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleControlSite_OnFocus(IOleControlSite *This,
		WINBOOL fGotFocus) {
	//_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleControlSite_ShowPropertyFrame(
		IOleControlSite *This) {
	//_w_ie_web_site *site = IE_IOleControlSite_SITE(This);
	return E_NOTIMPL;
}
IOleControlSiteVtbl IE_IOleControlSite_Vtbl = {
		IE_IOleControlSite_QueryInterface, IE_IOleControlSite_AddRef,
		IE_IOleControlSite_Release, IE_IOleControlSite_OnControlInfoChanged,
		IE_IOleControlSite_LockInPlaceActive,
		IE_IOleControlSite_GetExtendedControl,
		IE_IOleControlSite_TransformCoords,
		IE_IOleControlSite_TranslateAccelerator, IE_IOleControlSite_OnFocus,
		IE_IOleControlSite_ShowPropertyFrame };
#define IE_IAdviseSink_SITE(This) ((_w_ie_web_site *) ((void*)This - 2 * sizeof(void*)))
/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE IE_IAdviseSink_QueryInterface(IAdviseSink *This,
REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IAdviseSink_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IAdviseSink_AddRef(IAdviseSink *This) {
	_w_ie_web_site *site = IE_IAdviseSink_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IAdviseSink_Release(IAdviseSink *This) {
	_w_ie_web_site *site = IE_IAdviseSink_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IAdviseSink methods ***/
void STDMETHODCALLTYPE IE_IAdviseSink_OnDataChange(IAdviseSink *This,
		FORMATETC *pFormatetc, STGMEDIUM *pStgmed) {
	//_w_ie_web_site *site = IE_IAdviseSink_SITE(This);
	//return E_NOTIMPL;
}

void STDMETHODCALLTYPE IE_IAdviseSink_OnViewChange(IAdviseSink *This,
		DWORD dwAspect, LONG lindex) {
	//_w_ie_web_site *site = IE_IAdviseSink_SITE(This);
	//return E_NOTIMPL;
}

void STDMETHODCALLTYPE IE_IAdviseSink_OnRename(IAdviseSink *This, IMoniker *pmk) {
	//_w_ie_web_site *site = IE_IAdviseSink_SITE(This);
	//return E_NOTIMPL;
}

void STDMETHODCALLTYPE IE_IAdviseSink_OnSave(IAdviseSink *This) {
	//_w_ie_web_site *site = IE_IAdviseSink_SITE(This);
	//return E_NOTIMPL;
}

void STDMETHODCALLTYPE IE_IAdviseSink_OnClose(IAdviseSink *This) {
	//_w_ie_web_site *site = IE_IAdviseSink_SITE(This);
	//return E_NOTIMPL;
}
IAdviseSinkVtbl IE_IAdviseSink_Vtbl = { IE_IAdviseSink_QueryInterface,
		IE_IAdviseSink_AddRef, IE_IAdviseSink_Release,
		IE_IAdviseSink_OnDataChange, IE_IAdviseSink_OnViewChange,
		IE_IAdviseSink_OnRename, IE_IAdviseSink_OnSave, IE_IAdviseSink_OnClose };
#define IE_IOleInPlaceSite_SITE(This) ((_w_ie_web_site *) ((void*)This -3* sizeof(void*)))
/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_QueryInterface(
		IOleInPlaceSite *This,
		REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IOleInPlaceSite_AddRef(IOleInPlaceSite *This) {
	_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IOleInPlaceSite_Release(IOleInPlaceSite *This) {
	_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IOleWindow methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_GetWindow(IOleInPlaceSite *This,
		HWND *phwnd) {
	_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	_w_browser *br = W_IE_WEB_SITE_GET_BROWSER(site);
	*phwnd = _W_WIDGET(br)->handle;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_ContextSensitiveHelp(
		IOleInPlaceSite *This, WINBOOL fEnterMode) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return E_NOTIMPL;
}

/*** IOleInPlaceSite methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_CanInPlaceActivate(
		IOleInPlaceSite *This) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_OnInPlaceActivate(
		IOleInPlaceSite *This) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_OnUIActivate(IOleInPlaceSite *This) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_GetWindowContext(
		IOleInPlaceSite *This, IOleInPlaceFrame **ppFrame,
		IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect,
		LPOLEINPLACEFRAMEINFO lpFrameInfo) {
	_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	_w_browser *br = W_IE_WEB_SITE_GET_BROWSER(site);
	lpFrameInfo->hwndFrame = _W_WIDGET(br)->handle;
	lpFrameInfo->fMDIApp = TRUE;
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;
	GetClientRect(_W_WIDGET(br)->handle, lprcClipRect);
	memcpy(lprcPosRect, lprcClipRect, sizeof(RECT));
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_Scroll(IOleInPlaceSite *This,
		SIZE scrollExtant) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_OnUIDeactivate(
		IOleInPlaceSite *This, WINBOOL fUndoable) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_OnInPlaceDeactivate(
		IOleInPlaceSite *This) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_DiscardUndoState(
		IOleInPlaceSite *This) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_DeactivateAndUndo(
		IOleInPlaceSite *This) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleInPlaceSite_OnPosRectChange(
		IOleInPlaceSite *This, LPCRECT lprcPosRect) {
	//_w_ie_web_site *site = IE_IOleInPlaceSite_SITE(This);
	return E_NOTIMPL;
}
IOleInPlaceSiteVtbl IE_IOleInPlaceSite_Vtbl =
		{ IE_IOleInPlaceSite_QueryInterface, IE_IOleInPlaceSite_AddRef,
				IE_IOleInPlaceSite_Release, IE_IOleInPlaceSite_GetWindow,
				IE_IOleInPlaceSite_ContextSensitiveHelp,
				IE_IOleInPlaceSite_CanInPlaceActivate,
				IE_IOleInPlaceSite_OnInPlaceActivate,
				IE_IOleInPlaceSite_OnUIActivate,
				IE_IOleInPlaceSite_GetWindowContext, IE_IOleInPlaceSite_Scroll,
				IE_IOleInPlaceSite_OnUIDeactivate,
				IE_IOleInPlaceSite_OnInPlaceDeactivate,
				IE_IOleInPlaceSite_DiscardUndoState,
				IE_IOleInPlaceSite_DeactivateAndUndo,
				IE_IOleInPlaceSite_OnPosRectChange };
#define IE_IOleDocumentSite_SITE(This) ((_w_ie_web_site *) ((void*)This - 4* sizeof(void*)))
/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleDocumentSite_QueryInterface(
		IOleDocumentSite *This,
		REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IOleDocumentSite_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IOleDocumentSite_AddRef(IOleDocumentSite *This) {
	_w_ie_web_site *site = IE_IOleDocumentSite_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IOleDocumentSite_Release(IOleDocumentSite *This) {
	_w_ie_web_site *site = IE_IOleDocumentSite_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IOleDocumentSite methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleDocumentSite_ActivateMe(IOleDocumentSite *This,
		IOleDocumentView *pViewToActivate) {
	//_w_ie_web_site *site = IE_IOleDocumentSite_SITE(This);
	return E_NOTIMPL;
}
IOleDocumentSiteVtbl IE_IOleDocumentSite_Vtbl = {
		IE_IOleDocumentSite_QueryInterface, IE_IOleDocumentSite_AddRef,
		IE_IOleDocumentSite_Release, IE_IOleDocumentSite_ActivateMe };

/*** IUnknown methods ***/
#define IE_IDocHostUIHandler_SITE(This) ((_w_ie_web_site *) ((void*)This - 5* sizeof(void*)))
HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_QueryInterface(
		IDocHostUIHandler *This,
		REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IDocHostUIHandler_AddRef(IDocHostUIHandler *This) {
	_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IDocHostUIHandler_Release(IDocHostUIHandler *This) {
	_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IDocHostUIHandler methods ***/
HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_ShowContextMenu(
		IDocHostUIHandler *This, DWORD dwID, POINT *ppt,
		IUnknown *pcmdtReserved, IDispatch *pdispReserved) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_GetHostInfo(
		IDocHostUIHandler *This, DOCHOSTUIINFO *pInfo) {
	_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_ShowUI(IDocHostUIHandler *This,
		DWORD dwID, IOleInPlaceActiveObject *pActiveObject,
		IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame,
		IOleInPlaceUIWindow *pDoc) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_HideUI(IDocHostUIHandler *This) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_UpdateUI(IDocHostUIHandler *This) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_EnableModeless(
		IDocHostUIHandler *This, WINBOOL fEnable) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_OnDocWindowActivate(
		IDocHostUIHandler *This, WINBOOL fActivate) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_OnFrameWindowActivate(
		IDocHostUIHandler *This, WINBOOL fActivate) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_ResizeBorder(
		IDocHostUIHandler *This, LPCRECT prcBorder,
		IOleInPlaceUIWindow *pUIWindow, WINBOOL fRameWindow) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_TranslateAccelerator(
		IDocHostUIHandler *This, LPMSG lpMsg, const GUID *pguidCmdGroup,
		DWORD nCmdID) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_GetOptionKeyPath(
		IDocHostUIHandler *This, LPOLESTR *pchKey, DWORD dw) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_GetDropTarget(
		IDocHostUIHandler *This, IDropTarget *pDropTarget,
		IDropTarget **ppDropTarget) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_GetExternal(
		IDocHostUIHandler *This, IDispatch **ppDispatch) {
	_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	*ppDispatch = (IDispatch*) &site->iDispatch;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_TranslateUrl(
		IDocHostUIHandler *This, DWORD dwTranslate, LPWSTR pchURLIn,
		LPWSTR *ppchURLOut) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostUIHandler_FilterDataObject(
		IDocHostUIHandler *This, IDataObject *pDO, IDataObject **ppDORet) {
	//_w_ie_web_site *site = IE_IDocHostUIHandler_SITE(This);
	return E_NOTIMPL;
}
IDocHostUIHandlerVtbl IE_IDocHostUIHandler_Vtbl =
		{ IE_IDocHostUIHandler_QueryInterface, IE_IDocHostUIHandler_AddRef,
				IE_IDocHostUIHandler_Release,
				IE_IDocHostUIHandler_ShowContextMenu,
				IE_IDocHostUIHandler_GetHostInfo, IE_IDocHostUIHandler_ShowUI,
				IE_IDocHostUIHandler_HideUI, IE_IDocHostUIHandler_UpdateUI,
				IE_IDocHostUIHandler_EnableModeless,
				IE_IDocHostUIHandler_OnDocWindowActivate,
				IE_IDocHostUIHandler_OnFrameWindowActivate,
				IE_IDocHostUIHandler_ResizeBorder,
				IE_IDocHostUIHandler_TranslateAccelerator,
				IE_IDocHostUIHandler_GetOptionKeyPath,
				IE_IDocHostUIHandler_GetDropTarget,
				IE_IDocHostUIHandler_GetExternal,
				IE_IDocHostUIHandler_TranslateUrl,
				IE_IDocHostUIHandler_FilterDataObject };
#define IE_IDocHostShowUI_SITE(This) ((_w_ie_web_site *) ((void*)This -6 *  sizeof(void*)))
/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE IE_IDocHostShowUI_QueryInterface(IDocHostShowUI *This,
REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IDocHostShowUI_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IDocHostShowUI_AddRef(IDocHostShowUI *This) {
	_w_ie_web_site *site = IE_IDocHostShowUI_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IDocHostShowUI_Release(IDocHostShowUI *This) {
	_w_ie_web_site *site = IE_IDocHostShowUI_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IDocHostShowUI methods ***/
HRESULT STDMETHODCALLTYPE IE_IDocHostShowUI_ShowMessage(IDocHostShowUI *This,
		HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption, DWORD dwType,
		LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult) {
	//_w_ie_web_site *site = IE_IDocHostShowUI_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDocHostShowUI_ShowHelp(IDocHostShowUI *This,
		HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand, DWORD dwData,
		POINT ptMouse, IDispatch *pDispatchObjectHit) {
	//_w_ie_web_site *site = IE_IDocHostShowUI_SITE(This);
	return E_NOTIMPL;
}
IDocHostShowUIVtbl IE_IDocHostShowUI_Vtbl = { IE_IDocHostShowUI_QueryInterface,
		IE_IDocHostShowUI_AddRef, IE_IDocHostShowUI_Release,
		IE_IDocHostShowUI_ShowMessage, IE_IDocHostShowUI_ShowHelp };
/*** IUnknown methods ***/
#define IE_IServiceProvider_SITE(This) ((_w_ie_web_site *) ((void*)This - 7 * sizeof(void*)))
HRESULT STDMETHODCALLTYPE IE_IServiceProvider_QueryInterface(
		IServiceProvider *This,
		REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IServiceProvider_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IServiceProvider_AddRef(IServiceProvider *This) {
	_w_ie_web_site *site = IE_IServiceProvider_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IServiceProvider_Release(IServiceProvider *This) {
	_w_ie_web_site *site = IE_IServiceProvider_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IServiceProvider methods ***/
HRESULT STDMETHODCALLTYPE IE_IServiceProvider_QueryService(
		IServiceProvider *This,
		REFGUID guidService,
		REFIID riid, void **ppvObject) {
	//_w_ie_web_site *site = IE_IServiceProvider_SITE(This);
	return E_NOTIMPL;
}
IServiceProviderVtbl IE_IServiceProvider_Vtbl = {
		IE_IServiceProvider_QueryInterface, IE_IServiceProvider_AddRef,
		IE_IServiceProvider_Release, IE_IServiceProvider_QueryService };

/*** IUnknown methods ***/
#define IE_IInternetSecurityManager_SITE(This) ((_w_ie_web_site *) ((void*)This - 8 * sizeof(void*)))
HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_QueryInterface(
		IInternetSecurityManager *This,
		REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IInternetSecurityManager_AddRef(
		IInternetSecurityManager *This) {
	_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IInternetSecurityManager_Release(
		IInternetSecurityManager *This) {
	_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IInternetSecurityManager methods ***/
HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_SetSecuritySite(
		IInternetSecurityManager *This, IInternetSecurityMgrSite *pSite) {
	//_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_GetSecuritySite(
		IInternetSecurityManager *This, IInternetSecurityMgrSite **ppSite) {
	//_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_MapUrlToZone(
		IInternetSecurityManager *This, LPCWSTR pwszUrl, DWORD *pdwZone,
		DWORD dwFlags) {
	//_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_GetSecurityId(
		IInternetSecurityManager *This, LPCWSTR pwszUrl, BYTE *pbSecurityId,
		DWORD *pcbSecurityId, DWORD_PTR dwReserved) {
	//_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_ProcessUrlAction(
		IInternetSecurityManager *This, LPCWSTR pwszUrl, DWORD dwAction,
		BYTE *pPolicy, DWORD cbPolicy, BYTE *pContext, DWORD cbContext,
		DWORD dwFlags, DWORD dwReserved) {
	//_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_QueryCustomPolicy(
		IInternetSecurityManager *This, LPCWSTR pwszUrl,
		REFGUID guidKey, BYTE **ppPolicy, DWORD *pcbPolicy, BYTE *pContext,
		DWORD cbContext, DWORD dwReserved) {
	//_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_SetZoneMapping(
		IInternetSecurityManager *This, DWORD dwZone, LPCWSTR lpszPattern,
		DWORD dwFlags) {
	//_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IInternetSecurityManager_GetZoneMappings(
		IInternetSecurityManager *This, DWORD dwZone,
		IEnumString **ppenumString, DWORD dwFlags) {
	//_w_ie_web_site *site = IE_IInternetSecurityManager_SITE(This);
	return E_NOTIMPL;
}
IInternetSecurityManagerVtbl IE_IInternetSecurityManager_Vtbl = {
		IE_IInternetSecurityManager_QueryInterface,
		IE_IInternetSecurityManager_AddRef, IE_IInternetSecurityManager_Release,
		IE_IInternetSecurityManager_SetSecuritySite,
		IE_IInternetSecurityManager_GetSecuritySite,
		IE_IInternetSecurityManager_MapUrlToZone,
		IE_IInternetSecurityManager_GetSecurityId,
		IE_IInternetSecurityManager_ProcessUrlAction,
		IE_IInternetSecurityManager_QueryCustomPolicy,
		IE_IInternetSecurityManager_SetZoneMapping,
		IE_IInternetSecurityManager_GetZoneMappings };
/*** IUnknown methods ***/
#define IE_IOleCommandTarget_SITE(This) ((_w_ie_web_site *) ((void*)This - 9 * sizeof(void*)))
HRESULT STDMETHODCALLTYPE IE_IOleCommandTarget_QueryInterface(
		IOleCommandTarget *This,
		REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IOleCommandTarget_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IOleCommandTarget_AddRef(IOleCommandTarget *This) {
	_w_ie_web_site *site = IE_IOleCommandTarget_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IOleCommandTarget_Release(IOleCommandTarget *This) {
	_w_ie_web_site *site = IE_IOleCommandTarget_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IOleCommandTarget methods ***/
HRESULT STDMETHODCALLTYPE IE_IOleCommandTarget_QueryStatus(
		IOleCommandTarget *This, const GUID *pguidCmdGroup, ULONG cCmds,
		OLECMD prgCmds[], OLECMDTEXT *pCmdText) {
	//_w_ie_web_site *site = IE_IOleCommandTarget_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IOleCommandTarget_Exec(IOleCommandTarget *This,
		const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt,
		VARIANT *pvaIn, VARIANT *pvaOut) {
	//_w_ie_web_site *site = IE_IOleCommandTarget_SITE(This);
	return E_NOTIMPL;
}
IOleCommandTargetVtbl IE_IOleCommandTarget_Vtbl = {
		IE_IOleCommandTarget_QueryInterface, IE_IOleCommandTarget_AddRef,
		IE_IOleCommandTarget_Release, IE_IOleCommandTarget_QueryStatus,
		IE_IOleCommandTarget_Exec };

/*** IUnknown methods ***/
#define IE_IAuthenticate_SITE(This) ((_w_ie_web_site *) ((void*)This - 10 * sizeof(void*)))
HRESULT STDMETHODCALLTYPE IE_IAuthenticate_QueryInterface(IAuthenticate *This,
REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IAuthenticate_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IAuthenticate_AddRef(IAuthenticate *This) {
	_w_ie_web_site *site = IE_IAuthenticate_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IAuthenticate_Release(IAuthenticate *This) {
	_w_ie_web_site *site = IE_IAuthenticate_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IAuthenticate methods ***/
HRESULT STDMETHODCALLTYPE IE_IAuthenticate_Authenticate(IAuthenticate *This,
		HWND *phwnd, LPWSTR *pszUsername, LPWSTR *pszPassword) {
	//_w_ie_web_site *site = IE_IAuthenticate_SITE(This);
	return E_NOTIMPL;
}
IAuthenticateVtbl IE_IAuthenticate_Vtbl = { IE_IAuthenticate_QueryInterface,
		IE_IAuthenticate_AddRef, IE_IAuthenticate_Release,
		IE_IAuthenticate_Authenticate };

/*** IUnknown methods ***/
#define IE_IDispatch_SITE(This) ((_w_ie_web_site *) ((void*)This - 11 * sizeof(void*)))
const DISPID kInvokeId = 1000;
HRESULT STDMETHODCALLTYPE IE_IDispatch_QueryInterface(IDispatch *This,
REFIID riid, void **ppvObject) {
	_w_ie_web_site *site = IE_IDispatch_SITE(This);
	return IE_IOleClientSite_QueryInterface((IOleClientSite*) site, riid,
			ppvObject);
}

ULONG STDMETHODCALLTYPE IE_IDispatch_AddRef(IDispatch *This) {
	_w_ie_web_site *site = IE_IDispatch_SITE(This);
	return IE_IOleClientSite_AddRef((IOleClientSite*) site);
}

ULONG STDMETHODCALLTYPE IE_IDispatch_Release(IDispatch *This) {
	_w_ie_web_site *site = IE_IDispatch_SITE(This);
	return IE_IOleClientSite_Release((IOleClientSite*) site);
}

/*** IDispatch methods ***/
HRESULT STDMETHODCALLTYPE IE_IDispatch_GetTypeInfoCount(IDispatch *This,
		UINT *pctinfo) {
	//_w_ie_web_site *site = IE_IDispatch_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDispatch_GetTypeInfo(IDispatch *This, UINT iTInfo,
		LCID lcid, ITypeInfo **ppTInfo) {
	//_w_ie_web_site *site = IE_IDispatch_SITE(This);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IE_IDispatch_GetIDsOfNames(IDispatch *This,
REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) {
	//_w_ie_web_site *site = IE_IDispatch_SITE(This);
	HRESULT res = DISP_E_UNKNOWNNAME;
	/*struct UnicodeString str;
	 unicode_set(&str, "postMessage", -1);
	 if (cNames == 1 && lstrcmpW(rgszNames, str.str)) {
	 rgDispId[0] = kInvokeId;
	 res = S_OK;
	 }
	 unicode_free(&str);*/
	return res;
}

HRESULT STDMETHODCALLTYPE IE_IDispatch_Invoke(IDispatch *This,
		DISPID dispIdMember,
		REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams,
		VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) {
	_w_ie_web_site *site = IE_IDispatch_SITE(This);
	HRESULT hr = S_OK;
	switch (dispIdMember) {
	case DISPID_NAVIGATECOMPLETE2:
	case DISPID_NAVIGATECOMPLETE:
		break;
	default:
		hr = E_NOTIMPL;
		break;
	}
	return hr;
}
IDispatchVtbl IE_IDispatch_Vtbl = { IE_IDispatch_QueryInterface,
		IE_IDispatch_AddRef, IE_IDispatch_Release,
		IE_IDispatch_GetTypeInfoCount, IE_IDispatch_GetTypeInfo,
		IE_IDispatch_GetIDsOfNames, IE_IDispatch_Invoke };
void _w_ie_web_site_init(_w_ie_web_site *site) {
	site->iOleClientSite = &IE_IOleClientSite_Vtbl;
	site->iOleControlSite = &IE_IOleControlSite_Vtbl;
	site->iAdviseSink = &IE_IAdviseSink_Vtbl;
	site->iOleInPlaceSite = &IE_IOleInPlaceSite_Vtbl;
	site->iOleDocumentSite = &IE_IOleDocumentSite_Vtbl;
	site->iDocHostUIHandler = &IE_IDocHostUIHandler_Vtbl;
	site->iDocHostShowUI = &IE_IDocHostShowUI_Vtbl;
	site->iServiceProvider = &IE_IServiceProvider_Vtbl;
	site->iInternetSecurityManager = &IE_IInternetSecurityManager_Vtbl;
	site->iOleCommandTarget = &IE_IOleCommandTarget_Vtbl;
	site->iAuthenticate = &IE_IAuthenticate_Vtbl;
	site->iDispatch = &IE_IDispatch_Vtbl;
}
#endif
