/*
 * App.h
 *
 *  Created on: 27 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_APP_H_
#define SWTP_WIDGETS_APP_H_
#include "Toolkit.h"
class SWTP_PUBLIC WApp: public w_app {
public:
	WApp() {
		w_app_init(this);
	}
	WApp(int argc, char **argv) {
		w_app_init_with_args(this, argc, argv);
	}
	~WApp() {
		w_app_dispose(this);
	}
	static WApp* Get() {
		return (WApp*) w_app_get();
	}
	int GetArgc() {
		return this->argc;
	}
	char** GetArgv() {
		return this->argv;
	}
	const char* GetExecutableFile() {
		return w_app_get_executable_file(this);
	}
	const char* GetExecutablePath() {
		return w_app_get_executable_path(this);
	}
	const char* GetCurrentDirectory() {
		return w_app_get_current_directory(this);
	}
	static int Run() {
		return WToolkit::GetDefault()->Run();
	}
	static void Exit(int exit_code) {
		WToolkit::GetDefault()->PostQuit(exit_code);
	}
	static void Exit() {
		WToolkit::GetDefault()->PostQuit();
	}
	static WThread* GetToolkitThread() {
		return WToolkit::GetDefault()->GetThread();
	}
	WThread* GetMainThread() {
		return (WThread*) &this->mainthread;
	}
	bool IsMainThread() {
		return w_threadid_equal(w_app_get()->mainthread.id,
				w_thread_get_current_id());
	}
};

namespace swt {
typedef ::WApp App;
}  // namespace swt

#endif /* SWTP_WIDGETS_APP_H_ */
