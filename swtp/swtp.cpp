/*
 * swtp.cpp
 *
 *  Created on: 10 sept. 2020
 *      Author: azeddine El Bassbasi
 */
#define SWTPBUILD 1
#include <swtp.h>
IDestruct::~IDestruct() {

}
size_t w_alloc_std_string(void *user_data, size_t size, void **string) {
	std::string *str = (std::string*) user_data;
	str->resize(size);
	*string = (void*) str->c_str();
	return size;
}
size_t w_alloc_new(void *user_data, size_t size, void **ptr) {
	*ptr = new char[size];
	*((void**) user_data) = *ptr;
	return size;
}
/*
 * Widget
 */
WWidget::~WWidget() {
	Dispose();
}
bool WWidget::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_PLATFORM:
		return OnPlatformEvent((WPlatformEvent*) e);
		break;
	case W_EVENT_DISPOSE:
		OnDispose(*e);
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool WWidget::OnPlatformEvent(WPlatformEvent *e) {
	return false;
}
void WWidget::OnFreeMemory(WEvent &e, WWidget *widget) {
	if (widget != 0)
		delete widget;
}
void WWidget::OnDispose(WEvent &e) {
}
bool WWidget::OnSashSelection(WEvent &e) {
	return false;
}

void WWidget::SetListener(IWListener *listener) {
	IWListener *last = GetListener();
	SetData(2, listener);
	if (listener != 0)
		listener->IncRef();
	if (last != 0)
		last->DecRef();
}
void WItem::SetListener(IWListener *listener) {
	IWListener *last = GetListener();
	SetData(listener);
	if (listener != 0)
		listener->IncRef();
	if (last != 0)
		last->DecRef();
}

int WWidget::post_event_proc(w_widget *widget, w_event *event) {
	return ((WWidget*) widget)->PostEvent0((WEvent*) event);
}
int WWidget::PostEvent0(WEvent *e) {
	bool ret = false;
	if (e->type == W_EVENT_PLATFORM) {
		ret = this->OnPlatformEvent((WPlatformEvent*) e);
	} else if (e->type == W_EVENT_FREE_MEMORY) {
		OnFreeMemory(*e, (WWidget*) e->widget);
		return false;
	} else {
		IWListener *listener = this->GetListener();
		if (listener != 0) {
			ret = listener->HandleEvent(e);
		}
		if (!ret) {
			ret = this->PostEvent((WEvent*) e);
		}
	}
	if (e->type == W_EVENT_DISPOSE) {
		SetListener(0);
	}
	if (!ret) {
		ret = this->DefaultPostEvent((WEvent*) e);
	}
	return ret;
}
#if __cplusplus >= 201103L
void WWidget::SetListenerFunction(const WListenerFunction &function) {
	WListener *listener = new WListener();
	listener->function = function;
	listener->CreateRef();
	SetListener(listener);
}
void WWidget::SetSelectionFunction(const WSelectionFunction &function) {
	WSelectionListener *listener = new WSelectionListener();
	listener->function = function;
	listener->CreateRef();
	SetListener(listener);
}
bool WListener::HandleEvent(WEvent *e) {
	return this->function(*e);
}

bool WSelectionListener::HandleEvent(WEvent *e) {
	if (e->type == W_EVENT_SELECTION) {
		this->function(*e);
		return true;
	} else
		return false;
}

bool WSelectionItemListener::HandleEvent(WEvent *e) {
	if (e->type == W_EVENT_ITEM_SELECTION) {
		this->function(*((WItemEvent*) e));
		return true;
	} else
		return false;
}
void WItem::SetListenerFunction(const WListenerFunction &function) {
	WListener *listener = new WListener();
	listener->function = function;
	listener->CreateRef();
	SetListener(listener);
}

void WItem::SetSelectionFunction(const WSelectionItemFunction &function) {
	WSelectionItemListener *listener = new WSelectionItemListener();
	listener->function = function;
	listener->CreateRef();
	SetListener(listener);
}
#endif

WListenerBase::WListenerBase() {
	this->ref = -1;
}
WListenerBase::~WListenerBase() {
}
void WListenerBase::IncRef() {
	if (this->ref >= 0) {
		this->ref = watomic_fetch_add(&this->ref, 1);
	}
}
void WListenerBase::DecRef() {
	if (this->ref > 0) {
		this->ref = watomic_fetch_sub(&this->ref, 1);
		if (this->ref == 0)
			delete this;
	}
}

