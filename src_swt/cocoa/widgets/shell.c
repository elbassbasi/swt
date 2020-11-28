/*
 * shell.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "shell.h"
#include "toolkit.h"
#define ISCUSTOMRESIZE(style) ((style & W_NO_TRIM) == 0 && (style & (W_RESIZE | W_ON_TOP)) == (W_RESIZE | W_ON_TOP))
wuint64 _w_shell_check_style(w_widget *control, wuint64 style) {
	if ((style & W_NO_TRIM) != 0) {
		style &= ~(W_CLOSE | W_TITLE | W_MIN | W_MAX | W_RESIZE | W_BORDER);
	} else if ((style & W_NO_MOVE) != 0) {
		style |= W_TITLE;
	}
	if ((style & (W_MENU | W_MIN | W_MAX | W_CLOSE)) != 0) {
		style |= W_TITLE;
	}
	style &= ~W_TRANSPARENT;
	int mask = W_SYSTEM_MODAL | W_APPLICATION_MODAL | W_PRIMARY_MODAL;
	if ((style & W_SHEET) != 0) {
		w_composite *parent = _W_CONTROL(control)->parent;
		if (/*Display.getSheetEnabled ()*/1) {
			style &= ~(W_CLOSE | W_TITLE | W_MIN | W_MAX);
			if (parent == 0) {
				style &= ~W_SHEET;
				style |= W_SHELL_TRIM;
			}
		} else {
			style &= ~W_SHEET;
			style |= parent == 0 ? W_SHELL_TRIM : W_DIALOG_TRIM;
		}
		style |= parent == 0 ? W_SHELL_TRIM : W_DIALOG_TRIM;
		if ((style & mask) == 0) {
			style |= parent == 0 ? W_APPLICATION_MODAL : W_PRIMARY_MODAL;
		}
	}
	int bits = style & ~mask;
	if ((style & W_SYSTEM_MODAL) != 0)
		return bits | W_SYSTEM_MODAL;
	if ((style & W_APPLICATION_MODAL) != 0)
		return bits | W_APPLICATION_MODAL;
	if ((style & W_PRIMARY_MODAL) != 0)
		return bits | W_PRIMARY_MODAL;
	return bits;
}
wresult _w_shell_create_embedded(w_widget *widget, w_widget *parent,
		wuint64 style, w_widget_post_event_proc post_event, void *handle,
		wbool embedded) {
	if (parent != 0) {
		if (!w_widget_is_ok(parent))
			return W_ERROR_INVALID_ARGUMENT;
		if (w_widget_class_id(parent) != _W_CLASS_SHELL)
			return W_ERROR_INVALID_ARGUMENT;
	}
	_W_CONTROL(widget)->parent = W_COMPOSITE(parent);
	if (/*!Display.getSheetEnabled ()*/0) {
	_W_SHELL(widget)->center = parent != 0 && (style & W_SHEET) != 0;
	}
	_W_WIDGET(widget)->post_event = post_event;
	_W_WIDGET(widget)->style = _w_shell_check_style(widget, style);
	if (handle != 0) {
		if (embedded) {
			_W_WIDGET(widget)->handle = handle;
		} else {
			_W_SHELL(widget)->window = handle;
			_W_WIDGET(widget)->state |= STATE_FOREIGN_HANDLE;
		}
	}
	_w_widget_reserved *reserved = _w_widget_get_reserved(widget);
	//reskinWidget();
	wresult result = reserved->create_widget(widget, reserved);
    if (result > 0) {
        _w_toolkit_add_shell(_W_SHELL(widget));
    }
	return result;
}
wresult _w_shell_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_shell_create_embedded(widget, parent, style, post_event, 0,
			W_FALSE);
}
////@Override
/*boolean _w_shell_accessibilityIsIgnored(long id, long sel) {
	// The content view of a shell is always ignored.
	if (id == view.id) return true;
	return super.accessibilityIsIgnored(id, sel);
}*/
/*void _w_shell_attachObserversToWindow(w_shell *shell,NSWindow* newWindow) {
	if (newWindow == 0) return;
	long newHostWindowClass = OS.object_getClass(newWindow.id);
	long sendEventImpl = OS.class_getMethodImplementation(newHostWindowClass, OS.sel_sendEvent_);
	if (sendEventImpl == Display.windowCallback3.getAddress()) return;
	hostWindow = newWindow;
	hostWindow.retain();
	long embeddedSubclass = display.createWindowSubclass(newHostWindowClass, "SWTAWTWindow", true);
	OS.object_setClass(hostWindow.id, embeddedSubclass);
	display.addWidget (hostWindow, this);
	hostWindowClass = newHostWindowClass;

	if (windowEmbedCounts == null) windowEmbedCounts = new HashMap();
	Integer embedCount = (Integer) windowEmbedCounts.get(hostWindow);
	if (embedCount == null) {
		embedCount = Integer.valueOf(0);
	}
	embedCount = Integer.valueOf(embedCount.intValue() + 1);
	windowEmbedCounts.put(hostWindow, embedCount);

	// Register for notifications. An embedded shell has no control over the host window,
	// so it isn't correct to install a delegate.
	NSNotificationCenter defaultCenter = NSNotificationCenter.defaultCenter();
	defaultCenter.addObserver(windowDelegate, OS.sel_windowDidBecomeKey_, OS.NSWindowDidBecomeKeyNotification, hostWindow);
	defaultCenter.addObserver(windowDelegate, OS.sel_windowDidDeminiaturize_, OS.NSWindowDidDeminiaturizeNotification, hostWindow);
	defaultCenter.addObserver(windowDelegate, OS.sel_windowDidMiniaturize_, OS.NSWindowDidMiniaturizeNotification, hostWindow);
	defaultCenter.addObserver(windowDelegate, OS.sel_windowDidMove_, OS.NSWindowDidMoveNotification, hostWindow);
	defaultCenter.addObserver(windowDelegate, OS.sel_windowDidResize_, OS.NSWindowDidResizeNotification, hostWindow);
	defaultCenter.addObserver(windowDelegate, OS.sel_windowDidResignKey_, OS.NSWindowDidResignKeyNotification, hostWindow);
	defaultCenter.addObserver(windowDelegate, OS.sel_windowWillClose_, OS.NSWindowWillCloseNotification, hostWindow);
}*/

//@Override
/*void _w_shell_becomeKeyWindow (long id, long sel) {
	Shell modal = getModalShell();
	if (modal != null && modal.window != null) {
		modal.window.makeKeyAndOrderFront(null);
		return;
	}
	Display display = this.display;
	display.keyWindow = view.window();
	super.becomeKeyWindow(id, sel);
	display.checkFocus();
	display.keyWindow = null;
}*/

//@Override
void _w_shell_bring_to_top (w_shell* shell,int force) {
	/*if (_w_shell_get_minimized(shell)>0) return;
	if (force) {
		_w_shell_force_active (shell);
	} else {
		_w_shell_set_active (shell);
	}*/
}

//@Override
/*boolean _w_shell_canBecomeKeyWindow (long id, long sel) {
	if (isPopup) return false;
	// Only answer if SWT created the window.
	if (window != null) {
		if ((style & SWT.NO_FOCUS) != 0) {
			NSEvent nsEvent = NSApplication.sharedApplication().currentEvent();
			if (nsEvent != null && nsEvent.type() == OS.NSLeftMouseDown) {
				NSView contentView = window.contentView();
				if (contentView != null) {
					NSView view = contentView.hitTest(nsEvent.locationInWindow());
					if (view == contentView) return false;
				}
			}
		}
		long styleMask = window.styleMask();
		if (styleMask == OS.NSBorderlessWindowMask || (styleMask & (OS.NSNonactivatingPanelMask | OS.NSDocModalWindowMask | OS.NSResizableWindowMask)) != 0) return true;
	}
	return super.canBecomeKeyWindow (id, sel);
}*/

//@Override
void _w_shell_check_open (w_widget* widget) {
	if (!_W_SHELL(widget)->opened) _W_SHELL(widget)->resized = W_FALSE;
}

void _w_shell_center (w_shell* shell) {
	if (_W_CONTROL(shell)->parent == 0) return;
	/*Rectangle rect = getBounds ();
	Rectangle parentRect = display.map (parent, null, parent.getClientArea());
	int x = Math.max (parentRect.x, parentRect.x + (parentRect.width - rect.width) / 2);
	int y = Math.max (parentRect.y, parentRect.y + (parentRect.height - rect.height) / 2);
	Rectangle monitorRect = parent.getMonitor ().getClientArea();
	if (x + rect.width > monitorRect.x + monitorRect.width) {
		x = Math.max (monitorRect.x, monitorRect.x + monitorRect.width - rect.width);
	} else {
		x = Math.max (x, monitorRect.x);
	}
	if (y + rect.height > monitorRect.y + monitorRect.height) {
		y = Math.max (monitorRect.y, monitorRect.y + monitorRect.height - rect.height);
	} else {
		y = Math.max (y, monitorRect.y);
	}
	setLocation (x, y);*/
}

