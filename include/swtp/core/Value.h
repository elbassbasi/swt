/*
 * Value.h
 *
 *  Created on: 12 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CORE_VALUE_H_
#define SWTP_CORE_VALUE_H_
#include "String.h"

class SWTP_PUBLIC WValue {
public:
	w_value v;
	WValue() {
		w_value_init(&v);
	}
	WValue(bool _bool) {
		w_value_init_bool(&v, _bool);
	}
	WValue(char _char) {
		w_value_init_char(&v, _char);
	}
	WValue(short _short) {
		w_value_init_short(&v, _short);
	}
	WValue(int _int) {
		w_value_init_int(&v, _int);
	}
	WValue(wint64 _int) {
		w_value_init_int64(&v, _int);
	}
	WValue(float _float) {
		w_value_init_float(&v, _float);
	}
	WValue(double _double) {
		w_value_init_double(&v, _double);
	}
	WValue(const char *string) {
		w_value_init_string(&v, string, -1, 0);
	}
	WValue(const char *string, bool clone) {
		w_value_init_string(&v, string, -1, clone ? W_VALUE_FREE_MEMORY : 0);
	}
	template<typename T>
	WValue(T *v) {
		w_value_init(&v);
		this->v.clazz = &T::value_class;
		this->v.pointer = v;
	}
	~WValue() {
		w_value_free(&v);
	}
	void Free() {
		w_value_free(&v);
	}
	void SetInt(long long int int_) {
		w_value_set_int(&v, int_);
	}
	bool IsInt() {
		return v.clazz->type == W_VALUE_INT;
	}
	void SetFloat(double float_) {
		w_value_set_float(&v, float_);
	}
	bool SetString(const char *string, size_t length, int flags) {
		return w_value_set_string(&v, string, length, flags);
	}
	bool SetUserString(const char *string, size_t length, bool free) {
		return SetString(string, length,
				free ? W_VALUE_FREE_MEMORY | W_VALUE_USER_MEMORY : W_VALUE_USER_MEMORY);
	}
	void SetPointer(void *pointer, size_t length, int flags) {
		w_value_set_pointer(&v, pointer, length, flags);
	}
	void SetPointer(void *pointer, size_t length) {
		w_value_set_pointer(&v, pointer, length, 0);
	}
	void SetUserPointer(void *pointer, size_t length) {
		SetPointer(pointer, length, W_VALUE_USER_MEMORY);
	}
	bool SetString(const char *string, bool clone) {
		return SetString(string, -1, clone ? W_VALUE_FREE_MEMORY : 0);
	}
	void SetString(const char *string) {
		SetString(string, false);
	}
	bool CopyString(const char *string) {
		return SetString(string, true);
	}
	bool CopyString(const char *string, size_t length) {
		return SetString(string, length, W_VALUE_FREE_MEMORY);
	}
	/*void SetString(WString &str) {
	 w_value_set_string_ref(&v, str.ref);
	 }*/
	int Print(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int ret = Print(format, args);
		va_end(args);
		return ret;
	}
	int Sprint(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int ret = Print(format, args);
		va_end(args);
		return ret;
	}
	int Print(const char *format, va_list args) {
		return w_value_vprint(&v, format, args);
	}
};

#endif /* SWTP_CORE_VALUE_H_ */
