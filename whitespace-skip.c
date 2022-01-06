#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>

#define PPM_E_BADMAGIC  1  /* We expect {start of buffer}P6{whitespace} */
#define PPM_E_DATASTART 2  /* We expect {max_color_val}{whitespace}{data}
                              (and a comment's newline does not suffice) */
#define PPM_F_WSPFX 1     /* Skip initial whitespace too
                           * (Used in all but the initial call where P6 magic
                           * must come first) */
/* #define PPM_F_DATANEXT 2  /* Expecting data next. Without this, we skip
                           * whitespace after comments without checking.
                           * but when expecting data we need to ensure 
                           * Use this in all but the final call, where
                           * img data must be separated by whitespace even if
                           * the a full comment line ended with \n -- that \n
                           * is not considered whitespace) */

void skip_ws_comments(uint16_t *storei, uint8_t *wsfound,
		char *b, uint16_t i, uint16_t blen, uint8_t flags) {
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

struct ppm_info_st {
	uint16_t w, h, cmax, d_off; // width, height, color max, data offset
};

// returns 1 on error, 0 on false
// fills *pst with values on success (.w, .h, .cmax)
int get_ppm_info(struct ppm_info_st *pst, char *buf, uint16_t blen) {
	uint16_t i=0;
	uint8_t  ws_found; // if whitespace found
	char     *endp;
	skip_ws_comments(&i, &ws_found, buf, i, blen, 0);
	/* printf("Offset  : %d\n", i); */
	/* printf("Location: {{%s}}\n", buf+i); */
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
	if (!ws_found) return 1; // Err: No actual whitespace found before data

	pst->d_off = i;
	return 0;
}

struct tests_st {
	int shouldpass;
	char *buf;
	char *testmsg;
};

struct tests_st tests[] = {
	{ 1, "# Hey\nP6\n#Created with GIMP\n320 240\n255\ndata",
		"Proper example" },
	{ 0, "# Hey\nP6\n#Created with GIMP\n320 240\n255#error\ndata",
		"No whitespace between color max and data" },
	{ 1, "# Hey\nP6\n#Created with GIMP\n320 240\n255#error\n data",
		"Whitespace is proper between color max and data" },
};

int main(int argc, char *argv[]) {
	struct ppm_info_st ppminfo;
	// Loop through tests
	printf("Running %d tests:\n", sizeof(tests)/sizeof(*tests));
	for (int i=0; i<sizeof(tests)/sizeof(*tests); i++) {
		char *buf=tests[i].buf;
		int buflen=strlen(buf);
		if (get_ppm_info(&ppminfo, buf, buflen)) {
			if (tests[i].shouldpass) {
				printf("\033[31;1mFAILURE\033[;m: This example should have passed");
				printf(" Test #%d: String {{%s}}\n", i, buf);
			} else {
				printf("\033[32;1mPassed\033[;m: [%d] %s\n", i, tests[i].testmsg);
			}

		} else {
			if (!tests[i].shouldpass) {
				printf("\033[31;1mFAILURE\033[;m: This example should have errored");
				printf(" Test purpose: %s", tests[i].testmsg);
				printf(" Test #%d: String {{%s}}\n", i, buf);
			} else {
				printf("\033[32;1mPassed\033[;m: [%d] %s\n", i, tests[i].testmsg);
			}
			printf("Width: %d, Height: %d, Color max: %d, Data offset: %d\n",
					ppminfo.w,
					ppminfo.h,
					ppminfo.cmax,
					ppminfo.d_off);
			printf("Data: {{%s}}\n--------------------\n", buf + ppminfo.d_off);
			// ^ In our test we can print %s because buf is nul-terminated
			// But in actual image data, 0-color values might exist, AND
			// the data technically ends at the last blue value.
		}
	}
}
