/*
 * defs.h
 *
 *  Created on: 27 févr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CORE_DEFS_H_
#define SWT_CORE_DEFS_H_
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined _WIN32 || defined __CYGWIN__
    #ifdef __GNUC__
      #define DLL_EXPORT __attribute__ ((dllexport))
    #else
      #define DLL_EXPORT __declspec(dllexport)
    #endif
    #ifdef __GNUC__
      #define DLL_IMPORT __attribute__ ((dllimport))
    #else
      #define DLL_IMPORT __declspec(dllimport)
    #endif
#else
#if __GNUC__ >= 4
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#define DLL_IMPORT
#else
    #define DLL_IMPORT
    #define DLL_IMPORT
  #endif
#endif

#ifdef SWTBUILD
#define SWT_PUBLIC DLL_EXPORT
#else
#define SWT_PUBLIC DLL_IMPORT
#endif
typedef intptr_t wintptr;
typedef unsigned char wuchar;
typedef wchar_t wchar;
typedef wchar_t wwchar;
typedef unsigned int wunichar;
typedef short wshort;
typedef unsigned short wushort;
typedef int wint;
typedef unsigned int wuint;
typedef long long int wint64;
typedef unsigned long long int wuint64;
typedef wintptr wresult;
SWT_PUBLIC int w_max(int a, int b);
SWT_PUBLIC int w_min(int a, int b);
#ifdef __cplusplus
}
#endif
#endif /* SWT_CORE_DEFS_H_ */
