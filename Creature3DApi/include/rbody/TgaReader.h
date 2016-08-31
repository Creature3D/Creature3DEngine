#ifndef _TGAREADER_H
#define _TGAREADER_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

unsigned char *simage_tga_load(const char *filename, int *width_ret, int *height_ret, int *numComponents_ret);

#endif // _TGAREADER_H