//@Override
/*void _w_shell_clearDeferFlushing (long id, long sel) {
	deferFlushing = false;
	scrolling = false;
	if (window != null) window.flushWindowIfNeeded();
}*/
wresult _w_shell_close(w_shell *shell){
    _w_shell_close_widget (shell,0);
}
wresult _w_shell_close_widget (w_shell *shell,_w_event_platform *e) {
    wuint64 style = _W_WIDGET(shell)->style;
    w_event event;
    event.type = W_EVENT_CLOSE;
    event.platform_event = (w_event_platform*) e;
    event.widget = W_WIDGET(shell);
    event.data = 0;
    _w_widget_send_event(W_WIDGET(shell), &event);
    if (style & W_DISPOSE_ON_CLOSE) {
        w_widget_dispose(W_WIDGET(shell));
    }
}
//@Override
 wresult _w_shell_compute_size(w_widget *widget, w_event_compute_size *e,
 _w_widget_reserved *reserved){
	/*Point size = super.computeSize (wHint, hHint, changed);
	if (toolBar != null) {
		if (wHint == SWT.DEFAULT && toolBar.itemCount > 0) {
			Point tbSize = toolBar.computeSize (SWT.DEFAULT, SWT.DEFAULT);
			size.x = Math.max (tbSize.x, size.x);
		}
	}
	return size;*/
}

//@Override
wresult _w_shell_compute_trim(w_widget *widget, w_event_compute_trim *e,
 _w_widget_reserved *reserved)  {
	/*Rectangle trim = super.computeTrim(x, y, width, height);
	NSRect rect = new NSRect ();
	rect.x = trim.x;
	rect.y = trim.y;
	rect.width = trim.width;
	rect.height = trim.height;
	if (window != null) {
		if (!_getFullScreen() && !fixResize()) {
			double h = rect.height;
			rect = window.frameRectForContentRect(rect);
			rect.y += h-rect.height;
		}
	}
	return new Rectangle ((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height);*/
}

//@Override
wresult _w_shell_create_handle(w_widget *widget, _w_widget_reserved *reserved){
	_W_WIDGET(widget)->state |= STATE_HIDDEN;
    NSWindow* window=0;
    w_rect frame;
	if (_W_SHELL(widget)->window == 0 && _W_WIDGET(widget)->handle == 0) {
		int styleMask = NSBorderlessWindowMask;
		if ((_W_WIDGET(widget)->style & (W_TOOL | W_SHEET)) != 0) {
			window = SWTPanel_alloc(widget);
			if ((_W_WIDGET(widget)->style & W_SHEET) != 0) {
				styleMask |= NSDocModalWindowMask;
			} else {
				styleMask |= NSUtilityWindowMask | NSNonactivatingPanelMask;
			}
		} else {
			window =  SWTWindow_alloc(widget);
		}
        _W_SHELL(widget)->window = window;
		if ((_W_WIDGET(widget)->style & W_NO_TRIM) == 0) {
			if ((_W_WIDGET(widget)->style & W_TITLE) != 0) styleMask |= NSTitledWindowMask;
			if ((_W_WIDGET(widget)->style & W_CLOSE) != 0) styleMask |= NSClosableWindowMask;
			if ((_W_WIDGET(widget)->style & W_MIN) != 0) styleMask |= NSMiniaturizableWindowMask;
			if ((_W_WIDGET(widget)->style & W_MAX) != 0) styleMask |= NSResizableWindowMask;
			if ((_W_WIDGET(widget)->style & W_RESIZE) != 0) styleMask |= NSResizableWindowMask;
		}
		NSScreen* screen = 0;
        NSArray* screens = NSScreen_screens();
		NSScreen* primaryScreen = (NSScreen*)NSArray_objectAtIndex(screens, 0);
        if (_W_CONTROL(widget)->parent != 0){
            NSWindow* parentWindow = _w_shell_parent_window(W_SHELL(widget));
            screen = NSWindow_screen(parentWindow);
        }
		if (screen == 0) screen = primaryScreen;
        window = NSWindow_initWithContentRect_0(window, 0, styleMask, NSBackingStoreBuffered, (_W_WIDGET(widget)->style & W_ON_TOP) != 0, screen);
		if ((_W_WIDGET(widget)->style & (W_NO_TRIM | W_BORDER | W_SHELL_TRIM)) == 0 || (_W_WIDGET(widget)->style & (W_TOOL | W_SHEET)) != 0) {
            NSWindow_setHasShadow(window, W_TRUE);
		}
		if ((_W_WIDGET(widget)->style & W_NO_MOVE) != 0) {
            NSWindow_setMovable(window, W_FALSE);
		}
		if ((_W_WIDGET(widget)->style & W_TOOL) != 0) {
			// Feature in Cocoa: NSPanels that use NSUtilityWindowMask are always promoted to the floating window layer.
			// Fix is to call setFloatingPanel:NO, which turns off this behavior.
            NSPanel_setFloatingPanel((NSPanel*)window, W_FALSE);
			// By default, panels hide on deactivation.
			NSWindow_setHidesOnDeactivate(window, W_FALSE);
			// Normally a panel doesn't become key unless something inside it needs to be first responder.
			// TOOL shells always become key, so disable that behavior.
			NSPanel_setBecomesKeyOnlyIfNeeded((NSPanel*)window, W_FALSE);
		}
        NSWindow_setReleasedWhenClosed(window, W_TRUE);
		if ((_W_WIDGET(widget)->style & W_NO_TRIM) == 0) {
            NSWindow_minSize(window, &frame.sz);
            NSString* str = NSString_stringWithUTF8String("");
            frame.sz.width = NSWindow_minFrameWidthWithTitle(str, styleMask);
            NSObject_release(NSOBJECT(str));
			NSWindow_setMinSize(window,&frame.sz);
		}
		if (_w_shell_fix_resize(W_SHELL(widget))>0) {
			//if (window.respondsToSelector(OS.sel_setMovable_)) {
            NSWindow_setMovable(window, 0);
			//}
		}
		if (MAC_VERSION >= VERSION(10, 12, 0)) {
			/*
			 * In macOS 10.12, a new system preference "prefer tabs when opening documents"
			 * has been added which causes automatic tabbing of windows in Eclipse.
			 * Disable automatic window tabbing, but setting the NSWindow.allowsAutomaticWindowTabbing
			 * property to false.
			 */
            NSWindow_setAllowsAutomaticWindowTabbing(false);
		}
		//display.cascadeWindow(window, screen);
		w_rect screenFrame;
        NSScreen_frame(screen, &screenFrame);
		double width = screenFrame.width * 5 / 8, height = screenFrame.height * 5 / 8;
        NSWindow_frame(window, &frame);
		w_rect primaryFrame;
        NSScreen_frame(primaryScreen, &primaryFrame);
		frame.y = primaryFrame.height - ((primaryFrame.height - (frame.y + frame.height)) + height);
		frame.width = width;
		frame.height = height;
		NSWindow_setFrame(window,&frame, W_FALSE);
		if ((_W_WIDGET(widget)->style & W_ON_TOP) != 0) {
            NSWindow_setLevel(window, NSStatusWindowLevel);
		}
		_w_composite_create_handle(widget, reserved);
         NSView* topView =_W_CONTROL_RESERVED(reserved)->top_view(widget);
        NSView_setHidden(topView, W_TRUE);
	} else {
		_W_WIDGET(widget)->state &= ~STATE_HIDDEN;

		if (_W_SHELL(widget)->window != 0) {
			// In the FOREIGN_HANDLE case, 'window' is an NSWindow created on our behalf.
			// It may already have a content view, so if it does, grab and retain, since we release()
			// the view at disposal time.  Otherwise, create a new 'view' that will be used as the window's
			// content view in setZOrder.
			_W_WIDGET(widget)->handle = NSWindow_contentView(_W_SHELL(widget)->window);

			if (_W_WIDGET(widget)->handle == 0) {
				_w_composite_create_handle(widget, reserved);
			} else {
                NSObject_retain(NSOBJECT(_W_WIDGET(widget)->handle));
			}
		} else {
			// In the embedded case, 'view' is already set to the NSView we should add the window's content view to as a subview.
			// In that case we will hold on to the foreign view, create our own SWTCanvasView (which overwrites 'view') and then
			// add it to the foreign view.
			NSView* parentView = _W_WIDGET(widget)->handle;
            _w_composite_create_handle(widget, reserved);
            NSView* topView =_W_CONTROL_RESERVED(reserved)->top_view(widget);
            NSView_addSubview(parentView, topView);
		}

		_W_WIDGET(widget)->style |= W_NO_BACKGROUND;
	}

	if (_W_SHELL(widget)->window == 0) {
        NSView* view = _W_CONTROL_RESERVED(reserved)->get_view(widget);
		NSWindow* hostWindow = NSView_window(view);
		//attachObserversToWindow(hostWindow);
	} else {
        NSObject* windowDelegate = SWTWindowDelegate_alloc(widget);
		int behavior;
		if (_W_CONTROL(widget)->parent != 0) {
			behavior = NSWindowCollectionBehaviorMoveToActiveSpace;
		} else if ((_W_WIDGET(widget)->style & W_TOOL) != 0) {
			behavior = NSWindowCollectionBehaviorFullScreenAuxiliary;
		} else {
			behavior = NSWindowCollectionBehaviorFullScreenPrimary;
		}
        NSWindow_setCollectionBehavior(window, behavior);
        NSWindow_setAcceptsMouseMovedEvents(window, W_TRUE);
        NSWindow_setDelegate(window, windowDelegate);
	}

	/*NSWindow* fieldEditorWindow = window;
	if (fieldEditorWindow == null) fieldEditorWindow = view.window();
	id id = fieldEditorWindow.fieldEditor (true, null);
	if (id != null) {
		OS.object_setClass (id.id, OS.objc_getClass ("SWTEditorView"));
		new NSTextView(id).setAllowsUndo(true);
	}*/
    return W_TRUE;
}

