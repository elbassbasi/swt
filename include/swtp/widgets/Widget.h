/*
 * Widget.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_WIDGET_H_
#define SWTP_WIDGETS_WIDGET_H_
#include "Event.h"
#include <string>
#include <atomic>
class WShell;
class WAccessible;
class WComposite;
class WLayout;
class WToolTip;
class WControl;
class WToolkit;

class SWTP_PUBLIC IWListener: public IDestruct {
public:
	virtual void IncRef() = 0;
	virtual void DecRef() = 0;
	virtual bool HandleEvent(WEvent *e)=0;
};

class SWTP_PUBLIC WListenerBase: public IWListener {
protected:
	volatile int ref;
public:
	WListenerBase();
	~WListenerBase();
	void IncRef();
	void DecRef();
	WListenerBase* CreateRef();
};

#if __cplusplus >= 201103L
typedef std::function<bool(WEvent &e)> WListenerFunction;
typedef std::function<void(WEvent &e)> WSelectionFunction;
typedef std::function<void(WItemEvent &e)> WSelectionItemFunction;
class SWTP_PUBLIC WListener: public WListenerBase {
public:
	WListenerFunction function;
	bool HandleEvent(WEvent *e);
};
class SWTP_PUBLIC WSelectionListener: public WListenerBase {
public:
	WSelectionFunction function;
	bool HandleEvent(WEvent *e);
};
class SWTP_PUBLIC WSelectionItemListener: public WListenerBase {
public:
	WSelectionItemFunction function;
	bool HandleEvent(WEvent *e);
};
#endif
/**
 * This class is the abstract superclass of all user interface objects.
 * Widgets are created, disposed and issue notification to listeners
 * when events occur which affect them.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Dispose</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is intended to be subclassed <em>only</em>
 * within the SWT implementation. However, it has not been marked
 * final to allow those outside of the SWT development team to implement
 * patched versions of the class in order to get around specific
 * limitations in advance of when those limitations can be addressed
 * by the team.  Any class built using subclassing to access the internals
 * of this class will likely fail to compile or run between releases and
 * may be strongly platform specific. Subclassing should not be attempted
 * without an intimate and detailed understanding of the workings of the
 * hierarchy. No support is provided for user-written classes which are
 * implemented as subclasses of this class.
 * </p>
 */
class SWTP_PUBLIC WWidget {
public:
	WWidget() {
		Init();
	}
	virtual ~WWidget();
private:
	WWidget(const WWidget &widget) {
		Init();
	}
	WWidget& operator=(const WWidget &widget) {
		return *this;
	}
protected:
	int PostEvent0(WEvent *e);
	virtual bool PostEvent(WEvent *e);
	virtual bool OnPlatformEvent(WPlatformEvent *e);
	virtual void OnFreeMemory(WEvent &e, WWidget *widget);
	virtual void OnDispose(WEvent &e);
	virtual bool OnSashSelection(WEvent &e);
	virtual void OnUpdate();
protected:
	static int post_event_proc(w_widget *widget, w_event *event);
	static int exec_function(void *args);
	bool DefaultPostEvent(WEvent *e) {
		return w_widget_default_post_event(W_WIDGET(this), W_EVENT(e));
	}
	/**
	 * index 0..3
	 *  0 : void* data
	 *  1 : used in c as function
	 *  2 : used in c++ as listener
	 */
	void* GetData(int index) {
		return w_widget_get_data(W_WIDGET(this), index);
	}
	void* SetData(int index, void *data) {
		return w_widget_set_data(W_WIDGET(this), index, data);
	}
public:
	bool IsOk() {
		return w_widget_is_ok(W_WIDGET(this));
	}
	bool IsDisposed() {
		return !IsOk();
	}
	void Dispose() {
		w_widget_dispose(W_WIDGET(this));
	}
	int GetClassId() {
		return w_widget_class_id(W_WIDGET(this));
	}
	WToolkit* GetToolkit() {
		return (WToolkit*) w_widget_get_toolkit(W_WIDGET(this));
	}
	void AsyncExec(w_thread_start function, void *args) {
		w_toolkit_async_exec(w_widget_get_toolkit(W_WIDGET(this)), function,
				args);
	}
	void AsyncExec() {
		this->AsyncExec(WWidget::exec_function, this);
	}
	/**
	 * Returns the receiver's style information.
	 * <p>
	 * Note that the value which is returned by this method <em>may
	 * not match</em> the value which was provided to the constructor
	 * when the receiver was created. This can occur when the underlying
	 * operating system does not support a particular combination of
	 * requested styles. For example, if the platform widget used to
	 * implement a particular SWT widget always has scroll bars, the
	 * result of calling this method would always have the
	 * <code>SWT.H_SCROLL</code> and <code>SWT.V_SCROLL</code> bits set.
	 * </p>
	 *
	 * @return the style bits
	 */
	int GetStyle() {
		return w_widget_get_style(W_WIDGET(this));
	}

