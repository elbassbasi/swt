/*
 * stream.c
 *
 *  Created on: 23 oct. 2020
 *      Author: azeddine
 */
#ifdef __linux
#include "graphics.h"
#include "../widgets/toolkit.h"
struct WInputStream {
	GInputStream parent;
	w_stream *stream;
};
GInputStream* _w_input_stream_new(w_stream *stream) {
	return 0;
}
GOutputStream* _w_output_stream_new(w_stream *stream){
	return 0;
}
#endif
