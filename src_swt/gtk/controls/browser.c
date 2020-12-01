/*
 * browser.c
 *
 *  Created on: 11 août 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "browser.h"
#include "../widgets/toolkit.h"
#include <dlfcn.h>
#define  ABOUT_BLANK "about:blank"
#define   URI_FILEROOT "file:///"
/*
 *
 */
#define WEBKIT_VERSION_1 (1 << 1)
#define WEBKIT_VERSION_2 (1 << 2)
#define WEBKIT_VERSION_3 (1 << 3)
#define WEBKIT_VERSION_ALL (WEBKIT_VERSION_1 | WEBKIT_VERSION_2 | WEBKIT_VERSION_3)

#define WEBKIT_IGNORETLS (1 << 4)
const char *webkit_function_name[] = { //
		"webkit_web_view_new", //
				"webkit_web_view_get_type", //
				"webkit_web_view_load_uri", //
				"webkit_web_view_load_html_string", //
				"webkit_web_view_load_html", //
		};
const unsigned char webkit_function_version[] = { //
		WEBKIT_VERSION_ALL, //webkit_web_view_new
				WEBKIT_VERSION_ALL, //webkit_web_view_get_type
				WEBKIT_VERSION_ALL, //webkit_web_view_load_uri
				WEBKIT_VERSION_3, //webkit_web_view_load_html_string
				WEBKIT_VERSION_2, //webkit_web_view_load_html
		};
GType webkit_web_view_get_type(void) {
	return _W_BROWSER_KIT_RESERVED(&gtk_toolkit->class_browser_reserved)->webkit_web_view_get_type();
}
/*
 * public
 */
wresult _w_browser_kit_back(w_browser *browser) {
}
wresult _w_browser_kit_execute(w_browser *browser, const char *script) {
}
wresult _w_browser_kit_close(w_browser *browser) {
}
wresult _w_browser_kit_evaluate(w_browser *browser, const char *script,
		w_value *result) {
}
wresult _w_browser_kit_forward(w_browser *browser) {
}
const char* _w_browser_kit_get_browser_type(w_browser *browser) {
	return "WebKit";
}
wresult _w_browser_kit_get_javascript_enabled(w_browser *browser) {
}
wresult _w_browser_kit_get_text(w_browser *browser, w_alloc *text) {
}
wresult _w_browser_kit_get_url(w_browser *browser, w_alloc *url) {
}
wresult _w_browser_kit_is_back_enabled(w_browser *browser) {
}
wresult _w_browser_kit_is_forward_enabled(w_browser *browser) {
}
wresult _w_browser_kit_refresh(w_browser *browser) {
}
wresult _w_browser_kit_set_javascript_enabled(w_browser *browser,
		int enabled) {
}

wresult _w_browser_kit_set_text(w_browser *browser, const char *html_bytes,
		int trusted) {
	struct _w_browser_kit_reserved *reserved = _W_BROWSER_KIT_RESERVED(
			_w_widget_get_reserved(W_WIDGET(browser)));
	const char *uriBytes;

	/*
	 * If this.htmlBytes is not null then the about:blank page is already being loaded,
	 * so no navigate is required.  Just set the html that is to be shown.
	 */
	int blankLoading = _W_BROWSER_KIT(browser)->htmlBytes != 0; // Webkit1 only.
	if (reserved->version & WEBKIT_VERSION_1) {
		/*this.htmlBytes = html_bytes;
		 untrustedText = !trusted;*/
	}

	if (reserved->version & WEBKIT_VERSION_2) {
		//w2_bug527738LastRequestCounter.incrementAndGet();
		if (!trusted) {
			uriBytes = ABOUT_BLANK;
		} else {
			uriBytes = URI_FILEROOT;
		}
		reserved->webkit_web_view_load_html(_W_WIDGET(browser)->handle,
				html_bytes, uriBytes);
	} else {
		if (blankLoading)
			return W_TRUE;

		uriBytes = ABOUT_BLANK;
		reserved->webkit_web_view_load_uri(_W_WIDGET(browser)->handle,
				uriBytes);
	}

	return W_TRUE;
}
wresult _w_browser_kit_set_url(w_browser *browser, const char *url,
		const char *postData, const char *headers) {
	struct _w_browser_kit_reserved *reserved = _W_BROWSER_KIT_RESERVED(
			_w_widget_get_reserved(W_WIDGET(browser)));
	reserved->webkit_web_view_load_uri(
	_W_WIDGET(browser)->handle, url);
	return W_TRUE;
}
wresult _w_browser_kit_stop(w_browser *browser) {
}
/*
 *
 */
