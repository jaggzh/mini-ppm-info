#include <stdio.h>
#include <string.h>
/* #include <stdint.h> */
/* #include <ctype.h> */
/* #include <stdlib.h> */

#include "mini-ppm-info.h"

struct tests_st {
	int shouldpass;
	char *buf;
	char *testmsg;
};

struct tests_st tests[] = {
	// shouldpass
	{ 1, "# Hey\nP6\n#Created with GIMP\n320 240\n255\ndata",
		"Proper example" },
	{ 1, "# Hey\nP6\n#Created #with GIMP\n320 240\n255\ndata",
		"Proper example with # in comment" },
	{ 0, "# Hey\nP6\n#Created with GIMP\n320 240\n255#error\ndata",
		"No whitespace between color max and data" },
	{ 1, "# Hey\nP6\n#Created with GIMP\n320 240\n255#error\n data",
		"Whitespace is proper between color max and data" },
	{ 1, "# Hey\nP6\n#Created with GIMP\n320 240\n255 #error\n",
		"Should not error. Header only, no data, but whitespace before comment" },
	{ 1, "# Hey\nP6\n#Created with GIMP\n320 240\n255\n",
		"Should not error if it's only the header (no data with whitespace)" },
	{ 1, "# Hey\nP6\n#Created with GIMP\n320 240\n255#error\n",
		"Should not error. Header only, no data, and comment excludes whitespace" },
};

int main(int argc, char *argv[]) {
	struct ppm_info_st ppminfo;
	// Loop through tests
	printf("Running %d tests:\n", (int)(sizeof(tests)/sizeof(*tests)));
	for (int i=0; i<sizeof(tests)/sizeof(*tests); i++) {
		char *buf=tests[i].buf;
		int buflen=strlen(buf);
		if (get_ppm_info(&ppminfo, buf, buflen)) {
			if (tests[i].shouldpass) {
				printf("Test #%d: String {{%s}}\n", i, buf);
				printf("\033[31;1mFAILURE\033[;m: This example should have passed\n");
				printf(" Test purpose: %s\n", tests[i].testmsg);
			} else {
				printf("Test %d: \033[32;1mPassed\033[;m: %s\n", i, tests[i].testmsg);
			}

		} else {
			if (!tests[i].shouldpass) {
				printf("Test #%d: String {{%s}}\n", i, buf);
				printf("\033[31;1mFAILURE\033[;m: This example should have errored, but it didn't, so it's a failure.\n");
				printf(" Test purpose: %s\n", tests[i].testmsg);
			} else {
				printf("Test %d: \033[32;1mPassed\033[;m: %s\n", i, tests[i].testmsg);
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
