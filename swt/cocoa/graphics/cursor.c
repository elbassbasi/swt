/*
 * cursor.c
 *
 *  Created on: 1 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "graphics.h"
void w_cursor_init(w_cursor *cursor) {
}
void w_cursor_dispose(w_cursor *cursor) {
}
wresult w_cursor_create(w_cursor *cursor, w_image *source,
		int hotspotX, int hotspotY) {
	/*
	if (source == null) SWT.error(SWT.ERROR_NULL_ARGUMENT);
	if (mask == null) {
		if (!(source.getTransparencyType() == SWT.TRANSPARENCY_MASK)) SWT.error(SWT.ERROR_NULL_ARGUMENT);
		mask = source.getTransparencyMask();
	}
	// Check the bounds. Mask must be the same size as source
	if (mask.width != source.width || mask.height != source.height) {
		SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	}
	// Check the hotspots
	if (hotspotX >= source.width || hotspotX < 0 ||
		hotspotY >= source.height || hotspotY < 0) {
		SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	}
	// Convert depth to 1
	source = ImageData.convertMask(source);
	mask = ImageData.convertMask(mask);

	// Swap the bits in each byte and convert to appropriate scanline pad
	byte[] sourceData = new byte[source.data.length];
	byte[] maskData = new byte[mask.data.length];
	byte[] data = source.data;
	for (int i = 0; i < data.length; i++) {
		byte s = data[i];
		sourceData[i] = (byte)(((s & 0x80) >> 7) |
			((s & 0x40) >> 5) |
			((s & 0x20) >> 3) |
			((s & 0x10) >> 1) |
			((s & 0x08) << 1) |
			((s & 0x04) << 3) |
			((s & 0x02) << 5) |
			((s & 0x01) << 7));
		sourceData[i] = (byte) ~sourceData[i];
	}
	sourceData = ImageData.convertPad(sourceData, source.width, source.height, source.depth, source.scanlinePad, 1);
	data = mask.data;
	for (int i = 0; i < data.length; i++) {
		byte s = data[i];
		maskData[i] = (byte)(((s & 0x80) >> 7) |
			((s & 0x40) >> 5) |
			((s & 0x20) >> 3) |
			((s & 0x10) >> 1) |
			((s & 0x08) << 1) |
			((s & 0x04) << 3) |
			((s & 0x02) << 5) |
			((s & 0x01) << 7));
		maskData[i] = (byte) ~maskData[i];
	}
	maskData = ImageData.convertPad(maskData, mask.width, mask.height, mask.depth, mask.scanlinePad, 1);
	handle = createCursor(maskData, sourceData, source.width, source.height, hotspotX, hotspotY, true);
	if (handle == 0) SWT.error(SWT.ERROR_NO_HANDLES);*/
}
#endif