void _w_shell_deferFlushing () {
	/*deferFlushing = true;
	view.performSelector(OS.sel_clearDeferFlushing, null, 0.0, display.runLoopModes());*/
}

//@Override
void _w_shell_destroyWidget () {
	/*NSWindow window = this.window;
	if (window != null) window.retain();
	Display display = this.display;
	NSView view = topView();
	if (view != null) view.retain();

	boolean sheet = (style & (SWT.SHEET)) != 0;
	releaseHandle ();
	if (window != null) {
		if (sheet) {
			NSApplication application = NSApplication.sharedApplication();
			application.endSheet(window, 0);
		}
		window.close();
	} else if (view != null) {
		view.removeFromSuperview();
	}
	if (view != null) view.release();

	// If another shell is not going to become active, clear the menu bar.
	// Don't modify the menu bar if we are an embedded Shell, though.
	if (window != null) {
		if (!display.isDisposed () && display.getShells ().length == 0) {
			display.setMenuBar (null);
		}
		window.release();
	}*/
}

//@Override
void _w_shell_drawBackground (long id, NSGraphicsContext* context, w_rect* rect) {
	/*if (id != view.id) return;
	if (regionPath != null && background == null) {
		context.saveGraphicsState();
		NSColor.windowBackgroundColor().setFill();
		NSBezierPath.fillRect(rect);
		context.restoreGraphicsState();
		return;
	}
	super.drawBackground (id, context, rect);*/
}

//@Override
w_control* _w_shell_findBackgroundControl () {
	//return background != null || backgroundImage != null ? this : null;
}

w_composite* _w_shell_find_deferred_control (w_composite *composite,
_w_composite_reserved *reserved) {
	return _W_COMPOSITE(composite)->layoutCount > 0 ? composite : 0;
}

//@Override
w_cursor* _w_shell_find_cursor (w_widget* widget,_w_widget_reserved* reserved) {
	return _W_CONTROL(widget)->cursor;
}

wresult _w_shell_fix_resize (w_shell* shell) {
	/*
	* Feature in Cocoa.  It is not possible to have a resizable window
	* without the title bar.  The fix is to resize the content view on
	* top of the title bar.
	*
	* Never do this when the shell is embedded, because the window belongs to the AWT.
	*/
	if (_W_SHELL(shell)->window == 0) return W_FALSE;
	if ((_W_WIDGET(shell)->style & W_NO_TRIM) == 0) {
		if ((_W_WIDGET(shell)->style & W_RESIZE) != 0 && (_W_WIDGET(shell)->style & (W_SHEET | W_TITLE | W_CLOSE | W_MIN | W_MAX)) == 0) {
			return W_TRUE;
		}
	}
	return W_FALSE;
}

void _w_shell_fix_shell (w_shell* shell,w_shell* newShell, w_control* control) {
	/*if (this == newShell) return;
	if (control == lastActive) setActiveControl (null);*/
}
wresult _w_shell_force_active(w_shell *shell) {
	/*checkWidget ();
	if (!isVisible()) return;
	if (window == null) return;
	makeKeyAndOrderFront ();
	NSApplication application = NSApplication.sharedApplication ();
	application.activateIgnoringOtherApps (true);*/
}
wresult _w_shell_get_default_button(w_shell *shell,w_button** button){
	/*if (defaultButton != null && defaultButton.isDisposed ()) return null;
	return defaultButton;*/
}
wresult _w_shell_get_alpha(w_shell *shell) {
	/*checkWidget ();
	if (window == null) return 255;
	return (int)(window.alphaValue() * 255);*/
}

//@Override
 wresult _w_shell_get_bounds (w_control* control,w_point* location,w_size* size) {
    w_rect frame;
    int y;
	if (_W_SHELL(control)->window != 0) {
        if(location != 0){
            NSArray* screens = NSScreen_screens();
            if(screens != 0){
                NSScreen* screen = (NSScreen*)NSArray_objectAtIndex(screens, 0);
                NSScreen_frame(screen, &frame);
                y = frame.height;
            }else y = 0;
            NSWindow_frame(_W_SHELL(control)->window, &frame);
            y = y - (frame.y + frame.height);
            location->x = frame.x;
            location->y = y;
        }else{
            NSWindow_frame(_W_SHELL(control)->window, &frame);
        }
        if(size != 0){
            size->width = frame.width;
            size->height = frame.height;
        }
	} else {
        NSView* view = _w_scrollable_get_view(W_WIDGET(control));
        if(location != 0){
            location->x = 0;
            location->y = 0;
            NSArray* screens = NSScreen_screens();
            if(screens != 0){
                NSScreen* screen = (NSScreen*)NSArray_objectAtIndex(screens, 0);
                NSScreen_frame(screen, &frame);
                y = frame.height;
            }else y = 0;
            if (!NSView_isFlipped (view)) {
                NSView_bounds(view, &frame);
                location->y = frame.height - location->y;
            }
            NSView_convertPoint_toView_(view, &frame.pt, location, 0);
            NSWindow* _window = NSView_window(view);
            NSWindow_convertBaseToScreen(_window, location, &frame.pt);
            location->y = y - location->y;
        }
        if(size != 0){
            NSView_frame(view, &frame);
            size->width = frame.width;
            size->height = frame.height;
        }
	}
}

//@Override
wresult _w_shell_get_client_area(w_widget *widget,w_event_client_area *e, _w_widget_reserved *reserved) {
	if (_W_SHELL(widget)->window != 0) {
		if (!_w_shell_fix_resize(W_SHELL(widget))) {
            NSView* contentView =NSWindow_contentView(_W_SHELL(widget)->window);
            NSView_frame(contentView, e->rect);
		} else {
            NSWindow_frame(_W_SHELL(widget)->window, e->rect);
		}
	} else {
        NSView* topView =_W_CONTROL_RESERVED(reserved)->top_view(widget);
        NSView_frame(topView, e->rect);
	}
    e->rect->x = 0;
    e->rect->y = 0;
    NSView* scrollView = _W_SCROLLABLE_RESERVED(reserved)->scroll_view(widget);
	if (scrollView != 0) {
        NSScrollView_contentSizeForFrameSize((NSScrollView*)scrollView, &e->rect->sz, (_W_WIDGET(widget)->style & W_HSCROLL) != 0, (_W_WIDGET(widget)->style & W_VSCROLL) != 0, NSNoBorder);
	}
	return W_TRUE;
}
 wresult _w_shell_get_full_screen(w_shell *shell) {
    if ((NSWindow_collectionBehavior(_W_SHELL(shell)->window) & NSWindowCollectionBehaviorFullScreenPrimary) != 0) {
        return (NSWindow_styleMask(_W_SHELL(shell)->window) & NSFullScreenWindowMask) != 0 ? W_TRUE : W_FALSE;
    }
    return _W_SHELL(shell)->fullScreen;
}

wresult _w_shell_get_ime_input_mode(w_shell *shell) {
	return W_NONE;
}

//@Override
 wresult _w_shell_get_maximized(w_shell *shell) {
	if (_W_SHELL(shell)->window == 0) return W_FALSE;
	return _w_shell_get_full_screen(shell)<=0 && NSWindow_isZoomed(_W_SHELL(shell)->window);
}
 wresult _w_shell_get_menu_bar(w_shell *shell,w_menu** menubar){
	 //return menuBar;
 }
w_shell* _w_shell_getModalShell () {
	/*Shell shell = null;
	Shell [] modalShells = display.modalShells;
	if (modalShells != null) {
		int bits = SWT.APPLICATION_MODAL | SWT.SYSTEM_MODAL;
		int index = modalShells.length;
		while (--index >= 0) {
			Shell modal = modalShells [index];
			if (modal != null) {
				if ((modal.style & bits) != 0) {
					Control control = this;
					while (control != null) {
						if (control == modal) break;
						control = control.parent;
					}
					if (control != modal) return modal;
					break;
				}
				if ((modal.style & SWT.PRIMARY_MODAL) != 0) {
					if (shell == null) shell = getShell ();
					if (modal.parent == shell) return modal;
				}
			}
		}
	}
	return null;*/
}
wresult _w_shell_get_modified(w_shell *shell) {
    return NSWindow_isDocumentEdited(_W_SHELL(shell)->window);
}
wresult _w_shell_get_images(w_shell *shell, w_image *image,
        size_t length){
	/*if (images == null) return new Image [0];
	Image [] result = new Image [images.length];
	System.arraycopy (images, 0, result, 0, images.length);
	return result;*/
}
//@Override
wresult _w_shell_get_minimized(w_shell *shell) {
	/*if (!getVisible ()) return super.getMinimized ();
	if (window == null) return false;
	return window.isMiniaturized();*/
}
 wresult _w_shell_get_minimum_size(w_shell *shell, w_size *size) {
     if (_W_SHELL(shell)->window == 0){
         memset(size, 0, sizeof(w_size));
         return W_TRUE;
     }
     NSWindow_minSize(_W_SHELL(shell)->window, size);
     return W_TRUE;
}
//@Override
 wresult _w_shell_getRegion (w_region* region) {
	/* This method is needed for the @since 3.0 Javadoc */
	/*checkWidget ();
	return region;*/
}

wresult _w_shell_get_shell(w_control *control,w_shell** shell) {
 	*shell = W_SHELL(control);
 	return W_TRUE;
 }
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator) {
    _w_toolkit_get_shells_from_parent(shell, iterator);
    return W_TRUE;
}
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc,void* user_data){
	//return text;
}
wresult _w_shell_get_text_unicode(w_shell *shell, w_alloc *text){
    
}

