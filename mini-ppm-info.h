#include <stdint.h>

// Options
//#define PPM_REQUIRES_DATA  // Use if you want to ensure whitespace+data
                             // is in the PPM. (It only checks for one byte
                             // of data.)

/* Mini PPM header information lib
 * CC-By: jaggz.h {over at} gmail com
 * Gets width, height, color max, and offset to data
 * NO VALIDATION is done of the data. For instance, we do not
 * examine if it has enough for the width or height.
 * This is because it's intended for chunked data being processed by
 * an MCU.
 * The main requirement is that the buffer contain at least the header
 * to the start of data.
 */
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

// Result information:
struct ppm_info_st {
	uint16_t w, h, cmax, d_off; // width, height, color max, data offset
};

// This is the main call:
//   Example:
//   struct ppm_info_st ppminfo;
//   get_ppm_info(&ppminfo, ppm_data, length_of_ppm_data);
// returns 1 on error, 0 on false
// returns: Fills *pst with values on success (.w, .h, .cmax)
// psg: Pointer to existing struct to fill with info on success
// buf: Buffer which contains at least the header and maybe some data
// blen: length of actual data available (so we don't run off the end)
//       it is not necessarily your buffer's full size, and we stop when
//       we reach the start of data anyway.
int get_ppm_info(struct ppm_info_st *pst, char *buf, uint16_t blen);

// Internal use (skips whitespace + comments)
void skip_ws_comments(uint16_t *storei, uint8_t *wsfound,
		char *b, uint16_t i, uint16_t blen, uint8_t flags);

