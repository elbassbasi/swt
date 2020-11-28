/*
 * stream.h
 *
 *  Created on: 19 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CORE_STREAM_H_
#define SWT_CORE_STREAM_H_
#include "defs.h"
#ifdef __cplusplus
extern "C" {
#endif
#define W_SEEK_SET SEEK_SET
#define W_SEEK_CUR SEEK_CUR
#define W_SEEK_END SEEK_END
typedef struct w_stream {
	void *v_table; //used in c++
	struct _w_stream_class *clazz;
} w_stream;
struct _w_stream_class {
	wresult (*close)(w_stream *stream);
	wresult (*read)(w_stream *stream, void *pv, size_t cb, size_t *pcbRead);
	wresult (*write)(w_stream *stream, const void *pv, size_t cb,
			size_t *pcbWritten);
	wresult (*seek)(w_stream *stream, wuint64 dlibMove, int dwOrigin);
	wresult (*tell)(w_stream *stream, wuint64 *pos);
	wresult (*rewind)(w_stream *stream);
	wresult (*set_size)(w_stream *stream, wuint64 libNewSize);
	wresult (*get_size)(w_stream *stream, wuint64 *libNewSize);
};

#ifdef __cplusplus
}
#endif
#endif /* SWT_CORE_STREAM_H_ */