WListenerBase* WListenerBase::CreateRef() {
	if (this->ref < 0)
		this->ref = 0;
	return this;
}
int WWidget::exec_function(void *args) {
	((WWidget*) args)->OnUpdate();
	return true;
}

void WWidget::OnUpdate() {
}
/*
 * DropTarget
 */
bool WDropTarget::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_DRAGENTER:
		return OnDragEnter(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DRAGOVER:
		return OnDragOver(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DRAGLEAVE:
		return OnDragLeave(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DRAGOPERATIONCHANGED:
		return OnDragOperationChanged(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DROPACCEPT:
		return OnDropAccept(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DROP:
		return OnDrop(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	default:
		return WWidget::PostEvent(e);
		break;
	}
}

bool WDropTarget::OnDragEnter(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDragOver(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDragLeave(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDragOperationChanged(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDropAccept(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDrop(WDropTargetEvent &e) {
	return false;
}
/*
 * DragSource
 */
bool WDragSource::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_DRAGSTART:
		return OnDragStart(reinterpret_cast<WDragSourceEvent&>(*e));
		break;
	case W_EVENT_DRAGSETDATA:
		return OnDragSetData(reinterpret_cast<WDragSourceEvent&>(*e));
		break;
	case W_EVENT_DRAGFINISHED:
		return OnDragFinished(reinterpret_cast<WDragSourceEvent&>(*e));
		break;
	default:
		return WWidget::PostEvent(e);
		break;
	}
}

bool WDragSource::OnDragStart(WDragSourceEvent &e) {
	return false;
}
bool WDragSource::OnDragSetData(WDragSourceEvent &e) {
	return false;
}
bool WDragSource::OnDragFinished(WDragSourceEvent &e) {
	return false;
}
/*
 * Menu
 */
bool WMenuBase::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_ITEM_SELECTION:
		return OnItemSelection(*static_cast<WMenuEvent*>(e));
		break;
	case W_EVENT_ITEM_DISPOSE:
		return OnItemDispose(*static_cast<WMenuEvent*>(e));
		break;
	default:
		return WWidget::PostEvent(e);
		break;
	}
}

bool WMenu::OnHelp(WEvent &e) {
	return false;
}

bool WMenu::OnHide(WEvent &e) {
	return false;
}

bool WMenu::OnShow(WEvent &e) {
	return false;
}

bool WMenu::OnItemArm(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(static_cast<WEvent*>(&e));
	else
		return false;
}

bool WMenu::OnItemHelp(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(static_cast<WEvent*>(&e));
	else
		return Notify(e);
}

bool WMenu::OnItemSelection(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(static_cast<WEvent*>(&e));
	else {
		if (this->items != 0) {
			wushort id = e.item->GetId();
			WControl::SelectionAction action = this->items[id].action;
			if (action != 0) {
				WControl *c = GetParent();
				(c->*action)(&e);
			}
		}
	}
	return Notify(e);
}

bool WMenu::OnItemDispose(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener) {
		bool ret = listener->HandleEvent(static_cast<WEvent*>(&e));
		e->SetListener(0);
		return ret;
	} else
		return Notify(e);
}

bool WMenu::OnItemAdded(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(static_cast<WEvent*>(&e));
	else
		return false;
}

bool WMenu::Notify(WEvent &e) {
	WControl *c = this->GetParent();
	if (WComposite::IsComposite(c)) {
		return WComposite::_Notify((WComposite*) c, e);
	} else {
		WComposite *composite = c->GetParent();
		return WComposite::_Notify((WComposite*) composite, e);
	}
}

void WMenu::OnDispose(WEvent &e) {
	WWidget::OnDispose(e);
	this->items = 0;
}

bool WMenu::CreateItems(WMenuItems *items, size_t length) {
	WMenuItem item;
	GetRoot(item);
	size_t start = 0;
	this->items = items;
	this->items_length = length;
	bool ret = CreateSubItems(item, start);
	if (length == -1) {
		this->items_length = start;
	}
	return ret;
}
bool WMenu::CreateSubItems(WMenuItem &parent, size_t &start) {
	WMenuItem item;
	while (1) {
		if (start >= items_length)
			return true;
		if (items[start].style == 0)
			return true;
		if (items[start].style == W_CASCADE_END) {
			start++;
			return true;
		}
		parent.Append(item, items[start].name, items[start].style);
		item.SetId(start);
		if (items[start].accelerator != 0) {
			item.SetAccelerator(items[start].accelerator);
		}
		if (items[start].style & W_CASCADE) {
			start++;
			CreateSubItems(item, start);
		} else
			start++;
	}
}

/*
 * Control
 */
WControl* WControl::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->Create(parent, W_NONE, 0);
	}
	return this;
}

bool WControl::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_RESIZE:
		return OnResize(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_MOVE:
		return OnMove(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_DRAGDETECT:
		return OnDragDetect(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_FOCUSIN:
		return OnFocusIn(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_FOCUSOUT:
		return OnFocusOut(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_HELP:
		return OnHelp(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_KEYUP:
		return OnKeyUp(static_cast<WKeyEvent&>(*e));
		break;
	case W_EVENT_KEYDOWN:
		return OnKeyDown(static_cast<WKeyEvent&>(*e));
		break;
	case W_EVENT_TRAVERSE:
		return OnTraverse(static_cast<WKeyEvent&>(*e));
		break;
	case W_EVENT_MENUDETECT:
		return OnMenuDetect(static_cast<WMenuDetectEvent&>(*e));
		break;
	case W_EVENT_MOUSEDOWN:
		return OnMouseDown(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEUP:
		return OnMouseUp(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEDOUBLECLICK:
		return OnMouseDoubleClick(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEENTER:
		return OnMouseEnter(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEEXIT:
		return OnMouseExit(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEHOVER:
		return OnMouseHover(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEMOVE:
		return OnMouseMove(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEWHEEL:
		return OnMouseWheel(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_PAINT:
		return OnPaint(static_cast<WPaintEvent&>(*e));
		break;
	case W_EVENT_TIMER:
		return OnTimer(static_cast<WTimerEvent&>(*e));
		break;
	case W_EVENT_GESTURE:
		return OnGesture(static_cast<WGestureEvent&>(*e));
		break;
	case W_EVENT_TOUCH:
		return OnTouch(static_cast<WTouchEvent&>(*e));
		break;
	case W_EVENT_COMPUTE_SIZE:
		return OnComputeSize((w_event_compute_size*) e);
		break;
	default:
		return WWidget::PostEvent(e);
		break;
	}
}

bool WControl::OnComputeSize(w_event_compute_size *e) {
	return w_widget_default_post_event(W_WIDGET(this), (w_event*) e);
}

bool WControl::OnResize(WEvent &e) {
	return false;
}

bool WControl::OnMove(WEvent &e) {
	return false;
}

bool WControl::OnPaint(WPaintEvent &e) {
	return false;
}

bool WControl::OnMenuNotify(WEvent &e) {
	return false;
}

bool WControl::OnDragDetect(WMouseEvent &e) {
	return false;
}

bool WControl::OnFocusIn(WEvent &e) {
	return false;
}

bool WControl::OnFocusOut(WEvent &e) {
	return false;
}

bool WControl::OnHelp(WEvent &e) {
	return false;
}

bool WControl::OnKeyUp(WKeyEvent &e) {
	return false;
}

bool WControl::OnKeyDown(WKeyEvent &e) {
	return false;
}

bool WControl::OnTraverse(WKeyEvent &e) {
	return false;
}

bool WControl::OnMenuDetect(WMenuDetectEvent &e) {
	e.menu = GetMenu();
	return true;
}

bool WControl::OnMouseDown(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseUp(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseDoubleClick(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseEnter(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseExit(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseHover(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseMove(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseWheel(WMouseEvent &e) {
	return false;
}

bool WControl::OnGesture(WGestureEvent &e) {
	return false;
}

bool WControl::OnTouch(WTouchEvent &e) {
	return false;
}

bool WControl::OnReserved1(WEvent &e) {
	return false;
}

bool WControl::OnReserved2(WEvent &e) {
	return false;
}

bool WControl::OnReserved3(WEvent &e) {
	return false;
}

bool WControl::OnReserved4(WEvent &e) {
	return false;
}

bool WControl::OnTimer(WTimerEvent &e) {
	return false;
}

bool WControl::Notify(WEvent &e) {
	WComposite *parent = GetParent();
	if (parent != 0) {
		return WComposite::_Notify(parent, e);
	} else
		return false;
}
/*
 * Scrollable
 */
bool WScrollable::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_HSCROLL:
		return this->OnHScroll(*static_cast<WScrollBarEvent*>(e));
		break;
	case W_EVENT_VSCROLL:
		return this->OnVScroll(*static_cast<WScrollBarEvent*>(e));
		break;
	case W_EVENT_COMPUTE_TRIM:
		return this->OnComputeTrim(*reinterpret_cast<w_event_compute_trim*>(e));
		break;
	case W_EVENT_CLIENT_AREA:
		return this->OnGetClientArea(*reinterpret_cast<w_event_client_area*>(e));
		break;
	default:
		return WControl::PostEvent(e);
		break;
	}
}

bool WScrollable::OnHScroll(WScrollBarEvent &e) {
	return false;
}

bool WScrollable::OnVScroll(WScrollBarEvent &e) {
	return false;
}

bool WScrollable::OnComputeTrim(w_event_compute_trim &e) {
	return false;
}

bool WScrollable::OnGetClientArea(w_event_client_area &e) {
	return false;
}
/*
 * Composite
 */
WResult WComposite::Create(WComposite *parent, wuint64 style,
		WToolkit *toolkit) {
	return w_composite_create(W_COMPOSITE(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
bool WComposite::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_LAYOUTDETECT:
		return OnLayout(static_cast<WEvent&>(*e));
		break;
	default:
		return WScrollable::PostEvent(e);
		break;
	}

}

bool WComposite::OnLayout(WEvent &e) {
	return false;
}

bool WComposite::OnNotify(WEvent &e) {
	return WControl::Notify(e);
}
/*
 * canvas
 */
WResult WCanvas::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_canvas_create(W_CANVAS(this), (w_toolkit*) toolkit,
			(w_composite*) parent, style, WWidget::post_event_proc);
}
bool WCanvas::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_IMECOMPOSITION:
		return OnImeComposition(static_cast<WImeEvent&>(*e));
		break;
	default:
		return WComposite::PostEvent(e);
		break;
	}
}

bool WCanvas::OnImeComposition(WImeEvent &e) {
	return false;
}
/*
 *
 */
WResult WGLCanvas::Create(WComposite *parent, wuint64 style,
		WToolkit *toolkit) {
	return w_glcanvas_create(W_GLCANVAS(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 * WShell
 */
WResult WShell::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return _create(toolkit, (WShell*) parent, style);
}
bool WShell::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_CLOSE:
		return OnClose(*e);
		break;
	case W_EVENT_ICONIFY:
		return OnIconify(*e);
		break;
	case W_EVENT_DEICONIFY:
		return OnDeiconify(*e);
		break;
	case W_EVENT_ACTIVATE:
		return OnActivate(*e);
		break;
	case W_EVENT_DEACTIVATE:
		return OnDeactivate(*e);
		break;
	default:
		return WCanvas::PostEvent(e);
		break;
	}
}

bool WShell::OnClose(WEvent &e) {
	return false;
}

bool WShell::OnIconify(WEvent &e) {
	return false;
}

bool WShell::OnDeiconify(WEvent &e) {
	return false;
}

bool WShell::OnActivate(WEvent &e) {
	return false;
}

bool WShell::OnDeactivate(WEvent &e) {
	return false;
}

bool WShell::OnNotify(WEvent &e) {
	return false;
}
/*
 * layout class
 */
void w_layout_cpp_compute_size(w_layout *layout, struct w_composite *composite,
		w_size *size, int wHint, int hHint, wbool flushCache) {
	((WLayout*) layout)->ComputeSize((WComposite*) composite, *((WSize*) size),
			wHint, hHint, flushCache);

}
wresult w_layout_cpp_flush_cache(w_layout *layout, w_control *control) {
	return ((WLayout*) layout)->FlushCache((WControl*) control);
}
void w_layout_cpp_do_layout(w_layout *layout, struct w_composite *composite,
		wbool flushCache) {
	((WLayout*) layout)->DoLayout((WComposite*) composite, flushCache);
}
void w_layout_cpp_set_data(w_layout *layout, w_control *control,
		const w_layout_data *data) {
	((WLayout*) layout)->SetData((WControl*) control, *((WLayoutData*) data));
}
void w_layout_cpp_get_data(w_layout *layout, w_control *control,
		w_layout_data *data) {
	((WLayout*) layout)->GetData((WControl*) control, *((WLayoutData*) data));
}
struct _w_layout_class __w_layout_cpp_class = { //
		w_layout_cpp_compute_size, //
				w_layout_cpp_flush_cache, //
				w_layout_cpp_do_layout, //
				w_layout_cpp_set_data, //
				w_layout_cpp_get_data //
		};
/*
 * WLayout
 */
void WLayout::Init() {
	((w_layout*) this)->clazz = &__w_layout_cpp_class;
}

WLayout::~WLayout() {
}
bool WLayout::FlushCache(WControl *control) {
	return false;
}

void WLayout::GetData(WControl *control, WLayoutData &data) {
}

void WLayout::SetData(WControl *control, const WLayoutData &data) {
}
/*
 * WFillLayout
 */
void WFillLayout::Init(int type) {
	w_layout_fill_init((w_layout_fill*) this, type);
	WLayout::Init();
}
WFillLayout::WFillLayout() {
	Init(W_HORIZONTAL);
}

WFillLayout::WFillLayout(int type) {
	Init(type);
}

void WFillLayout::ComputeSize(WComposite *composite, WSize &result, int wHint,
		int hHint, bool flushCache) {
	w_layout_fill_compute_size((w_layout*) this, (w_composite*) composite,
			(w_size*) &result, wHint, hHint, flushCache);
}

void WFillLayout::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_fill_do_layout((w_layout*) this, (w_composite*) composite,
			flushCache);
}
/*
 * RowLayout
 */

void WRowLayout::ComputeSize(WComposite *composite, WSize &result, int wHint,
		int hHint, bool flushCache) {
	w_layout_row_compute_size((w_layout*) this, (w_composite*) composite,
			(w_size*) &result, wHint, hHint, flushCache);
}

void WRowLayout::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_row_do_layout((w_layout*) this, (w_composite*) composite,
			flushCache);
}

void WRowLayout::SetData(WControl *control, const WLayoutData &data) {
	w_layout_row_set_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}

void WRowLayout::GetData(WControl *control, WLayoutData &data) {
	w_layout_row_get_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}
/*
 * WGridLayout
 */
void WGridLayout::Init(int numColumns, bool makeColumnsEqualWidth) {
	w_layout_grid_init((w_layout_grid*) this, numColumns,
			makeColumnsEqualWidth);
	WLayout::Init();
}
WGridLayout::WGridLayout() {
	Init();
}

WGridLayout::WGridLayout(int numColumns, bool makeColumnsEqualWidth) {
	Init(numColumns, makeColumnsEqualWidth);
}
void WGridLayout::ComputeSize(WComposite *composite, WSize &result, int wHint,
		int hHint, bool flushCache) {
	w_layout_grid_compute_size((w_layout*) this, (w_composite*) composite,
			(w_size*) &result, wHint, hHint, flushCache);
}
void WGridLayout::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_grid_do_layout((w_layout*) this, (w_composite*) composite,
			flushCache);
}
void WGridLayout::SetData(WControl *control, const WLayoutData &data) {
	w_layout_grid_set_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}
void WGridLayout::GetData(WControl *control, WLayoutData &data) {
	w_layout_grid_get_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}
/*
 * FormLayout
 */

WFormLayout::WFormLayout() {
	Init();
}

void WFormLayout::ComputeSize(WComposite *composite, WSize &result, int wHint,
		int hHint, bool flushCache) {
	w_layout_form_compute_size((w_layout*) this, (w_composite*) composite,
			(w_size*) &result, wHint, hHint, flushCache);
}

void WFormLayout::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_form_do_layout((w_layout*) this, (w_composite*) composite,
			flushCache);
}

void WFormLayout::SetData(WControl *control, const WLayoutData &data) {
	w_layout_form_set_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}

void WFormLayout::GetData(WControl *control, WLayoutData &data) {
	w_layout_form_get_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}
/*
 *
 */
void WSashLayoutBase::ComputeSize(WComposite *composite, WSize &result,
		int wHint, int hHint, bool flushCache) {
	w_layout_sash_compute_size((w_layout*) this, W_COMPOSITE(composite),
			(w_size*) &result, wHint, hHint, flushCache);
}

void WSashLayoutBase::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_sash_do_layout((w_layout*) this, W_COMPOSITE(composite),
			flushCache);
}
WSashLayoutBase::~WSashLayoutBase() {
	w_layout_sash_free((w_layout_sash*) this);
}
void WSashLayoutBase::GetData(WControl *control, WLayoutData &data) {
	void *d;
	w_control_get_layout_data(W_CONTROL(control), &d);
	if (d != 0) {
		((WSashLayoutData*) &data)->weight = ((WSashLayoutData*) d)->weight;
	} else {
		((WSashLayoutData*) &data)->weight = ((200 << 16) + 999) / 1000;
	}
}

void WSashLayoutBase::SetData(WControl *control, const WLayoutData &data) {
	void *d;
	w_control_new_layout_data(W_CONTROL(control), &d, sizeof(WSashLayoutData));
	if (d != 0) {
		((WSashLayoutData*) d)->weight = ((WSashLayoutData*) &data)->weight;
	}
}
/*
 * WBrowser
 */
WResult WBrowser::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_browser_create(W_BROWSER(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 * Button
 */
WResult WButton::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_button_create(W_BUTTON(this), (w_toolkit*) toolkit,
			(w_composite*) parent, style, WWidget::post_event_proc);
}
bool WButton::PostEvent(WEvent *e) {
	if (e->type == W_EVENT_SELECTION) {
		return OnSelection(*e);
	} else {
		return WControl::PostEvent(e);
	}
}

bool WButton::OnSelection(WEvent &e) {
	return WControl::Notify(e);
}
/*
 * WCombo
 */
WResult WCombo::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_combo_create(W_COMBO(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}

/*
 * CoolBar
 */
WResult WCoolBar::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_coolbar_create(W_COOLBAR(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 * WDateTime
 */
WResult WDateTime::Create(WComposite *parent, wuint64 style,
		WToolkit *toolkit) {
	return w_datetime_create(W_DATETIME(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 * WExpandBar
 */
WResult WExpandBar::Create(WComposite *parent, wuint64 style,
		WToolkit *toolkit) {
	return w_expandbar_create(W_EXPANDBAR(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 * WGroup
 */
WResult WGroup::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_group_create(W_GROUP(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 * WLabel
 */
WResult WLabel::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_label_create(W_LABEL(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *
 */
WResult WLink::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_link_create(W_LINK(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *
 */
WResult WList::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_list_create(W_LIST(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *
 */
WResult WProgressBar::Create(WComposite *parent, wuint64 style,
		WToolkit *toolkit) {
	return w_progressbar_create(W_PROGRESSBAR(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *	WSash
 */
WResult WSash::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_sash_create(W_SASH(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
bool WSash::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_SELECTION:
		return OnSelection(static_cast<WSashEvent&>(*e));
		break;
	default:
		return WControl::PostEvent(e);
		break;
	}
}

bool WSash::OnSelection(WSashEvent &e) {
	return Notify(e);
}
/*
 *
 */
WResult WScale::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_scale_create(W_SCALE(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *
 */
WResult WSlider::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_slider_create(W_SLIDER(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *
 */

WResult WSpinner::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_spinner_create(W_SPINNER(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *	WTabFolder
 */
WResult WTabFolder::Create(WComposite *parent, wuint64 style,
		WToolkit *toolkit) {
	return w_tabfolder_create(W_TABFOLDER(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
bool WTabFolder::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_ITEM_SELECTION:
		return OnItemSelection(static_cast<WTabItemEvent&>(*e));
		break;
	case W_EVENT_ITEM_GET_CONTROL:
		return OnItemGetControl(static_cast<WTabItemEvent&>(*e));
		break;
	case W_EVENT_ITEM_SET_CONTROL:
		return OnItemSetControl(static_cast<WTabItemEvent&>(*e));
		break;
	case W_EVENT_ITEM_CLOSE:
		return OnItemClose(static_cast<WTabItemEvent&>(*e));
		break;
	}
	return WComposite::PostEvent(e);
}

bool WTabFolder::OnItemSelection(WTabItemEvent &e) {
	return false;
}

bool WTabFolder::OnItemGetControl(WTabItemEvent &e) {
	e.control = (WControl*) e->GetData();
	return true;
}

bool WTabFolder::OnItemSetControl(WTabItemEvent &e) {
	e->SetData(e.control);
	return true;
}

bool WTabFolder::OnItemClose(WTabItemEvent &e) {
	return false;
}
/*
 *
 */
WResult WTable::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_table_create(W_TABLE(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *
 */
WResult WText::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_text_create(W_TEXT(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 *
 */
WResult WToolBar::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_toolbar_create(W_TOOLBAR(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 * WTree
 */
WResult WTree::Create(WComposite *parent, wuint64 style, WToolkit *toolkit) {
	return w_tree_create(W_TREE(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
bool WTree::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_ITEM_SELECTION:
		return OnItemSelection(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_DEFAULTSELECTION:
		return OnItemDefaultSelection(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_EXPAND:
		return OnItemExpand(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_COLLAPSE:
		return OnItemCollapse(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_GET_VALUE:
		return OnItemGetValue(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_SET_VALUE:
		return OnItemSetValue(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_PAINT:
		return OnItemPaint(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_ERASE:
		return OnItemErase(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_MEASURE:
		return OnItemMeasure(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_GET_ATTR:
		return OnItemGetAttr(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_SET_ATTR:
		return OnItemSetAttr(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_DISPOSE:
		return OnItemDispose(static_cast<WTreeEvent&>(*e));
		break;
	}
	return WComposite::PostEvent(e);
}

bool WTree::OnItemGetValue(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemSetValue(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemPaint(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemErase(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemMeasure(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemGetAttr(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemSetAttr(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemExpand(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemCollapse(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemSelection(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemDefaultSelection(WTreeEvent &e) {
	return false;
}
bool WTree::OnItemDispose(WTreeEvent &e) {
	return false;
}
/*
 * Iterator
 */
wresult w_iterator_cpp_close(w_iterator *it) {
	IWIterator *i = reinterpret_cast<IWIterator*>(it);
	i->~IWIterator();
	return true;
}
wbool w_iterator_cpp_next(w_iterator *it, void *obj) {
	return reinterpret_cast<IWIterator*>(it)->Next(obj);
}
wresult w_iterator_cpp_reset(w_iterator *it) {
	return reinterpret_cast<IWIterator*>(it)->Reset();
}
wresult w_iterator_cpp_remove(w_iterator *it) {
	return reinterpret_cast<IWIterator*>(it)->Remove();
}
size_t w_iterator_cpp_get_count(w_iterator *it) {
	return reinterpret_cast<IWIterator*>(it)->GetCount();
}
_w_iterator_class w_iterator_cpp_clazz = { //
		w_iterator_cpp_close, //
				w_iterator_cpp_next, //
				w_iterator_cpp_reset, //
				w_iterator_cpp_remove, //
				w_iterator_cpp_get_count //
		};

IWIterator::IWIterator() {
	this->clazz = &w_iterator_cpp_clazz;
}

IWIterator::~IWIterator() {
}

WResult IWIterator::Reset() {
	return false;
}

WResult IWIterator::Remove() {
	return false;
}

size_t IWIterator::GetCount() {
	return 0;
}
/*
 * thread
 */
IWRunnable::~IWRunnable() {
}
WThread::WThread() {
	Init();
}
WThread::~WThread() {
	this->Dispose();
}
void WThread::Run() {
}
int WThread::w_thread_runnable_start(void *args) {
	IWRunnable *runnable = (IWRunnable*) args;
	runnable->Run();
	return 0;
}
#if __cplusplus >= 201103L
class WRunnableLambda: public IWRunnable {
public:
	WRunnable function;
	WRunnableLambda() {

	}
	WRunnableLambda(WRunnable &function) {
		this->function = function;
	}
	void Run() {
		this->function();
		delete this;
	}
};
bool WThread::Create(WRunnable &run) {
	WRunnableLambda *lambda = new WRunnableLambda(run);
	return Create(lambda);
}
#endif