wresult _w_browser_kit_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	struct _w_browser_kit_reserved *reserved = _W_BROWSER_KIT_RESERVED(
			_w_widget_get_reserved(widget));
	GtkWidget *fixedHandle, *scrolledWindow = 0, *webView = 0;
	fixedHandle = (GtkWidget*) _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	/*scrolledWindow = gtk_scrolled_window_new(0, 0);
	 if (scrolledWindow == 0)
	 goto _err;
	 gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
	 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	 webView = reserved->webkit_web_view_new();
	 if (webView == 0)
	 goto _err;
	 //webViewData = C.malloc(C.PTR_SIZEOF);
	 //C.memmove (webViewData, new int [] {webView}, C.PTR_SIZEOF);
	 //gtk_container_add(GTK_CONTAINER(scrolledWindow), webView);
	 //gtk_container_add(GTK_CONTAINER(fixedHandle), scrolledWindow);
	 gtk_container_add(GTK_CONTAINER(fixedHandle), webView);
	 g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	 //g_object_set_qdata(G_OBJECT(scrolledWindow), gtk_toolkit->quark[0], widget);
	 g_object_set_qdata(G_OBJECT(webView), gtk_toolkit->quark[0], widget);
	 _W_WIDGET(widget)->handle = webView;
	 gtk_widget_show_all(fixedHandle);
	 _w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	 return W_TRUE;
	 _err: return W_ERROR_NO_HANDLES;*/
	//int [] vers = internalGetWebkitVersion();
	if (reserved->ExternalClass == 0) {
		/*System.setProperty(SWT_WEBKITGTK_VERSION,
		 String.format("%s.%s.%s", vers[0], vers[1], vers[2])); // $NON-NLS-1$
		 if (Device.DEBUG) {
		 System.out.println(String.format("WebKit version %s.%s.%s", vers[0], vers[1], vers[2])); //$NON-NLS-1$
		 }*/

		if (reserved->version & WEBKIT_VERSION_1) { // 'external' object only used on webkit1 for javaCall. Webkit2 has a different mechanism.
			/*JSClassDefinition jsClassDefinition = new JSClassDefinition ();
			 byte[] bytes = Converter.wcsToMbcs (CLASSNAME_EXTERNAL, true);
			 jsClassDefinition.className = C.malloc (bytes.length);
			 C.memmove (jsClassDefinition.className, bytes, bytes.length);

			 jsClassDefinition.hasProperty = JSObjectHasPropertyProc.getAddress ();
			 jsClassDefinition.getProperty = JSObjectGetPropertyProc.getAddress ();
			 long classDefinitionPtr = C.malloc (JSClassDefinition.sizeof);
			 WebKitGTK.memmove (classDefinitionPtr, jsClassDefinition, JSClassDefinition.sizeof);

			 ExternalClass = WebKitGTK.JSClassCreate (classDefinitionPtr);*/
		}

		/*byte [] bytes = Converter.wcsToMbcs ("POST", true); //$NON-NLS-1$
		 PostString = C.malloc (bytes.length);
		 C.memmove (PostString, bytes, bytes.length);*/

		/*
		 * WebKitGTK version 1.8.x and newer can crash sporadically in
		 * webkitWebViewRegisterForIconNotification().  The root issue appears
		 * to be WebKitGTK accessing its icon database from a background
		 * thread.  Work around this crash by disabling the use of WebKitGTK's
		 * icon database, which should not affect the Browser in any way.
		 */
		if (reserved->version & WEBKIT_VERSION_1) {
			/*long database = WebKitGTK.webkit_get_favicon_database ();
			 if (database != 0) {*/
			/* WebKitGTK version is >= 1.8.x */
			/*WebKitGTK.webkit_favicon_database_set_path (database, 0);
			 }*/
		}
	}

	if (reserved->version & WEBKIT_VERSION_1) {
		/*scrolledWindow = GTK.gtk_scrolled_window_new (0, 0);
		 GTK.gtk_scrolled_window_set_policy (scrolledWindow, GTK.GTK_POLICY_AUTOMATIC, GTK.GTK_POLICY_AUTOMATIC);*/
	}

	webView = reserved->webkit_web_view_new();

	// Bug 522733 Webkit2 workaround for crash
	//   As of Webkitgtk 2.18, webkitgtk2 crashes if the first instance of webview is not referenced when JVM shuts down.
	//   There is a exit handler that tries to dereference the first instance [which if not referenced]
	//   leads to a crash. This workaround would benefit from deeper investigation (find root cause etc...).
	// [edit] Bug 530678. Note, it seems that as of Webkit2.18, webkit auto-disposes itself if parent get's disposed.
	//        While not directly related, see onDispose() for how to deal with disposal of this.
	/*if ((reserved->version & WEBKIT_VERSION_2) && !bug522733FirstInstanceCreated && vers[0] == 2 && vers[1] >= 18) {
	 bug522733FirstInstanceCreated = true;
	 OS.g_object_ref(webView);
	 }*/
	if (reserved->version & WEBKIT_IGNORETLS) {
		/*WebKitGTK.webkit_web_context_set_tls_errors_policy(WebKitGTK.webkit_web_view_get_context(webView),
		 WebKitGTK.WEBKIT_TLS_ERRORS_POLICY_IGNORE);*/
		printf(
				"***WARNING: WebKitGTK is configured to ignore TLS errors via -Dorg.eclipse.swt.internal.webkitgtk.ignoretlserrors=true .\n");
		printf("***WARNING: Please use for development purposes only!\n");
	}

	if (reserved->version & WEBKIT_VERSION_1) {
		/*webViewData = C.malloc (C.PTR_SIZEOF);
		 C.memmove (webViewData, new long[] {webView}, C.PTR_SIZEOF);*/
	}

	// Documentation for these signals/properties is usually found under signal/property of WebKitWebView.
	// notify_* usually implies a property change. For these, the first arg is typically the webview handle.
	if (reserved->version & WEBKIT_VERSION_1) {
		// Webkit1 signal documentation: https://webkitgtk.org/reference/webkitgtk/unstable/webkitgtk-webkitwebview.html#WebKitWebView--progress
		/*GTK.gtk_container_add (scrolledWindow, webView);
		 GTK.gtk_container_add (browser.handle, scrolledWindow);
		 GTK.gtk_widget_show (scrolledWindow);
		 OS.g_signal_connect (webView, WebKitGTK.close_web_view, 			Proc2.getAddress (), CLOSE_WEB_VIEW);
		 OS.g_signal_connect (webView, WebKitGTK.web_view_ready, 			Proc2.getAddress (), WEB_VIEW_READY);

		 OS.g_signal_connect (webView, WebKitGTK.hovering_over_link, 		Proc4.getAddress (), HOVERING_OVER_LINK);

		 OS.g_signal_connect (webView, WebKitGTK.window_object_cleared, 		Proc5.getAddress (), WINDOW_OBJECT_CLEARED);
		 OS.g_signal_connect (webView, WebKitGTK.console_message, 			Proc5.getAddress (), CONSOLE_MESSAGE);

		 OS.g_signal_connect (webView, WebKitGTK.navigation_policy_decision_requested, 	Proc6.getAddress (), NAVIGATION_POLICY_DECISION_REQUESTED);
		 OS.g_signal_connect (webView, WebKitGTK.mime_type_policy_decision_requested, 	Proc6.getAddress (), MIME_TYPE_POLICY_DECISION_REQUESTED);
		 OS.g_signal_connect (webView, WebKitGTK.resource_request_starting, Proc6.getAddress (), RESOURCE_REQUEST_STARTING);*/

	} else {
		// Webkit2 Signal Documentation: https://webkitgtk.org/reference/webkit2gtk/stable/WebKitWebView.html#WebKitWebView--title
		gtk_container_add(GTK_CONTAINER(fixedHandle), webView);
		/*OS.g_signal_connect (webView, WebKitGTK.close, Proc2.getAddress (), CLOSE_WEB_VIEW);
		 OS.g_signal_connect (webView, WebKitGTK.ready_to_show, Proc2.getAddress (), WEB_VIEW_READY);
		 OS.g_signal_connect (webView, WebKitGTK.decide_policy, Proc4.getAddress (), DECIDE_POLICY);

		 OS.g_signal_connect (webView, WebKitGTK.mouse_target_changed, Proc4.getAddress (), MOUSE_TARGET_CHANGED);
		 OS.g_signal_connect (webView, WebKitGTK.context_menu, Proc5.getAddress (), CONTEXT_MENU);
		 OS.g_signal_connect (webView, WebKitGTK.load_failed_with_tls_errors, Proc5.getAddress (), LOAD_FAILED_TLS);*/
	}

	// Proc3 is overloaded in that not only Webview connects to it,
	// but also (webkit1) WebFrame and (webkit2) WebKitDownload hook into it as well.
	// Pay extra attention to argument 1 (handle) to prevent wrong type of handle being passed to gtk and causing segfaults. (See 533545)
	if (reserved->version & WEBKIT_VERSION_1) {
		// WebKitWebView* user_function (WebKitWebView  *web_view, WebKitWebFrame *frame, gpointer user_data)
		/*OS.g_signal_connect (webView, WebKitGTK.create_web_view, 		 Proc3.getAddress (), CREATE_WEB_VIEW);

		 // Property change: load-status  (webview is first arg)  https://webkitgtk.org/reference/webkitgtk/unstable/WebKitWebFrame.html#WebKitWebFrame--load-status
		 OS.g_signal_connect (webView, WebKitGTK.notify_load_status, 	 Proc3.getAddress (), NOTIFY_LOAD_STATUS);

		 // gboolean user_function (WebKitWebView  *web_view, WebKitDownload *download, gpointer        user_data)
		 OS.g_signal_connect (webView, WebKitGTK.download_requested, 	 Proc3.getAddress (), DOWNLOAD_REQUESTED);

		 // void user_function (WebKitWebView *web_view, GtkMenu *menu, gpointer user_data)
		 OS.g_signal_connect (webView, WebKitGTK.populate_popup, 		 Proc3.getAddress (), POPULATE_POPUP);

		 // Property change: progress.  (first arg is webview)
		 OS.g_signal_connect (webView, WebKitGTK.notify_progress, 		 Proc3.getAddress (), NOTIFY_PROGRESS);

		 // void user_function (WebKitWebView *webkitwebview, gchar  *arg1, gpointer user_data)
		 OS.g_signal_connect (webView, WebKitGTK.status_bar_text_changed, Proc3.getAddress (), STATUS_BAR_TEXT_CHANGED);*/

	}
	if (reserved->version & WEBKIT_VERSION_2) { // Note: In Webkit2, webkit_download_started(...) also connects return signals to proc3.
		// GtkWidget* user_function (WebKitWebView *web_view, WebKitNavigationAction *navigation_action,  gpointer  user_data)
		/*OS.g_signal_connect (webView, WebKitGTK.create, 						Proc3.getAddress (), CREATE_WEB_VIEW);

		 //void user_function (WebKitWebView  *web_view,  WebKitLoadEvent load_event,  gpointer  user_data)
		 OS.g_signal_connect (webView, WebKitGTK.load_changed, 					Proc3.getAddress (), LOAD_CHANGED);

		 // Property change: of 'estimated-load-progress'   args: webview, pspec
		 OS.g_signal_connect (webView, WebKitGTK.notify_estimated_load_progress, Proc3.getAddress (), NOTIFY_PROGRESS);

		 // gboolean user_function (WebKitWebView *web_view,  WebKitAuthenticationRequest *request,  gpointer user_data)
		 OS.g_signal_connect (webView, WebKitGTK.authenticate, 					Proc3.getAddress (), AUTHENTICATE);

		 // (!) Note this one's a 'webContext' signal, not webview. See:
		 // https://webkitgtk.org/reference/webkit2gtk/stable/WebKitWebContext.html#WebKitWebContext-download-started
		 OS.g_signal_connect (WebKitGTK.webkit_web_context_get_default(), WebKitGTK.download_started, Proc3.getAddress (), DOWNLOAD_STARTED);*/
	}

	// Webview 'title' property. Webkit1 & Webkit2.
	//OS.g_signal_connect (webView, WebKitGTK.notify_title, 						Proc3.getAddress (), NOTIFY_TITLE);

	/* Callback to get events before WebKit receives and consumes them */
	if (reserved->version & WEBKIT_VERSION_2) {
		/*OS.g_signal_connect (webView, OS.button_press_event, JSDOMEventProc.getAddress (), WIDGET_EVENT);
		 OS.g_signal_connect (webView, OS.button_release_event, JSDOMEventProc.getAddress (), WIDGET_EVENT);
		 OS.g_signal_connect (webView, OS.focus_in_event, JSDOMEventProc.getAddress (), 	WIDGET_EVENT);
		 OS.g_signal_connect (webView, OS.focus_out_event, JSDOMEventProc.getAddress (), WIDGET_EVENT);*/
		// if connecting any other special gtk event to webkit, add SWT.* to w2_passThroughSwtEvents above.
	}
	/*this.pageId = WebKitGTK.webkit_web_view_get_page_id (webView);
	 if (reserved->version & WEBKIT_VERSION_1) {
	 OS.g_signal_connect (webView, OS.button_press_event, JSDOMEventProc.getAddress (), 0);
	 OS.g_signal_connect (webView, OS.button_release_event, JSDOMEventProc.getAddress (), 0);
	 }
	 OS.g_signal_connect (webView, OS.key_press_event, JSDOMEventProc.getAddress (),  	WIDGET_EVENT);
	 OS.g_signal_connect (webView, OS.key_release_event, JSDOMEventProc.getAddress (),	WIDGET_EVENT);
	 OS.g_signal_connect (webView, OS.scroll_event, JSDOMEventProc.getAddress (), 		WIDGET_EVENT);
	 OS.g_signal_connect (webView, OS.motion_notify_event, JSDOMEventProc.getAddress (), WIDGET_EVENT);*/

	/*
	 * Callbacks to get the events not consumed by WebKit, and to block
	 * them so that they don't get propagated to the parent handle twice.
	 * This hook is set after WebKit and is therefore called after WebKit's
	 * handler because GTK dispatches events in their order of registration.
	 */
	if (reserved->version & WEBKIT_VERSION_1) {
		/*OS.g_signal_connect (scrolledWindow, OS.button_press_event, JSDOMEventProc.getAddress (), STOP_PROPOGATE);
		 OS.g_signal_connect (scrolledWindow, OS.button_release_event, JSDOMEventProc.getAddress (), STOP_PROPOGATE);
		 OS.g_signal_connect (scrolledWindow, OS.key_press_event, JSDOMEventProc.getAddress (), STOP_PROPOGATE);
		 OS.g_signal_connect (scrolledWindow, OS.key_release_event, JSDOMEventProc.getAddress (), STOP_PROPOGATE);
		 OS.g_signal_connect (scrolledWindow, OS.scroll_event, JSDOMEventProc.getAddress (), STOP_PROPOGATE);
		 OS.g_signal_connect (scrolledWindow, OS.motion_notify_event, JSDOMEventProc.getAddress (), STOP_PROPOGATE);*/
	}

	/*long settings = WebKitGTK.webkit_web_view_get_settings (webView);
	 OS.g_object_set (settings, WebKitGTK.javascript_can_open_windows_automatically, 1, 0);
	 OS.g_object_set (settings, WebKitGTK.enable_webgl, 1, 0);

	 if (reserved->version & WEBKIT_VERSION_2){
	 OS.g_object_set (settings, WebKitGTK.default_charset, "UTF-8", 0);
	 if (WebKitGTK.webkit_get_minor_version() >= 14) {
	 OS.g_object_set (settings, WebKitGTK.allow_universal_access_from_file_urls, 1, 0);
	 } else {
	 System.err.println("SWT WEBKIT: Warning, you are using Webkitgtk below version 2.14. Your version is: "
	 + "Your version is: " + internalGetWebKitVersionStr()
	 + "\nJavascript execution limited to same origin due to unimplemented feature of this version.");
	 }
	 } else {
	 OS.g_object_set (settings, WebKitGTK.default_encoding, "UTF-8", 0);
	 OS.g_object_set (settings, WebKitGTK.enable_universal_access_from_file_uris, 1, 0);
	 }*/

	/*Listener listener = event -> {
	 switch (event.type) {
	 case SWT.Dispose: {*/
	/* make this handler run after other dispose listeners */
	/*if (ignoreDispose) {
	 ignoreDispose = false;
	 break;
	 }
	 ignoreDispose = true;
	 browser.notifyListeners (event.type, event);
	 event.type = SWT.NONE;
	 onDispose (event);
	 break;
	 }
	 case SWT.FocusIn: {
	 if (WEBKIT2 && webView != 0)
	 GTK.gtk_widget_grab_focus (webView);
	 break;
	 }
	 case SWT.Resize: {
	 onResize (event);
	 break;
	 }
	 }
	 };
	 browser.addListener (SWT.Dispose, listener);
	 browser.addListener (SWT.FocusIn, listener);
	 browser.addListener (SWT.KeyDown, listener);
	 browser.addListener (SWT.Resize, listener);*/

	if (reserved->version & WEBKIT_VERSION_1) {
		/*
		 * Ensure that our Authenticate listener is at the front of the signal
		 * queue by removing the default Authenticate listener, adding ours,
		 * and then re-adding the default listener.
		 */
		/*long session = WebKitGTK.webkit_get_default_session ();
		 long originalAuth = WebKitGTK.soup_session_get_feature (session, WebKitGTK.webkit_soup_auth_dialog_get_type ());
		 if (originalAuth != 0) {
		 WebKitGTK.soup_session_feature_detach (originalAuth, session);
		 }
		 OS.g_signal_connect (session, WebKitGTK.authenticate, Proc5.getAddress (), webView);
		 if (originalAuth != 0) {
		 WebKitGTK.soup_session_feature_attach (originalAuth, session);
		 }*/

		/*
		 * Check for proxy values set as documented java properties and update the
		 * session to use these values if needed.
		 */
		/*String proxyHost = System.getProperty (PROPERTY_PROXYHOST);
		 String proxyPortString = System.getProperty (PROPERTY_PROXYPORT);
		 int port = -1;
		 if (proxyPortString != null) {
		 try {
		 int value = Integer.valueOf (proxyPortString).intValue ();
		 if (0 <= value && value <= MAX_PORT) port = value;
		 } catch (NumberFormatException e) {*/
		/* do nothing, java property has non-integer value */
		/*}
		 }
		 if (proxyHost != null || port != -1) {
		 if (!proxyHost.startsWith (PROTOCOL_HTTP)) {
		 proxyHost = PROTOCOL_HTTP + proxyHost;
		 }
		 proxyHost += ":" + port; //$NON-NLS-1$
		 bytes = Converter.wcsToMbcs (proxyHost, true);
		 long uri = WebKitGTK.soup_uri_new (bytes);
		 if (uri != 0) {
		 OS.g_object_set (session, WebKitGTK.SOUP_SESSION_PROXY_URI, uri, 0);
		 WebKitGTK.soup_uri_free (uri);
		 }
		 }*/
	}

	if (reserved->version & WEBKIT_VERSION_1) { // HandleWebKitEvent registration. Pre Webkit 1.4 way of handling mouse/keyboard events. Webkit2 uses dom.
		/*eventFunction = new BrowserFunction (browser, "HandleWebKitEvent") { //$NON-NLS-1$
		 @Override
		 public Object function(Object[] arguments) {
		 return handleEventFromFunction (arguments) ? Boolean.TRUE : Boolean.FALSE;
		 }
		 };*/
	}

	/*
	 * Bug in WebKitGTK.  MouseOver/MouseLeave events are not consistently sent from
	 * the DOM when the mouse enters and exits the browser control, see
	 * https://bugs.webkit.org/show_bug.cgi?id=35246.  As a workaround for sending
	 * MouseEnter/MouseExit events, swt's default mouse enter/exit mechanism is used,
	 * but in order to do this the Browser's default sub-window check behavior must
	 * be changed.
	 */
	//browser.setData (KEY_CHECK_SUBWINDOW, Boolean.FALSE);
	/*
	 * Bug in WebKitGTK.  In WebKitGTK 1.10.x a crash can occur if an
	 * attempt is made to show a browser before a size has been set on
	 * it.  The workaround is to temporarily give it a size that forces
	 * the native resize events to fire.
	 */
	/*int major = vers[0], minor = vers[1];
	 if (major == 1 && minor >= 10) {
	 Rectangle minSize = browser.computeTrim (0, 0, 2, 2);
	 Point size = browser.getSize ();
	 size.x += minSize.width; size.y += minSize.height;
	 browser.setSize (size);
	 size.x -= minSize.width; size.y -= minSize.height;
	 browser.setSize (size);
	 }*/
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	if (scrolledWindow != 0)
		g_object_set_qdata(G_OBJECT(scrolledWindow), gtk_toolkit->quark[0],
				widget);
	g_object_set_qdata(G_OBJECT(webView), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = webView;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	gtk_widget_show_all(fixedHandle);
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
void _w_browser_kit_dispose(w_widget *widget) {

}
void _w_browser_kit_class_init(struct _w_browser_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BROWSER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_browser_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_browser);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_browser_kit);
	W_WIDGET_CLASS(clazz)->dispose = _w_browser_kit_dispose;
	//
	W_WIDGET_CLASS(clazz)->create = _w_browser_kit_create;
	clazz->back = _w_browser_kit_back;
	clazz->execute = _w_browser_kit_execute;
	clazz->close = _w_browser_kit_close;
	clazz->evaluate = _w_browser_kit_evaluate;
	clazz->forward = _w_browser_kit_forward;
	clazz->get_browser_type = _w_browser_kit_get_browser_type;
	clazz->get_javascript_enabled = _w_browser_kit_get_javascript_enabled;
	clazz->get_text = _w_browser_kit_get_text;
	clazz->get_url = _w_browser_kit_get_url;
	clazz->is_back_enabled = _w_browser_kit_is_back_enabled;
	clazz->is_forward_enabled = _w_browser_kit_is_forward_enabled;
	clazz->refresh = _w_browser_kit_refresh;
	clazz->set_javascript_enabled = _w_browser_kit_set_javascript_enabled;
	clazz->set_text = _w_browser_kit_set_text;
	clazz->set_url = _w_browser_kit_set_url;
	clazz->stop = _w_browser_kit_stop;
	/*
	 *
	 */
	struct _w_browser_reserved *reserved = _W_BROWSER_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hp;
	_W_CONTROL_RESERVED(reserved)->fixedHandle = _w_widget_hp;
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle = _w_widget_h0;

}
int _w_browser_kit_dlsym(struct _w_browser_kit_reserved *reserved,
		void *module) {
	void **function = reserved->function;
	unsigned char version = reserved->version;
	for (int i = 0;
			i < sizeof(webkit_function_name) / sizeof(webkit_function_name[0]);
			i++) {
		if (webkit_function_version[i] & version) {
			function[i] = dlsym(module, webkit_function_name[i]);
			if (function[i] == 0) {
				fprintf(stderr, "swt Error : not find symbol %s",
						webkit_function_name[i]);
				return W_FALSE;
			}
		} else {
			function[i] = 0;
		}
	}
	return W_TRUE;
}
int _w_browser_kit_verify(struct _w_browser_class *clazz) {
	void *module;
	struct _w_browser_kit_reserved *reserved = _W_BROWSER_KIT_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
#if GTK3
	module = dlopen("libwebkitgtk-3.0.so", RTLD_LAZY | RTLD_GLOBAL);
	reserved->version = WEBKIT_VERSION_3;
	if (module == 0) {
		module = dlopen("libwebkit2gtk-4.0.so.37", RTLD_LAZY | RTLD_GLOBAL);
		reserved->version = WEBKIT_VERSION_2;
	}
#else
	module = dlopen("libwebkitgtk-1.0.so.0", RTLD_LAZY | RTLD_GLOBAL);
	reserved->version = WEBKIT_VERSION_2;
#endif
	if (module) {
		return _w_browser_kit_dlsym(reserved, module);
	}
	return 0;
}
/*
 * public
 */
