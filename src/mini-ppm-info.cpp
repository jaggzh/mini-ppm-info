/* #define DEBUG_MINI_PPM_INFO */
#if !defined(ARDUINO) && defined(DEBUG_MINI_PPM_INFO)
	#include <stdio.h>
	/* #include <string.h> */
	/* #include <stdint.h> */
#endif

#ifdef ARDUINO
	#include <ctype.h>
	#include <stdlib.h>
#endif

#include "mini-ppm-info.h"

void skip_ws_comments(uint16_t *storei, uint8_t *wsfound,
		const char *b, uint16_t i, uint16_t blen, uint8_t flags) {
	if (wsfound) *wsfound=0;
	if (flags & PPM_F_WSPFX) {
		uint16_t starti=i;
		for (; i<blen && isspace(b[i]); i++);
		if (i > starti && wsfound) *wsfound=1;
	}
	for (; i<blen; i++) {
		if (b[i] == '#') {
			i++;
			for (; i<blen && b[i] != '\n'; i++);
			if (b[i] != '\n') {
				*storei = i;
				return; // hit end of buffer
			}
			// comment line done. [i] is ON the \n
		} else if (isspace(b[i])) {
			if (wsfound && !*wsfound) *wsfound=1;
		} else {
			*storei = i;
			return;
		}
	}
	*storei = i;
	return;
}

int get_ppm_info(struct ppm_info_st *pst, const char *buf, uint16_t blen) {
	uint16_t i=0;
	uint8_t  ws_found; // if whitespace found
	char     *endp;
	skip_ws_comments(&i, &ws_found, buf, i, blen, 0);
	#ifdef DEBUG_MINI_PPM_INFO
		printf("Offset  : %d\n", i);
		printf("Location: {{%s}}\n", buf+i);
	#endif
	if (buf[i++] != 'P' || buf[i++] != '6' || !isspace(buf[i++])) return 1;
	skip_ws_comments(&i, &ws_found, buf, i, blen, PPM_F_WSPFX);

	pst->w = strtol(buf+i, &endp, 10);
	i = endp-buf;
	skip_ws_comments(&i, &ws_found, buf, i, blen, PPM_F_WSPFX);

	pst->h = strtol(buf+i, &endp, 10);
	i = endp-buf;
	skip_ws_comments(&i, &ws_found, buf, i, blen, PPM_F_WSPFX);

	pst->cmax = strtol(buf+i, &endp, 10);
	i = endp-buf;
	skip_ws_comments(&i, &ws_found, buf, i, blen, PPM_F_WSPFX);
	#ifdef DEBUG_MINI_PPM_INFO
		printf("Data: {{%s}}\n", buf+i);
		printf("Data-2: {{%s}}\n", buf+i-2);
	#endif

	// \/ \/  Err: Data exists, but was not whitespace-separated from colormax
	if (buf[i] && !ws_found) return 1;
	#ifdef PPM_REQUIRES_DATA
		// Err: No actual whitespace separating data, or no data found
		if (!ws_found || !*buf[i]) return 1;
		// ^^  We don't care if the header ends without whitespace, nor if no
		//     data follows, so we won't error unless the user defines ....
	#endif
	pst->d_off = i;

	return 0;
}

