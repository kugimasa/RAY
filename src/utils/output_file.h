//
// Created by kugimasa on 2021/11/23.
//

#ifndef RAY_UTILS_OUTPUT_FILE_H_
#define RAY_UTILS_OUTPUT_FILE_H_

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "png.h"

typedef struct{
  unsigned char *data;
  unsigned int width;
  unsigned int height;
  unsigned int ch;
} BITMAPDATA_t;

int pngFileEncodeWrite(BITMAPDATA_t *, const char*);
int freeBitmapData(BITMAPDATA_t *);

#endif //RAY_UTILS_OUTPUT_FILE_H_