//@Override
float _w_shell_getThemeAlpha () {
	//return 1;
}
wresult _w_shell_get_toolbar(w_shell *shell,w_toolbar** toolbar) {
	/*checkWidget();
	if ((style & SWT.NO_TRIM) == 0) {
		if (toolBar == null) toolBar = new ToolBar(this, SWT.HORIZONTAL | SWT.SMOOTH, true);
	}
	return toolBar;*/
}

//@Override
wresult _w_shell_hasBorder () {
	//return false;
}

//@Override
wresult _w_shell_hasRegion () {
	//return region != null;
}

//@Override
void _w_shell_helpRequested(long id, long sel, long theEvent) {
	/*Control control = display.getFocusControl();
	while (control != null) {
		if (control.hooks (SWT.Help)) {
			control.postEvent (SWT.Help);
			break;
		}
		control = control.parent;
	}*/
}

//@Override
void _w_shell_invalidateVisibleRegion () {
	/*resetVisibleRegion ();
	if (toolBar != null) toolBar.resetVisibleRegion();
	invalidateChildrenVisibleRegion ();*/
}

//@Override
wresult _w_shell_isDrawing () {
	//return getDrawing ();
}

//@Override
wresult _w_shell_isEnabled () {
	/*checkWidget();
	return getEnabled ();*/
}

//@Override
wresult _w_shell_isEnabledCursor () {
	//return true;
}

//@Override
wresult _w_shell_isResizing () {
	//return (state & RESIZING) != 0;
}

//@Override
wresult _w_shell_isTransparent() {
	//return false;
}

//@Override
 wresult _w_shell_is_visible (w_control* shell) {
     return _w_control_get_visible(shell);
}

//@Override
wresult _w_shell_makeFirstResponder (long id, long sel, long responder) {
	/*Display display = this.display;
	boolean result = super.makeFirstResponder(id, sel, responder);
	if (!display.isDisposed()) display.checkFocus();
	return result;*/
}

void _w_shell_make_key_and_order_front(w_shell* shell) {
	/*
	* Bug in Cocoa.  If a child window becomes the key window when its
	* parent window is miniaturized, the parent window appears as if
	* restored to its full size without actually being restored. In this
	* case the parent window does become active when its child is closed
	* and the user is forced to restore the window from the dock.
	* The fix is to be sure that the parent window is deminiaturized before
	* making the child a key window.
	*/
	if (_W_CONTROL(shell)->parent != 0) {
		NSWindow* parentWindow = _w_shell_parent_window(shell);
        if (NSWindow_isMiniaturized(parentWindow)){
            NSWindow_deminiaturize(parentWindow, 0);
        }
	}
    NSWindow_makeKeyAndOrderFront(_W_SHELL(shell)->window, 0);
}

//@Override
void _w_shell_mouseMoved(long id, long sel, long theEvent) {
	//super.mouseMoved(id, sel, theEvent);

	/**
	 * Bug in AWT. WebViews need to have a mouseMove: handled by the window so it can generate
	 * DOMMouseMove events and also provide proper feedback to the window. However, the top-level
	 * view in an AWT window does not have the NSWindow as a next responder.
	 *
	 * Fix is to forward the message to the window if this is an embedded shell (that is, window == null)
	 */
	/*if (id == view.id && window == null) {
		view.window().mouseMoved(new NSEvent(theEvent));
	}*/
}

//@Override
void _w_shell_noResponderFor(long id, long sel, long selector) {
	/**
	 * Feature in Cocoa.  If the selector is keyDown and nothing has handled the event
	 * a system beep is generated.  There's no need to beep, as many keystrokes in the SWT
	 * are listened for and acted upon but not explicitly handled in a keyDown handler.  Fix is to
	 * not call the default implementation when a keyDown: is being handled.
	 */
	//if (selector != OS.sel_keyDown_) super.noResponderFor(id, sel, selector);
}
wresult _w_shell_open(w_shell *shell) {
	int mask =W_PRIMARY_MODAL | W_APPLICATION_MODAL | W_SYSTEM_MODAL;
	if ((_W_WIDGET(shell)->style & mask) != 0) {
		//display.setModalShell (this);
	} else {
		//updateModal ();
	}
	//bringToTop (false);
    _w_shell_set_window_visible(shell,W_TRUE, W_TRUE);
	if (w_widget_is_ok(W_WIDGET(shell)) <=0) return W_FALSE;
	/*if (!restoreFocus () && !traverseGroup (true)) {
		// if the parent shell is minimized, setting focus will cause it
		// to become unminimized.
		if (parent == null || !parentWindow ().isMiniaturized()) {
			setFocus ();
		}
	}*/
}

NSWindow* _w_shell_parent_window (w_shell *shell) {
    w_shell* parent = W_SHELL(_W_CONTROL(shell)->parent);
	if (parent == 0) return 0;
    return NSView_window(_W_WIDGET(parent)->handle);
}

//@Override
wresult _w_shell_print (w_graphics* gc) {
	/*checkWidget ();
	if (gc == null) error (SWT.ERROR_NULL_ARGUMENT);
	if (gc.isDisposed ()) error (SWT.ERROR_INVALID_ARGUMENT);
	return false;*/
}

//@Override
void _w_shell_releaseChildren (wresult destroy) {
	/*Shell [] shells = getShells ();
	for (int i=0; i<shells.length; i++) {
		Shell shell = shells [i];
		if (shell != null && !shell.isDisposed ()) {
			shell.dispose ();
		}
	}
	super.releaseChildren (destroy);*/
}

//@Override
wresult _w_shell_dispose (w_widget* widget) {
    NSObject* windowDelegate = 0;
    if (_W_SHELL(widget)->window != 0){
        windowDelegate = NSWindow_getDelegate(_W_SHELL(widget)->window);
        NSWindow_setDelegate(_W_SHELL(widget)->window, 0);
    }
    NSObject_release(NSOBJECT(_W_SHELL(widget)->window));
	//removeObserversFromWindow();
    if (windowDelegate != 0){
        NSObject_release(windowDelegate);
    }

	//super.releaseHandle ();
	//window = null;
    _w_toolkit_remove_shell(_W_SHELL(widget));
    if (mac_toolkit->shells == 0) {
        w_toolkit_post_quit(W_TOOLKIT(mac_toolkit), EXIT_SUCCESS);
    }
}

//@Override
void _w_shell_releaseParent () {
	/* Do nothing */
}

//@Override
void _w_shell_releaseWidget () {
	/*super.releaseWidget ();
	if (toolBar != null) {
		toolBar.dispose();
		toolBar = null;
	}
	if (tooltipTag != 0) {
		view.window().contentView().removeToolTip(tooltipTag);
		tooltipTag = 0;
	}
	display.clearModal (this);
	updateParent (false);
	display.updateQuitMenu();
	lastActive = null;*/
}

void _w_shell_removeObserversFromWindow () {
	/*NSNotificationCenter.defaultCenter().removeObserver(windowDelegate);

	if (hostWindow != null) {
		Integer embedCount = (Integer) windowEmbedCounts.get(hostWindow);
		if (embedCount == null) {
			embedCount = Integer.valueOf(0);
		}
		embedCount = Integer.valueOf(embedCount.intValue() - 1);
		windowEmbedCounts.put(hostWindow, embedCount);

		if (embedCount.intValue() <= 0) {
			windowEmbedCounts.remove(hostWindow);
			if (hostWindowClass != 0) OS.object_setClass(hostWindow.id, hostWindowClass);
			display.removeWidget(hostWindow);
			hostWindow.release();
			hostWindow = null;
			hostWindowClass = 0;
		}
	}*/
}

//@Override
 void _w_shell_requestLayout () {
	//layout (null, SWT.DEFER);
}

void _w_shell_sendToolTipEvent (wbool enter) {
	/*if (!isVisible()) return;
	NSWindow eventWindow = view.window();
	if (tooltipTag == 0) {
		NSView view = eventWindow.contentView();
		tooltipTag = view.addToolTipRect(new NSRect(), eventWindow, 0);
		if (tooltipTag != 0) {
			NSTrackingArea trackingArea = new NSTrackingArea(tooltipTag);
			id owner = trackingArea.owner();
			if (owner != null) tooltipOwner = owner.id;
			id userInfo = trackingArea.userInfo();
			if (userInfo != null) {
				tooltipUserData = userInfo.id;
			} else {
				long [] value = new long [1];
				OS.object_getInstanceVariable(tooltipTag, new byte[]{'_','u', 's', 'e', 'r', 'I', 'n', 'f', 'o'}, value);
				tooltipUserData = value[0];
			}
		}
	}
	if (tooltipTag == 0 || tooltipOwner == 0 || tooltipUserData == 0) return;
	NSPoint pt = eventWindow.convertScreenToBase(NSEvent.mouseLocation());
	NSEvent event = NSEvent.enterExitEventWithType(enter ? OS.NSMouseEntered : OS.NSMouseExited, pt, 0, 0, eventWindow.windowNumber(), null, 0, tooltipTag, tooltipUserData);
	OS.objc_msgSend(tooltipOwner, enter ? OS.sel_mouseEntered_ : OS.sel_mouseExited_, event.id);*/
}
wresult _w_shell_set_active(w_shell *shell) {
	/*if (window == null) return;
	checkWidget ();
	if (!isVisible()) return;
	makeKeyAndOrderFront ();*/
}

