/*
 * browser.h
 *
 *  Created on: 12 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_BROWSER_IE_H_
#define SRC_WIN32_CONTROLS_BROWSER_IE_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include <exdisp.h>
#include <exdispid.h>
#include <ole2.h>
typedef struct _w_ie_web_site {
	IOleClientSiteVtbl *iOleClientSite;
	IOleControlSiteVtbl *iOleControlSite;
	IAdviseSinkVtbl *iAdviseSink;
	IOleInPlaceSiteVtbl *iOleInPlaceSite;
	IOleDocumentSiteVtbl *iOleDocumentSite;
	IDocHostUIHandlerVtbl *iDocHostUIHandler;
	IDocHostShowUIVtbl *iDocHostShowUI;
	IServiceProviderVtbl *iServiceProvider;
	IInternetSecurityManagerVtbl *iInternetSecurityManager;
	IOleCommandTargetVtbl *iOleCommandTarget;
	IAuthenticateVtbl *iAuthenticate;
	IDispatchVtbl *iDispatch;
	IWebBrowser* browser;

} _w_ie_web_site;
typedef struct _w_browser {
	struct _w_composite composite;
	_w_ie_web_site site;
} _w_browser;

struct _w_browser_reserved {
	struct _w_composite_reserved composite;
	_w_ie_web_site site;

};
#define W_IE_WEB_SITE_GET_BROWSER(site) ((struct _w_browser*)((void*)site - sizeof(struct _w_composite)))
#define _W_BROWSER_IE(x) ((_w_browser*)x)
#define _W_BROWSER_IE_RESERVED(x) ((struct _w_browser_reserved*)x)
void _w_ie_web_site_init(_w_ie_web_site *site);
void _w_browser_class_init(struct _w_browser_class *clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_BROWSER_IE_H_ */
