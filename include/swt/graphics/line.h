/*
 * pen.h
 *
 *  Created on: 13 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_GRAPHICS_LINE_H_
#define SWT_GRAPHICS_LINE_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_line_dash {
	int length;
	float dash[0];
}w_line_dash;
typedef struct w_line_attributes {
	unsigned style :3;
	unsigned cap :2;
	unsigned join :2;
	unsigned dashOffset :13;
	unsigned miterLimit :12;
	float width;
	w_line_dash* dash;
}w_line_attributes;
typedef struct w_line {
    unsigned style :3;
    unsigned cap :2;
    unsigned join :2;
    unsigned dashOffset :13;
    unsigned miterLimit :12;
    float width;
    w_line_dash dash;
} w_line;

#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_LINE_H_ */