wresult _w_browser_back(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_execute(w_browser *browser, const char *script) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_close(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_evaluate(w_browser *browser, const char *script,
		w_value *result) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_forward(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
const char* _w_browser_get_browser_type(w_browser *browser) {
	return "empty";
}
wresult _w_browser_get_javascript_enabled(w_browser *browser) {
	return W_FALSE;
}
wresult _w_browser_get_text(w_browser *browser, w_alloc *text) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_get_url(w_browser *browser, w_alloc *url) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_is_back_enabled(w_browser *browser) {
	return W_FALSE;
}
wresult _w_browser_is_forward_enabled(w_browser *browser) {
	return W_FALSE;
}
wresult _w_browser_refresh(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_set_javascript_enabled(w_browser *browser, int enabled) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_set_text(w_browser *browser, const char *html,
		int trusted) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_set_url(w_browser *browser, const char *url,
		const char *postData, const char *headers) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_stop(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
/*
 *
 */
void _w_browser_dispose(w_widget *widget) {

}
gboolean _gtk_browser_draw(struct w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->state & STATE_OBSCURED) != 0)
		return 0;
#if USE_CAIRO
	cairo_t *cr = (cairo_t*) e->args[0];
	cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);

	/*cairo_select_font_face(cr, "Purisa",
	 CAIRO_FONT_SLANT_NORMAL,
	 CAIRO_FONT_WEIGHT_BOLD);*/

	cairo_set_font_size(cr, 13);
	cairo_move_to(cr, 20, 30);
	cairo_show_text(cr, "Browser is not installed in your operating system");
	cairo_move_to(cr, 20, 60);
	cairo_show_text(cr, "Please install Browser (WebKit)");
	_gtk_control_draw(widget, e, reserved);

#else
#endif
	return 0;
}
void _w_browser_empty_class_init(struct _w_browser_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BROWSER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_browser_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_browser);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_browser);
	W_WIDGET_CLASS(clazz)->dispose = _w_browser_dispose;
	//
	clazz->back = _w_browser_back;
	clazz->execute = _w_browser_execute;
	clazz->close = _w_browser_close;
	clazz->evaluate = _w_browser_evaluate;
	clazz->forward = _w_browser_forward;
	clazz->get_browser_type = _w_browser_get_browser_type;
	clazz->get_javascript_enabled = _w_browser_get_javascript_enabled;
	clazz->get_text = _w_browser_get_text;
	clazz->get_url = _w_browser_get_url;
	clazz->is_back_enabled = _w_browser_is_back_enabled;
	clazz->is_forward_enabled = _w_browser_is_forward_enabled;
	clazz->refresh = _w_browser_refresh;
	clazz->set_javascript_enabled = _w_browser_set_javascript_enabled;
	clazz->set_text = _w_browser_set_text;
	clazz->set_url = _w_browser_set_url;
	clazz->stop = _w_browser_stop;
	/*
	 *
	 */
	struct _w_browser_reserved *reserved = _W_BROWSER_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->signals[SIGNAL_DRAW] = _gtk_browser_draw;
}
void _w_browser_class_init(struct _w_browser_class *clazz) {
	if (_w_browser_kit_verify(clazz)) {
		_w_browser_kit_class_init(clazz);
		return;
	}
	_w_browser_empty_class_init(clazz);
}
#endif