void _w_shell_setActiveControl (w_control* control) {
	//setActiveControl (control, SWT.None);
}

void _w_shell_setActiveControl_0 (w_control* control, int type) {
	/*if (control != null && control.isDisposed ()) control = null;
	if (lastActive != null && lastActive.isDisposed ()) lastActive = null;
	if (lastActive == control) return;*/

	/*
	* Compute the list of controls to be activated and
	* deactivated by finding the first common parent
	* control.
	*/
	/*Control [] activate = (control == null) ? new Control[0] : control.getPath ();
	Control [] deactivate = (lastActive == null) ? new Control[0] : lastActive.getPath ();
	lastActive = control;
	int index = 0, length = Math.min (activate.length, deactivate.length);
	while (index < length) {
		if (activate [index] != deactivate [index]) break;
		index++;
	}*/

	/*
	* It is possible (but unlikely), that application
	* code could have destroyed some of the widgets. If
	* this happens, keep processing those widgets that
	* are not disposed.
	*/
	/*for (int i=deactivate.length-1; i>=index; --i) {
		if (!deactivate [i].isDisposed ()) {
			deactivate [i].sendEvent (SWT.Deactivate);
		}
	}
	for (int i=activate.length-1; i>=index; --i) {
		if (!activate [i].isDisposed ()) {
			Event event = new Event ();
			event.detail = type;
			activate [i].sendEvent (SWT.Activate, event);
		}
	}*/
}
wresult _w_shell_set_alpha(w_shell *shell, int alpha) {
	if (_W_SHELL(shell)->window == 0) return W_FALSE;
	alpha &= 0xFF;
    NSWindow_setAlphaValue(_W_SHELL(shell)->window, alpha/255.);
    return W_TRUE;
}
wresult _w_shell_set_default_button(w_shell *shell, w_button *button){
	/*if (button != null) {
		if (button.isDisposed ()) error (SWT.ERROR_INVALID_ARGUMENT);
		if (button.menuShell () != this) error (SWT.ERROR_INVALID_PARENT);
		if ((button.style & SWT.PUSH) == 0) return;
	}
	if (button == defaultButton) return;
	defaultButton = button;
	NSButtonCell cell = null;
	if (defaultButton != null && (defaultButton.style & SWT.PUSH) != 0) {
		cell = new NSButtonCell (((NSButton)defaultButton.view).cell ());
	}
	view.window().setDefaultButtonCell (cell);
	display.updateDefaultButton();*/
}
//@Override
wresult _w_shell_set_bounds (w_control* shell,w_point* location,w_size* size) {
	if (_W_SHELL(shell)->window == 0) {
		// Embedded shells aren't movable.
		if (location != 0) return;
		if (size) {
            NSView_setFrameSize(_W_WIDGET(shell)->handle, size);
			return;
		}
	}
    if (_w_shell_get_full_screen(W_SHELL(shell))>0){
        _w_shell_set_full_screen(W_SHELL(shell), W_FALSE);
    }
	wbool sheet = NSWindow_isSheet(_W_SHELL(shell)->window);
	if (sheet && location!=0 && size==0) return;
    w_rect frame,screenframe;
    NSArray* screens = NSScreen_screens();
    NSScreen* screen =(NSScreen*) NSArray_objectAtIndex(screens, 0);
    NSScreen_frame(screen, &screenframe);
    int screenHeight = screenframe.height;
    NSWindow_frame(_W_SHELL(shell)->window, &frame);
    int x,y,width,height;
	if (location == 0) {
		x = (int)frame.x;
		y = screenHeight - (int)(frame.y + frame.height);
    }else{
        x = location->x;
        y = location->y;
    }
	if (size != 0) {
		w_size minSize;
        NSWindow_minSize(_W_SHELL(shell)->window, &minSize);
		width = w_max(size->width, (int)minSize.width);
		height = w_max(size->height, (int)minSize.height);
	} else {
		width = (int)frame.width;
		height = (int)frame.height;
	}
	if (sheet) {
		y = screenHeight - (int)(frame.y + frame.height);
        w_rect parentRect;
        w_shell* __shell;
        w_control_get_shell((w_control*)_W_CONTROL(shell)->parent, &__shell);
        NSWindow_frame(_W_SHELL(__shell)->window, &parentRect);
		frame.width = width;
		frame.height = height;
		frame.x = parentRect.x + (parentRect.width - frame.width) / 2;
		frame.y = screenHeight - (int)(y + frame.height);
        NSWindow_setFrame_0(_W_SHELL(shell)->window, &frame,_w_shell_is_visible(shell),W_TRUE);
	} else {
		frame.x = x;
		frame.y = screenHeight - (int)(y + height);
		frame.width = width;
		frame.height = height;
        NSWindow_setFrame(_W_SHELL(shell)->window, &frame,_w_shell_is_visible(shell));
	}
}

//@Override
void _w_shell_setClipRegion (NSView* view) {
	/*if (regionPath != null) {
		NSView rgnView = topView ();
		if (!rgnView.isFlipped()) rgnView = eventView ();
		NSPoint pt = view.convertPoint_toView_(new NSPoint(), rgnView);
		NSAffineTransform transform = NSAffineTransform.transform();
		transform.translateXBy(-pt.x, -pt.y);
		regionPath.transformUsingAffineTransform(transform);
		regionPath.addClip();
		transform.translateXBy(2*pt.x, 2*pt.y);
		regionPath.transformUsingAffineTransform(transform);
	}*/
}

//@Override
 void _w_shell_setEnabled (wbool enabled) {
	/*checkWidget();
	if (((state & DISABLED) == 0) == enabled) return;
	super.setEnabled (enabled);
	if (enabled && window != null && window.isMainWindow()) {
		if (!restoreFocus ()) traverseGroup (false);
	}*/
}
wresult _w_shell_set_full_screen(w_shell *shell, wbool fullScreen) {
	/*checkWidget ();
	if (window == null) return;
	if (_getFullScreen () == fullScreen) return;

	if ((window.collectionBehavior() & OS.NSWindowCollectionBehaviorFullScreenPrimary) != 0) {
		window.toggleFullScreen(Display.applicationDelegate);
		return;
	}

	this.fullScreen = fullScreen;
	if (fullScreen) {
		currentFrame = window.frame();
		window.setShowsResizeIndicator(false); //only hides resize indicator
		if (window.respondsToSelector(OS.sel_setMovable_)) {
			OS.objc_msgSend(window.id, OS.sel_setMovable_, 0);
		}

		fullScreenFrame = NSScreen.mainScreen().frame();
		if (getMonitor().equals(display.getPrimaryMonitor ())) {
			if (menuBar != null) {
				double menuBarHt = NSStatusBar.systemStatusBar().thickness();
				fullScreenFrame.height -= menuBarHt;
				OS.SetSystemUIMode(OS.kUIModeContentHidden, 0);
			}
			else {
				OS.SetSystemUIMode(OS.kUIModeAllHidden, 0);
			}
		}
		window.setFrame(fullScreenFrame, true);
		NSRect contentViewFrame = new NSRect();
		contentViewFrame.width = fullScreenFrame.width;
		contentViewFrame.height = fullScreenFrame.height;
		window.contentView().setFrame(contentViewFrame);
	} else {
		window.setShowsResizeIndicator(true);
		if (window.respondsToSelector(OS.sel_setMovable_)) {
			OS.objc_msgSend(window.id, OS.sel_setMovable_, 1);
		}
		OS.SetSystemUIMode(OS.kUIModeNormal, 0);
		window.setFrame(currentFrame, true);
	}*/
}
void _w_shell_sort (w_image* images) {
 	/* Shell Sort from K&R, pg 108 */
 	/*int length = images.length;
 	if (length <= 1) return;
 	ImageData [] datas = new ImageData [length];
 	for (int i = 0; i < length; i++) {
 		datas [i] = images [i].getImageData ();
 	}
 	for (int gap=length/2; gap>0; gap/=2) {
 		for (int i=gap; i<length; i++) {
 			for (int j=i-gap; j>=0; j-=gap) {
 				if (compare (datas [j], datas [j + gap]) >= 0) {
 					Image swap = images [j];
 					images [j] = images [j + gap];
 					images [j + gap] = swap;
 					ImageData swapData = datas [j];
 					datas [j] = datas [j + gap];
 					datas [j + gap] = swapData;
 				}
 			}
 		}
 	}*/
}
wresult _w_shell_set_images(w_shell *shell, w_image *image,
         size_t length){
		/*if (images == null) error(SWT.ERROR_INVALID_ARGUMENT);
		for (int i = 0; i < images.length; i++) {
			if (images [i] == null || images [i].isDisposed ()) error (SWT.ERROR_INVALID_ARGUMENT);
		}
		this.images = images;
		if (parent != null) return;
		if (display.dockImage == null) {
			if (images != null && images.length > 1) {
				Image [] bestImages = new Image [images.length];
				System.arraycopy (images, 0, bestImages, 0, images.length);
				sort (bestImages);
				images = bestImages;
			}
			if (images != null && images.length > 0) {
				display.application.setApplicationIconImage (images [0].handle);
			} else {
				display.application.setApplicationIconImage (null);
			}
		}*/
 }

