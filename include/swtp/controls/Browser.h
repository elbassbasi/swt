/*
 * Browser.h
 *
 *  Created on: 12 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_BROWSER_H_
#define SWTP_CONTROLS_BROWSER_H_
#include "../widgets/Composite.h"
/**
 * Instances of this class implement the browser user interface
 * metaphor.  It allows the user to visualize and navigate through
 * HTML documents.
 * <p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not make sense to set a layout on it.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>MOZILLA, WEBKIT</dd>
 * <dt><b>Events:</b></dt>
 * <dd>CloseWindowListener, LocationListener, OpenWindowListener, ProgressListener, StatusTextListener, TitleListener, VisibilityWindowListener</dd>
 * </dl>
 * <p>
 * Note: At most one of the styles MOZILLA and WEBKIT may be specified.
 * </p>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WBrowser: public WComposite {
public:
	WBrowser() {
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param parent a widget which will be the parent of the new instance (cannot be null)
	 * @param style the style of widget to construct
	 *
	 * @see Widget#getStyle
	 */
	WBrowser(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param parent a widget which will be the parent of the new instance (cannot be null)
	 * @param style the style of widget to construct
	 *
	 * @see Widget#getStyle
	 */
	bool Create(WComposite *parent, wuint64 style) {
		return Create(0, parent, style);
	}
	bool Create(WToolkit *toolkit, WComposite *parent, wuint64 style) {
		return Create(parent, style, toolkit) > 0;
	}
	WResult Create(WComposite *parent, wuint64 style,
				WToolkit *toolkit);
	/**
	 * Navigate to the previous session history item.
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise
	 *
	 * @see #forward
	 */
	bool Back() {
		return w_browser_back(W_BROWSER(this)) > 0;

	}
	/**
	 * Executes the specified script.
	 * <p>
	 * Executes a script containing javascript commands in the context of the current document.
	 * If document-defined functions or properties are accessed by the script then this method
	 * should not be invoked until the document has finished loading (<code>ProgressListener.completed()</code>
	 * gives notification of this).
	 *
	 * @param script the script with javascript commands
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise
	 *
	 * @see ProgressListener#completed(ProgressEvent)
	 */
	bool Execute(const char *script) {
		return w_browser_execute(W_BROWSER(this), script) > 0;

	}
	/**
	 * Attempts to dispose the receiver, but allows the dispose to be vetoed
	 * by the user in response to an <code>onbeforeunload</code> listener
	 * in the Browser's current page.
	 *
	 * @return <code>true</code> if the receiver was disposed, and <code>false</code> otherwise
	 *
	 * @see #dispose()
	 */
	bool Close() {
		return w_browser_close(W_BROWSER(this)) > 0;

	}
	/**
	 * Returns the result, if any, of executing the specified script.
	 * <p>
	 * Evaluates a script containing javascript commands in the context of
	 * the current document.  If document-defined functions or properties
	 * are accessed by the script then this method should not be invoked
	 * until the document has finished loading (<code>ProgressListener.completed()</code>
	 * gives notification of this).
	 * </p><p>
	 * If the script returns a value with a supported type then a java
	 * representation of the value is returned.  The supported
	 * javascript -> java mappings are:
	 * <ul>
	 * <li>javascript null or undefined -> <code>null</code></li>
	 * <li>javascript number -> <code>java.lang.Double</code></li>
	 * <li>javascript string -> <code>java.lang.String</code></li>
	 * <li>javascript bool -> <code>java.lang.bool</code></li>
	 * <li>javascript array whose elements are all of supported types -> <code>java.lang.Object[]</code></li>
	 * </ul>
	 *
	 * An <code>SWTException</code> is thrown if the return value has an
	 * unsupported type, or if evaluating the script causes a javascript
	 * error to be thrown.
	 *
	 * @param script the script with javascript commands
	 *
	 * @return the return value, if any, of executing the script
	 *
	 * @see ProgressListener#completed(ProgressEvent)
	 */
	bool Evaluate(const char *script, WValue &result) {
		return w_browser_evaluate(W_BROWSER(this), script, (w_value*) &result)
				> 0;

	}
	/**
	 * Navigate to the next session history item.
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise
	 *
	 * @see #back
	 */
	bool Forward() {
		return w_browser_forward(W_BROWSER(this)) > 0;

	}
	/**
	 * Returns the type of native browser being used by this instance.
	 * Examples: "ie", "mozilla", "voyager", "webkit"
	 *
	 * @return the type of the native browser
	 */
	const char* GetBrowserType() {
		return w_browser_get_browser_type(W_BROWSER(this));

	}
	/**
	 * Returns <code>true</code> if javascript will be allowed to run in pages
	 * subsequently viewed in the receiver, and <code>false</code> otherwise.
	 *
	 * @return the receiver's javascript enabled state
	 *
	 * @see #setJavascriptEnabled
	 */
	bool GetJavascriptEnabled() {
		return w_browser_get_javascript_enabled(W_BROWSER(this)) > 0;

	}
	/**
	 * Returns a string with HTML that represents the content of the current page.
	 *
	 * @return HTML representing the current page or an empty <code>String</code>
	 * if this is empty
	 */
	WString GetText() {
		w_string_ref* ref = 0;
		w_browser_get_text(W_BROWSER(this),w_alloc_string_ref,&ref);
		return ref;
	}
	/**
	 * Returns the current URL.
	 *
	 * @return the current URL or an empty <code>String</code> if there is no current URL
	 *
	 * @see #setUrl
	 */
	WString GetUrl() {
		w_string_ref* ref = 0;
		w_browser_get_url(W_BROWSER(this),w_alloc_string_ref,&ref);
		return ref;
	}
	/**
	 * Returns <code>true</code> if the receiver can navigate to the
	 * previous session history item, and <code>false</code> otherwise.
	 *
	 * @return the receiver's back command enabled state
	 *
	 * @see #back
	 */
	bool IsBackEnabled() {
		return w_browser_is_back_enabled(W_BROWSER(this)) > 0;

	}
	/**
	 * Returns <code>true</code> if the receiver can navigate to the
	 * next session history item, and <code>false</code> otherwise.
	 *
	 * @return the receiver's forward command enabled state
	 *
	 * @see #forward
	 */
	bool IsForwardEnabled() {
		return w_browser_is_forward_enabled(W_BROWSER(this)) > 0;
	}
	/**
	 * Refresh the current page.
	 */
	bool Refresh() {
		return w_browser_refresh(W_BROWSER(this)) > 0;

	}
	/**
	 * Sets whether javascript will be allowed to run in pages subsequently
	 * viewed in the receiver.  Note that setting this value does not affect
	 * the running of javascript in the current page.
	 *
	 * @param enabled the receiver's new javascript enabled state
	 */
	bool SetJavascriptEnabled(bool enabled) {
		return w_browser_set_javascript_enabled(W_BROWSER(this), enabled) > 0;

	}
	/**
	 * Renders a string containing HTML.  The rendering of the content occurs asynchronously.
	 * The rendered page will be given trusted permissions; to render the page with untrusted
	 * permissions use <code>setText(String html, bool trusted)</code> instead.
	 * <p>
	 * The html parameter is Unicode-encoded since it is a java <code>String</code>.
	 * As a result, the HTML meta tag charset should not be set. The charset is implied
	 * by the <code>String</code> itself.
	 *
	 * @param html the HTML content to be rendered
	 *
	 * @return true if the operation was successful and false otherwise.
	 *
	 * @see #setText(String,bool)
	 * @see #setUrl
	 */
	bool SetText(const char *html) {
		return SetText(html, true);
	}
	/**
	 * Renders a string containing HTML.  The rendering of the content occurs asynchronously.
	 * The rendered page can be given either trusted or untrusted permissions.
	 * <p>
	 * The <code>html</code> parameter is Unicode-encoded since it is a java <code>String</code>.
	 * As a result, the HTML meta tag charset should not be set. The charset is implied
	 * by the <code>String</code> itself.
	 * <p>
	 * The <code>trusted</code> parameter affects the permissions that will be granted to the rendered
	 * page.  Specifying <code>true</code> for trusted gives the page permissions equivalent
	 * to a page on the local file system, while specifying <code>false</code> for trusted
	 * gives the page permissions equivalent to a page from the internet.  Page content should
	 * be specified as trusted if the invoker created it or trusts its source, since this would
	 * allow (for instance) style sheets on the local file system to be referenced.  Page
	 * content should be specified as untrusted if its source is not trusted or is not known.
	 *
	 * @param html the HTML content to be rendered
	 * @param trusted <code>false</code> if the rendered page should be granted restricted
	 * permissions and <code>true</code> otherwise
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise.
	 *
	 * @see #setText(String)
	 * @see #setUrl
	 */
	bool SetText(const char *html, bool trusted) {
		return w_browser_set_text(W_BROWSER(this), html, trusted) > 0;

	}
	/**
	 * Begins loading a URL.  The loading of its content occurs asynchronously.
	 *
	 * @param url the URL to be loaded
	 *
	 * @return true if the operation was successful and false otherwise.
	 *
	 * @see #getUrl
	 * @see #setUrl(String,String,String[])
	 */
	bool SetUrl(const char *url) {
		return SetUrl(url, 0, 0);
	}
	/**
	 * Begins loading a URL.  The loading of its content occurs asynchronously.
	 * <p>
	 * If the URL causes an HTTP request to be initiated then the provided
	 * <code>postData</code> and <code>header</code> arguments, if any, are
	 * sent with the request.  A value in the <code>headers</code> argument
	 * must be a name-value pair with a colon separator in order to be sent
	 * (for example: "<code>user-agent: custom</code>").
	 *
	 * @param url the URL to be loaded
	 * @param postData post data to be sent with the request, or <code>null</code>
	 * @param headers header lines to be sent with the request, or <code>null</code>
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise.
	 */
	bool SetUrl(const char *url, const char *postData, const char *headers) {
		return w_browser_set_url(W_BROWSER(this), url, postData, headers) > 0;
	}
	/**
	 * Stop any loading and rendering activity.
	 */
	bool Stop() {
		return w_browser_stop(W_BROWSER(this)) > 0;
	}
private:
	void *handles[(sizeof(w_browser) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_BROWSER_H_ */