	/**
	 * Returns the application defined widget data associated
	 * with the receiver, or null if it has not been set. The
	 * <em>widget data</em> is a single, unnamed field that is
	 * stored with every widget.
	 * <p>
	 * Applications may put arbitrary objects in this field. If
	 * the object stored in the widget data needs to be notified
	 * when the widget is disposed of, it is the application's
	 * responsibility to hook the Dispose event on the widget and
	 * do so.
	 * </p>
	 *
	 * @return the widget data
	 *
	 * @see #setData(Object)
	 */
	void* GetData() {
		return GetData(0);
	}
	/**
	 * Sets the application defined widget data associated
	 * with the receiver to be the argument. The <em>widget
	 * data</em> is a single, unnamed field that is stored
	 * with every widget.
	 * <p>
	 * Applications may put arbitrary objects in this field. If
	 * the object stored in the widget data needs to be notified
	 * when the widget is disposed of, it is the application's
	 * responsibility to hook the Dispose event on the widget and
	 * do so.
	 * </p>
	 *
	 * @param data the widget data
	 *
	 * @see #getData()
	 */
	void* SetData(void *data) {
		return SetData(0, data);
	}
	int GetId() {
		return w_widget_get_id(W_WIDGET(this));
	}
	void SetId(int id) {
		w_widget_set_id(W_WIDGET(this), id);
	}
	IWListener* GetListener() {
		return (IWListener*) GetData(2);
	}
	void SetListener(IWListener *listener);
#if __cplusplus >= 201103L
	void SetListenerFunction(const WListenerFunction &function);
	void operator <<=(const WListenerFunction &function) {
		SetListenerFunction(function);
	}
protected:
	void SetSelectionFunction(const WSelectionFunction &function);
#endif
private:
	void Init() {
		w_widget_init(W_WIDGET(this));
	}
	struct _w_widget_class *clazz;
	volatile int ref;
	wuint id;
	wuint64 style;
	void *handle;
	w_widget_post_event_proc post_event;
	void *data[sizeof(w_widget::data) / sizeof(w_widget::data[0])];
};
class SWTP_PUBLIC WWidgetData {
private:
	inline void __Init() {
		((w_widgetdata*) this)->clazz = 0;
	}
public:
	WWidgetData() {
		__Init();
	}
	WWidgetData(const WWidgetData &resourcedata) {
		__Init();
		w_widgetdata_copy((w_widgetdata*) &resourcedata, (w_widgetdata*) this);
	}
	~WWidgetData() {
		w_widgetdata_close((w_widgetdata*) this);
	}
	bool IsOk() const {
		return w_widgetdata_is_ok((w_widgetdata*) this);
	}
	void Close() const {
		w_widgetdata_close((w_widgetdata*) this);
	}
public:
	WWidgetData& operator=(const WWidgetData &resourcedata) {
		w_widgetdata_copy((w_widgetdata*) &resourcedata, (w_widgetdata*) this);
		return *this;
	}
	bool operator ==(const WWidgetData &resourcedata) const {
		return w_widgetdata_equals((w_widgetdata*) this,
				(w_widgetdata*) &resourcedata);
	}
	bool operator !=(const WWidgetData &resourcedata) const {
		return !w_widgetdata_equals((w_widgetdata*) this,
				(w_widgetdata*) &resourcedata);
	}
private:
	void *handles[sizeof(w_widgetdata) / sizeof(void*)];
};
class SWTP_PUBLIC WItem: public WWidgetData {
protected:
	/**
	 * Returns the receiver's cascade menu if it has one or null
	 * if it does not. Only <code>CASCADE</code> menu items can have
	 * a pull down menu. The sequence of key strokes, button presses
	 * and/or button releases that are used to request a pull down
	 * menu is platform specific.
	 *
	 * @return the receiver's menu
	 */
	WWidget* GetParentWidget() const {
		return (WWidget*) w_item_get_parent_widget(W_ITEM(this));
	}
	IWListener* GetListener() {
		return (IWListener*) GetData();
	}
	void SetListener(IWListener *listener);
#if __cplusplus >= 201103L
	void SetListenerFunction(const WListenerFunction &function);
	void SetSelectionFunction(const WSelectionItemFunction &function);
#endif
public:
	/**
	 * Returns the application defined widget data associated
	 * with the receiver, or null if it has not been set. The
	 * <em>widget data</em> is a single, unnamed field that is
	 * stored with every widget.
	 * <p>
	 * Applications may put arbitrary objects in this field. If
	 * the object stored in the widget data needs to be notified
	 * when the widget is disposed of, it is the application's
	 * responsibility to hook the Dispose event on the widget and
	 * do so.
	 * </p>
	 *
	 * @return the widget data
	 * @see #SetData(void*)
	 */
	void* GetData() {
		return w_item_get_data(W_ITEM(this));
	}
	wuint GetDataAsID() {
		return (intptr_t) GetData();
	}
	/**
	 * Sets the application defined widget data associated
	 * with the receiver to be the argument. The <em>widget
	 * data</em> is a single, unnamed field that is stored
	 * with every widget.
	 * <p>
	 * Applications may put arbitrary objects in this field. If
	 * the object stored in the widget data needs to be notified
	 * when the widget is disposed of, it is the application's
	 * responsibility to hook the Dispose event on the widget and
	 * do so.
	 * </p>
	 *
	 * @param data the widget data
	 * @see #getData()
	 */
	bool SetData(void *data) {
		return w_item_set_data(W_ITEM(this), data);
	}
	bool SetDataAsID(wuint id) {
		return SetData((void*) ((intptr_t) id));
	}
	int GetIndex() const {
		return w_item_get_index(W_ITEM(this));
	}
	/**
	 * Returns the receiver's text, which will be an empty
	 * string if it has never been set.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		WString str;
		w_item_get_text(W_ITEM(this),w_alloc_string_ref, &str.ref);
		return str;
	}
	/**
	 * Sets the receiver's text.
	 * <p>
	 * Note: If control characters like '\n', '\t' etc. are used
	 * in the string, then the behavior is platform dependent.
	 * </p>
	 * @param string the new text
	 */
	bool SetText(const char *text) const {
		return w_item_set_text(W_ITEM(this), text) > 0;
	}
private:
	void *handles[(sizeof(w_item) - sizeof(w_widgetdata)) / sizeof(void*)];
};

namespace swt {
typedef ::WWidget Widget;
typedef ::IWListener IListener;
typedef ::WListenerBase ListenerBase;
#if __cplusplus >= 201103L
typedef ::WListenerFunction ListenerFunction;
typedef ::WSelectionFunction SelectionFunction;
typedef ::WListener Listener;
typedef ::WSelectionListener SelectionListener;
#endif
}  // namespace swt

#endif /* SWTP_WIDGETS_WIDGET_H_ */