//@Override
 wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu) {
	/*checkWidget();
	super.setMenuBar (menu);
	if (display.getActiveShell () == this) {
		display.setMenuBar (menuBar);
	}*/
}
wresult _w_shell_set_ime_input_mode(w_shell *shell, wbool mode) {
    return W_TRUE;
}
wresult _w_shell_set_maximized(w_shell *shell, wbool maximized) {
	if (_W_SHELL(shell)->window == 0) return W_FALSE;
	if (NSWindow_isZoomed(_W_SHELL(shell)->window) == maximized) return W_TRUE;
    NSWindow_zoom(_W_SHELL(shell)->window, 0);
    return W_TRUE;
}
wresult _w_shell_set_minimized(w_shell *shell, wbool minimized) {
	if (_W_SHELL(shell)->window == 0) return W_FALSE;
	//if (!getVisible()) return;
	if (minimized) {
		NSWindow_miniaturize (_W_SHELL(shell)->window,0);
	} else {
		NSWindow_deminiaturize (_W_SHELL(shell)->window,0);
	}
    return W_TRUE;
}
 wresult _w_shell_set_minimum_size(w_shell *shell, w_size *size) {
    if (_W_SHELL(shell)->window == 0) return W_FALSE;
    NSWindow_setMinSize(_W_SHELL(shell)->window, size);
    w_rect frame;
    NSWindow_frame(_W_SHELL(shell)->window, &frame);
	if (size->width > frame.width || size->height > frame.height) {
        frame.width = (int)(size->width > frame.width ? size->width : frame.width);
		frame.height = (int)(size->height > frame.height ? size->height : frame.height);
        _w_shell_set_bounds(W_CONTROL(shell), 0, &frame.sz);
	}
}
 wresult _w_shell_set_modified(w_shell *shell, wbool modified) {
     NSWindow_setDocumentEdited(_W_SHELL(shell)->window, modified);
     return W_TRUE;
}
//@Override
 void _w_shell_setRegion (w_region* region) {
	/*checkWidget ();
	if ((style & SWT.NO_TRIM) == 0) return;
	if (window == null) return;
	if (region != null) {
		if (region.isDisposed()) error(SWT.ERROR_INVALID_ARGUMENT);
		Rectangle bounds = region.getBounds();
		setSize(bounds.x + bounds.width, bounds.y + bounds.height);
	}
	this.region = region;
	if (regionPath != null) regionPath.release();
	regionPath = getPath(region);
	if (region != null) {
		window.setBackgroundColor(NSColor.clearColor());
	} else {
		window.setBackgroundColor(NSColor.windowBackgroundColor());
	}
	updateOpaque ();
	window.contentView().setNeedsDisplay(true);
	if (isVisible() && window.hasShadow()) {
		window.display();
		window.invalidateShadow();
	}*/
}

void _w_shell_setScrolling () {
	/*scrolling = true;
	view.performSelector(OS.sel_clearDeferFlushing, null, 0.0, display.runLoopModes());*/
}
wresult _w_shell_set_text(w_shell *shell, const char *string,
        size_t length) {
	if (string == 0) return W_ERROR_NULL_ARGUMENT;
	if (_W_SHELL(shell)->window == 0) return W_TRUE;
    NSString *title= NSString_stringWithUTF8(string,length);
    NSWindow_setTitle(_W_SHELL(shell)->window, title);
    NSObject_release(NSOBJECT(title));
    return W_TRUE;
}
wresult _w_shell_set_text_unicode(w_shell *shell,
        const wchar_t *string, size_t length){}
//@Override
 wresult _w_shell_set_visible (wbool visible) {
	/*checkWidget();
	int mask = SWT.PRIMARY_MODAL | SWT.APPLICATION_MODAL | SWT.SYSTEM_MODAL;
	if ((style & mask) != 0) {
		if (visible) {
			display.setModalShell (this);
		} else {
			display.clearModal (this);
		}
	} else {
		updateModal ();
	}
	if (window == null) {
		super.setVisible(visible);
	} else {
		setWindowVisible (visible, false);
	}*/
}
wresult _w_shell_set_window_visible (w_shell* shell,wbool visible, wbool key) {
    w_event e;
	if (visible) {
		if ((_W_WIDGET(shell)->state & STATE_HIDDEN) == 0) return;
		_W_WIDGET(shell)->state &= ~STATE_HIDDEN;
	} else {
		if ((_W_WIDGET(shell)->state & STATE_HIDDEN) != 0) return;
		_W_WIDGET(shell)->state |= STATE_HIDDEN;
	}
    NSWindow* window = _W_SHELL(shell)->window;
	if (window != 0 && (NSWindow_isVisible(window) == visible)) return;
	if (visible) {
		//display.clearPool ();
		if (_W_SHELL(shell)->center && !_W_SHELL(shell)->moved) {
			if (w_widget_is_ok(W_WIDGET(shell))<=0) return W_FALSE;
            _w_shell_center(shell);
		}
        e.type = W_EVENT_SHOW;
        e.platform_event = 0;
        e.time = 0;
        e.widget =W_WIDGET(shell);
        e.data = 0;
        _w_widget_send_event(W_WIDGET(shell), &e);
		if (w_widget_is_ok(W_WIDGET(shell))<=0) return W_FALSE;
        _w_control_reserved* reserved = _W_CONTROL_GET_RESERVED(shell);
        NSView* topView = reserved->top_view(W_WIDGET(shell));
        NSView_setHidden(topView, W_FALSE);
		//invalidateVisibleRegion();
		if (window != 0) {
            NSWindow* parentWindow = _w_shell_parent_window(shell);
			if ((_W_WIDGET(shell)->style & (W_SHEET)) != 0) {
				NSApplication* application = NSApplication_sharedApplication();
                NSApplication_beginSheet(application, parentWindow, 0, 0, 0, 0);
			} else {
				// If the parent window is miniaturized, the window will be shown
				// when its parent is shown.
				wbool parentMinimized = _W_CONTROL(shell)->parent != 0 && NSWindow_isMiniaturized(parentWindow);
				if (!parentMinimized) {
					if (key && (_W_WIDGET(shell)->style & W_NO_FOCUS) == 0) {
                        _w_shell_make_key_and_order_front(shell);
					} else {
                        NSWindow_orderFront(window, 0);
					}
					if (w_widget_is_ok(W_WIDGET(shell))<=0) return;
					if (_W_SHELL(shell)->minimized != NSWindow_isMiniaturized(window)) {
						if (_W_SHELL(shell)->minimized) {
                            NSWindow_miniaturize(window, 0);
						} else {
							NSWindow_deminiaturize (window, 0);
						}
					}
				}
			}
		}
		if (w_widget_is_ok(W_WIDGET(shell))<=0) return W_FALSE;
		//updateParent (visible);
		_W_SHELL(shell)->opened = true;
		if (!_W_SHELL(shell)->moved) {
			_W_SHELL(shell)->moved = true;
			e.type = W_EVENT_MOVE;
            e.platform_event = 0;
            e.time = 0;
            e.widget =W_WIDGET(shell);
            e.data = 0;
            _w_widget_send_event(W_WIDGET(shell), &e);
			if (w_widget_is_ok(W_WIDGET(shell))<=0) return W_FALSE;
		}
		if (!_W_SHELL(shell)->resized) {
			_W_SHELL(shell)->resized = true;
			e.type = W_EVENT_RESIZE;
            e.platform_event = 0;
            e.time = 0;
            e.widget =W_WIDGET(shell);
            e.data = 0;
            _w_widget_send_event(W_WIDGET(shell), &e);
			if (w_widget_is_ok(W_WIDGET(shell))<=0) return W_FALSE;
			//if (layout != null) {
                _W_CONTROL_RESERVED(reserved)->mark_layout(W_CONTROL(shell),0,_W_CONTROL_RESERVED(reserved));
                _W_CONTROL_RESERVED(reserved)->update_layout(W_CONTROL(shell),0,_W_CONTROL_RESERVED(reserved));
			//}
		}
	} else {
		/*updateParent (visible);
		if (window != null) {
			if ((style & (SWT.SHEET)) != 0) {
				NSApplication application = NSApplication.sharedApplication();
				application.endSheet(window, 0);
			}
			window.orderOut (null);
		}
		if (w_widget_is_ok(W_WIDGET(shell))<=0) return W_FALSE;
		topView ().setHidden (true);
		invalidateVisibleRegion();*/
        e.type = W_EVENT_HIDE;
        e.platform_event = 0;
        e.time = 0;
        e.widget =W_WIDGET(shell);
        e.data = 0;
        _w_widget_send_event(W_WIDGET(shell), &e);
	}

	/*if (w_widget_is_ok(W_WIDGET(shell))<=0) return W_FALSE;
	display.updateQuitMenu();

	if (w_widget_is_ok(W_WIDGET(shell))<=0) return W_FALSE;
	NSView[] hitView = new NSView[1];
	Control control = display.findControl (false, hitView);
	if (control != null && (!control.isActive() || !control.isEnabled())) control = null;
	Control trimControl = control;
	if (trimControl != null && trimControl.isTrim (hitView[0])) trimControl = null;
	display.checkEnterExit (trimControl, null, false);*/
}

//@Override
void _w_shell_set_zorder(w_control *control,_w_control_reserved* reserved) {
    /*NSScrollView* scrollView =(NSScrollView*) __w_scrollable_scroll_view(_W_WIDGET(control)->handle);
    if (scrollView != 0){
        NSScrollView_setDocumentView(scrollView, view);
    }*/
    NSWindow* window =_W_SHELL(control)->window;
	if (window == 0) return;
    NSWindow_setContentView(window, _W_WIDGET(control)->handle);
	if (_w_shell_fix_resize(W_SHELL(control))>0) {
        w_rect rect;
        NSWindow_frame(window, &rect);
		rect.x = rect.y = 0;
        NSView_setFrame(NSWindow_contentView(window), &rect);
	}
}

