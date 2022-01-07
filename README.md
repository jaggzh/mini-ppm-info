# mini-ppm-info library (useful for MCUs, Arduino, etc.)

Tiny library to parse incoming potentially-chunked PPM image
header information.

It retrieves the width, height, colormax (ex. 255), and
gives the offset to the data.



## Usage:

Copy mini-ppm-info.c and .h to your project (or otherwise link the .c)

 c
/* Optionally: Define this if you require data to exist. Without it,
data is not required (only the header parsed), AND whitespace is not
needed after the header. */
//#define PPM_REQUIRES_DATA // optional
#include "mini-ppm-info.h"

struct ppm_info_st ppminfo;
err = get_ppm_info(&ppminfo, ppm_data, length_of_ppm_data);

int get_ppm_info(struct ppm_info_st *pst, char *buf, uint16_t blen);
// returns 1 on error, 0 on false
// Fills *pst with values on success (.w, .h, .cmax)
// buf: Buffer which contains at least the header and maybe some data
// blen: Length of actual data available (so we don't run off the end)
//       it is not necessarily your buffer's full size, for we stop when
//       we reach the start of data anyway.
 
