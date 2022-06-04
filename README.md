# mini-ppm-info library (useful for MCUs, Arduino, etc.)

Tiny library to parse incoming potentially-chunked PPM image
header information.

It retrieves the width, height, colormax (ex. 255), and
gives the offset to the data.

## Installation Methods:
1. Method 1: This might be available in Arduino's library manager soon
2. Method 2: Copy files directly into your project folder
3. Method 3: Include as a subrepository (`git submodule add https://github.com/jaggzh/mini-ppm-info`) (Then you must reference those files in the subdir).  Use `git submodule update` within the mini-ppm-info/ directory to pull updates later.

## Usage:
```c
/* Define this if you require data to exist, otherwise the lib
   accepts plain headers, AND whitespace is not needed after the header. */
//#define PPM_REQUIRES_DATA // optional
#include "mini-ppm-info.h"

/* Call with a char pointer to some PPM data.
   The data should contain at least the header of the PPM image and,
   for safety, datalen limits how far get_ppm_info() will look.
   If all is well, the width, height, max color value, and offset of
   the start of actual image data will be returned. */
void test_ppm_info(char *ppm_data, uint16_t datalen) {
	struct ppm_info_st ppminfo;
	err = get_ppm_info(&ppminfo, ppm_data, length_of_ppm_data);
	// returns 1 on error, 0 on false
	printf("Width: %d, Height: %d, Color max: %d, Data offset: %d\n",
			ppminfo.w,
			ppminfo.h,
			ppminfo.cmax,
			ppminfo.d_off);
}

// Reference from mini-ppm-info.h:
// int get_ppm_info(struct ppm_info_st *pst, char *buf, uint16_t blen);
// Fills *pst with values on success (.w, .h, .cmax)
// buf: Buffer which contains at least the header and maybe some data
// blen: Length of actual data available (so we don't run off the end)
//       it is not necessarily your buffer's full size, for we stop when
//       we reach the start of data anyway.
```