//@Override
void _w_shell_setZOrder (w_control* control, wbool above) {
	/*if (window == null) return;
	if (!getVisible ()) return;
	if (control == null) {
		if (above) {
			window.orderFront(null);
		} else {
			window.orderBack(null);
		}
	} else {
		NSWindow otherWindow = control.getShell().window;
		window.orderWindow(above ? OS.NSWindowAbove : OS.NSWindowBelow, otherWindow.windowNumber());
	}*/
}

//@Override
wresult _w_shell_traverseEscape () {
	/*if (parent == null) return false;
	if (!isVisible () || !isEnabled ()) return false;
	close ();
	return true;*/
}

//@Override
void _w_shell_updateCursorRects(wbool enabled) {
	//super.updateCursorRects(enabled);
	//if (toolBar != null) toolBar.updateCursorRects(enabled);
};

void _w_shell_updateModal () {
	// do nothing
}

void _w_shell_updateOpaque () {
	/*if (window == null) return;
	window.setOpaque (region == null && glContextCount == 0);*/
}

void _w_shell_updateParent (wbool visible) {
	/*if (window != null) {
		if (visible) {
			if (parent != null && parent.getVisible ()) {
				NSWindow parentWindow = display.modalPanel != null ? display.modalPanel : parentWindow ();
				parentWindow.addChildWindow (window, OS.NSWindowAbove);*/

				/**
				 * Feature in Cocoa: When a window is added as a child window,
				 * its window level resets to its parent's window level. So, we
				 * have to set the level for ON_TOP child window again.
				 */
				/*if ((style & SWT.ON_TOP) != 0) {
					window.setLevel(OS.NSStatusWindowLevel);
				} else if (OS.VERSION >= OS.VERSION(10, 11, 0)) {*/
					/*
					 * Feature in Cocoa on 10.11: Second-level child windows of
					 * a full-screen window are sometimes shown behind their
					 * parent window, although they take keyboard focus.
					 *
					 * The exact circumstances are unknown. Could only be
					 * reproduced when the app was launched with the Eclipse
					 * launcher. This hack is a workaround for
					 * https://bugs.eclipse.org/478975 .
					 */
					/*Shell parentShell = (Shell) parent;
					while (parentShell.parent != null) {
						parentShell = (Shell) parentShell.parent;
						if (parentShell._getFullScreen()) {
							window.setLevel(OS.NSSubmenuWindowLevel);
							break;
						}
					}
				}
			}
		} else {
			NSWindow parentWindow = window.parentWindow ();
			if (parentWindow != null) parentWindow.removeChildWindow (window);
		}
	}
	Shell [] shells = getShells ();
	for (int i = 0; i < shells.length; i++) {
		Shell shell = shells [i];
		if (shell.parent == this && shell.getVisible ()) {
			shell.updateParent (visible);
		}
	}*/
}

void _w_shell_updateSystemUIMode () {
	/*if ((window.collectionBehavior() & OS.NSWindowCollectionBehaviorFullScreenPrimary) != 0) return;
	if (!getMonitor ().equals (display.getPrimaryMonitor ())) return;
	int mode = display.systemUIMode, options = display.systemUIOptions;
	if (fullScreen) {
		mode = OS.kUIModeAllHidden;
		if (menuBar != null) {
			mode = OS.kUIModeContentHidden;
		}
		options = 0;
	}
	int[] uiMode = new int[1], uiOptions = new int[1];
	OS.GetSystemUIMode(uiMode, uiOptions);
	if (uiMode[0] != mode || uiOptions[0] != options) OS.SetSystemUIMode (mode, options);
	if (fullScreen)	window.setFrame(fullScreenFrame, true);*/
}

//@Override
wresult _ns_shell_view_stringForToolTip_point_userData (w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	/*NSPoint pt = new NSPoint();
	OS.memmove (pt, point, NSPoint.sizeof);
	Control control = display.findControl (false);
	if (control == null) return 0;
	Widget target = control.findTooltip (new NSView (view).convertPoint_toView_ (pt, null));
	String string = target.tooltipText ();
	if (string == null) return 0;
	char[] chars = new char [string.length ()];
	string.getChars (0, chars.length, chars, 0);
	int length = fixMnemonic (chars);
	return NSString.stringWithCharacters (chars, length).id;*/
}

//@Override
wresult _ns_shell_viewWillMoveToWindow(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	/*if (window == null) {
		long currentWindow = hostWindow != null ? hostWindow.id : 0;
		if (currentWindow != 0) {
			removeObserversFromWindow();
		}
		if (newWindow != 0) {
			attachObserversToWindow(new NSWindow(newWindow));
		}
	}*/
}

//@Override
wresult _ns_shell_windowDidBecomeKey(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	/*if (window != null) {
		Display display = this.display;
		display.setMenuBar (menuBar);
	}
	sendEvent (SWT.Activate);
	if (isDisposed ()) return;
	if (!restoreFocus () && !traverseGroup (true)) setFocus ();
	if (isDisposed ()) return;
	if ((window.collectionBehavior() & OS.NSWindowCollectionBehaviorFullScreenPrimary) == 0) {
		Shell parentShell = this;
		while (parentShell.parent != null) {
			parentShell = (Shell) parentShell.parent;
			if (parentShell._getFullScreen ()) {
				break;
			}
		}
		if (!parentShell._getFullScreen () || menuBar != null) {
			updateSystemUIMode ();
		} else {
			parentShell.updateSystemUIMode ();
		}
	}*/
}

//@Override
wresult _ns_shell_windowDidDeminiturize(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	/*minimized = false;
	sendEvent(SWT.Deiconify);*/
}

//@Override
wresult _ns_shell_windowDidMiniturize(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	/*minimized = true;
	sendEvent(SWT.Iconify);*/
}

//@Override
wresult _ns_shell_windowDidMove(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	/*moved = true;
	sendEvent(SWT.Move);*/
}

//@Override
wresult _ns_shell_windowDidResize(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	/*if (((window.collectionBehavior() & OS.NSWindowCollectionBehaviorFullScreenPrimary) == 0) && fullScreen) {
		window.setFrame(fullScreenFrame, true);
		NSRect contentViewFrame = new NSRect();
		contentViewFrame.width = fullScreenFrame.width;
		contentViewFrame.height = fullScreenFrame.height;
		window.contentView().setFrame(contentViewFrame);
	}
	if (fixResize ()) {
		NSRect rect = window.frame ();
		rect.x = rect.y = 0;
		window.contentView ().setFrame (rect);
	}*/
	_W_SHELL(widget)->resized = W_TRUE;
    w_event event;
    event.type = W_EVENT_RESIZE;
    event.time = 0;
    event.widget = widget;
    event.platform_event =(w_event_platform*) e;
    event.data = 0;
    _w_widget_send_event(widget, &event);
	if (!w_widget_is_ok(widget)) return W_FALSE;
	//if (layout != null) {
    _W_CONTROL_RESERVED(reserved)->mark_layout(W_CONTROL(widget),0,_W_CONTROL_RESERVED(reserved));
    _W_CONTROL_RESERVED(reserved)->update_layout(W_CONTROL(widget),0,_W_CONTROL_RESERVED(reserved));
	//}
}

//@Override
wresult _ns_shell_windowDidResignKey(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	/*if (display.isDisposed()) return;
	sendEvent (SWT.Deactivate);
	if (isDisposed ()) return;
	setActiveControl (null);
	if (isDisposed ()) return;
	saveFocus();*/
}

//@Override
wresult _ns_shell_windowSendEvent (w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
	NSEvent* nsEvent = e->args[0];
	int type = NSEvent_type(nsEvent);
	switch (type) {
		case NSLeftMouseDown:
		case NSRightMouseDown:
		case NSOtherMouseDown:
			/*display.clickCount = (int)(display.clickCountButton == nsEvent.buttonNumber() ? nsEvent.clickCount() : 1);
			display.clickCountButton = (int)nsEvent.buttonNumber();*/
			break;
		case NSLeftMouseUp:
		case NSRightMouseUp:
		case NSOtherMouseUp:
		case NSMouseMoved:
			/*NSView[] hitView = new NSView[1];
			Control control = display.findControl (false, hitView);
			if (control != null && (!control.isActive() || !control.isEnabled())) control = null;
			if (type == NSMouseMoved) {
				Control trimControl = control;
				if (trimControl != null && trimControl.isTrim (hitView[0])) trimControl = null;
				display.checkEnterExit (trimControl, nsEvent, false);
				// Browser will send MouseMoved in response to a DOM event, so don't send it here.
				if (trimControl != null && (trimControl.state & WEBKIT_EVENTS_FIX) != 0) trimControl = null;
				if (trimControl != null) trimControl.sendMouseEvent (nsEvent, type, false);
			}

			// Tooltip updating: Find the widget under the cursor. If it changed, clear the tooltip from
			// the last tracked item and send a tooltip event to make it visible on the new widget.
			Widget target = null;
			if (control != null) {
				NSPoint eventPoint = nsEvent.locationInWindow();
				if (hitView[0] != null) {
					NSWindow eventWindow = nsEvent.window();

					// If a NSMouseMoved happens on an inactive window, convert the
					// event coordinates to the window of the target view.
					if (eventWindow != null && eventWindow != hitView[0].window()) {
						eventPoint = eventWindow.convertBaseToScreen(eventPoint);
						eventPoint = hitView[0].window().convertScreenToBase(eventPoint);
					}
				}
				target = control.findTooltip (eventPoint);
			}

			if (display.tooltipControl != control || display.tooltipTarget != target) {
				Control oldControl = display.tooltipControl;
				Shell oldShell = oldControl != null && !oldControl.isDisposed() ? oldControl.getShell() : null;
				Shell shell = control != null && !control.isDisposed() ? control.getShell() : null;
				if (oldShell != null) oldShell.sendToolTipEvent (false);
				if (shell != null) shell.sendToolTipEvent (true);
			}
			display.tooltipControl = control;
			display.tooltipTarget = target;*/
			break;

		case NSKeyDown:
			/*
			* Feature in Cocoa.  For some reason, Cocoa does not perform accelerators
			* with ESC key code.  The fix is to perform the accelerators ourselves.
			*/
			//if (display.escAsAcceleratorPresent && nsEvent.keyCode() == 53 /* ESC */) {
				/*if (escMenuItem == null || escMenuItem.getAccelerator() != SWT.ESC) {
					updateEscMenuItem();
				}
				if (escMenuItem != null) {
					Menu parentMenu = escMenuItem.getParent();
					if (parentMenu != null) {
						NSMenu escNSMenu = parentMenu.nsMenu;
						if (escNSMenu != null) {
							escNSMenu.performKeyEquivalent(nsEvent);
						}
					}
				}
			}*/
			/**
			 * Feature in cocoa.  Control+Tab, Ctrl+Shift+Tab, Ctrl+PageDown and Ctrl+PageUp are
			 * swallowed to handle native traversal. If we find that, force the key event to
			 * the first responder.
			 */
			/*if ((nsEvent.modifierFlags() & OS.NSControlKeyMask) != 0) {
				NSString chars = nsEvent.characters();

				if (chars != null && chars.length() == 1) {
					int firstChar = (int)chars.characterAtIndex(0);

					// Shift-tab appears as control-Y.
					switch (firstChar) {
						case '\t':
						case 25:
						case OS.NSPageDownFunctionKey:
						case OS.NSPageUpFunctionKey:
							view.window().firstResponder().keyDown(nsEvent);
							return;
					}
				}
			}*/
			break;
	}

	// Window may have been disposed at this point.
	if (_w_widget_is_ok(widget)<=0) return;
	//super.windowSendEvent (id, sel, event);
}

void _ns_shell_updateEscMenuItem() {
	/*if (menuBar != null && !menuBar.isDisposed()){
		searchForEscMenuItem(menuBar);
	} else if (display.appMenuBar != null && !display.appMenuBar.isDisposed()) {
		searchForEscMenuItem(display.appMenuBar);
	}*/
}

wresult _ns_shell_searchForEscMenuItem(w_menu* menu) {
	/*if (menu == null || menu.isDisposed()) return false;
	MenuItem[] items = menu.getItems();
	if (items == null) return false;
	for (MenuItem item:items) {
		if (item == null || item.isDisposed()) {
			continue;
		} else if (item.getAccelerator() == SWT.ESC) {
			escMenuItem = item;
			return true;
		} else if ((item.getStyle() & SWT.CASCADE) != 0) {
			Menu subMenu = item.getMenu();
			if (searchForEscMenuItem(subMenu)) return true;
		}
	}
	return false;*/
}
wresult _ns_shell_windowShouldClose(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
    if (_w_control_is_enabled(W_CONTROL(widget))>0){
        _w_shell_close_widget(W_SHELL(widget), e);
    }
    e->result = W_FALSE;
	return W_TRUE;
}
wresult _ns_shell_windowWillClose(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
    return _w_shell_close_widget(W_SHELL(widget), e);
}

//@Override
w_widget* _w_shell_compute_tab_group (w_control* control) {
	return W_WIDGET(control);
}

//@Override
w_control* _w_shell_compute_tab_root (w_control* control) {
	return control;
}
//@Override
wresult _w_shell_is_reparentable (w_control* control) {
	return W_FALSE;
}

//@Override
wresult _w_shell_is_tab_group (w_control* control) {
	return W_TRUE;
}

//@Override
wresult _w_shell_is_tab_item (w_control* control) {
	return W_FALSE;
}
wresult _w_shell_restoreFocus () {
	/*if (savedFocus != null && savedFocus.isDisposed ()) savedFocus = null;
	if (savedFocus == null) return false;
	return savedFocus.setFocus ();*/
}

void _w_shell_saveFocus () {
	/*Control control = display._getFocusControl(view.window());
	if (control != null && control != this && this == control.menuShell ()) {
		setSavedFocus (control);
	}*/
}
void _w_shell_setSavedFocus (w_control* control) {
	//savedFocus = control;
}
//@Override
wresult _w_shell_traverse_item (w_control* control,wbool next) {
	return W_FALSE;
}

//@Override
wresult _w_shell_traverse_return (w_control* control) {
	/*if (defaultButton == null || defaultButton.isDisposed ()) return false;
	if (!defaultButton.isVisible () || !defaultButton.isEnabled ()) return false;
	defaultButton.click ();
	return true;*/
}

void _w_shell_class_init(struct _w_shell_class *clazz) {
	_w_canvas_class_init(W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SHELL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_shell_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_shell);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_shell);
	/*
	 * shell class
	 */
	W_WIDGET_CLASS(clazz)->create = _w_shell_create;
	W_CONTROL_CLASS(clazz)->get_shell = _w_shell_get_shell;
	clazz->close = _w_shell_close;
	clazz->get_toolbar = _w_shell_get_toolbar;
	clazz->get_alpha = _w_shell_get_alpha;
	clazz->get_full_screen = _w_shell_get_full_screen;
	clazz->get_minimum_size = _w_shell_get_minimum_size;
	clazz->get_modified = _w_shell_get_modified;
	clazz->get_ime_input_mode = _w_shell_get_ime_input_mode;
	clazz->get_shells = _w_shell_get_shells;
	clazz->open = _w_shell_open;
	clazz->set_active = _w_shell_set_active;
	clazz->set_alpha = _w_shell_set_alpha;
	clazz->set_full_screen = _w_shell_set_full_screen;
	clazz->set_ime_input_mode = _w_shell_set_ime_input_mode;
	clazz->set_minimum_size = _w_shell_set_minimum_size;
	clazz->set_modified = _w_shell_set_modified;
	clazz->force_active = _w_shell_force_active;
	clazz->get_default_button = _w_shell_get_default_button;
	//clazz->get_image = _w_shell_get_image;
	clazz->get_maximized = _w_shell_get_maximized;
	clazz->get_menu_bar = _w_shell_get_menu_bar;
	clazz->get_minimized = _w_shell_get_minimized;
	clazz->get_text = _w_shell_get_text;
	clazz->set_default_button = _w_shell_set_default_button;
    //clazz->set_image = _w_shell_set_image;
    clazz->set_maximized = _w_shell_set_maximized;
	clazz->set_menu_bar = _w_shell_set_menu_bar;
	clazz->set_minimized = _w_shell_set_minimized;
	clazz->set_text = _w_shell_set_text;
	/*
	 * Overrider class
	 */
	W_CONTROL_CLASS(clazz)->set_visible = _w_shell_set_visible;
    W_CONTROL_CLASS(clazz)->set_bounds = _w_shell_set_bounds;
    W_WIDGET_CLASS(clazz)->dispose = _w_shell_dispose;
	/*
	 * reserved
	 */
	struct _w_shell_reserved *reserved = _W_SHELL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
    _W_WIDGET_RESERVED(reserved)->create_handle = _w_shell_create_handle;
    _W_WIDGET_RESERVED(reserved)->get_client_area = _w_shell_get_client_area;
    _W_WIDGET_RESERVED(reserved)->check_open = _w_shell_check_open;
    _W_CONTROL_RESERVED(reserved)->find_cursor = _w_shell_find_cursor;
    _W_CONTROL_RESERVED(reserved)->set_zorder = _w_shell_set_zorder;
    _W_COMPOSITE_RESERVED(reserved)->find_deferred_control = _w_shell_find_deferred_control;
    /*
     messages
     */
    _w_msg* msgs = _W_WIDGET_RESERVED(reserved)->msgs;
    msgs[_NS_WILL_MOVE_TO_WINDOW]=_ns_shell_viewWillMoveToWindow;
    msgs[_NS_WINDOW_DID_BECOME_KEY]=_ns_shell_windowDidBecomeKey;
    msgs[_NS_WINDOW_DID_DEMINITURIZE]=_ns_shell_windowDidDeminiturize;
    msgs[_NS_WINDOW_DID_MINITURIZE]=_ns_shell_windowDidMiniturize;
    msgs[_NS_WINDOW_DID_MOVE]=_ns_shell_windowDidMove;
    msgs[_NS_WINDOW_DID_RESIZE]=_ns_shell_windowDidResize;
    msgs[_NS_WINDOW_DID_RESIGN_KEY]=_ns_shell_windowDidResignKey;
    msgs[_NS_WINDOW_SEND_EVENT]=_ns_shell_windowSendEvent;
    msgs[_NS_WINDOW_SHOULD_CLOSE]=_ns_shell_windowShouldClose;
    msgs[_NS_WINDOW_WILL_CLOSE]=_ns_shell_windowWillClose;
}
#endif
