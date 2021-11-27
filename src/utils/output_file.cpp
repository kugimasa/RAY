//
// Created by kugimasa on 2021/11/23.
//

#include "output_file.h"
#include "my_print.h"

int pngFileEncodeWrite(BITMAPDATA_t *bitmapData, const char *filename){
  FILE *fo;
  int j;

  png_structp png = NULL;
  png_infop info = NULL;
  png_bytepp datap = NULL;
  png_byte type;

  fo = fopen(filename, "wb");
  if (fo == NULL) {
    std::cout << "🛑 Cannot open " << filename << "." << std::endl;
    return -1;
  }

  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png == NULL) {
    error_print("png_create_write_struct");
    return -1;
  }
  info = png_create_info_struct(png);

  if(bitmapData->ch == 3) {
    type = PNG_COLOR_TYPE_RGB;
  } else if(bitmapData->ch == 4) {
    type = PNG_COLOR_TYPE_RGB_ALPHA;
  } else {
    error_print("ch num is invalid!");
    png_destroy_write_struct(&png, &info);
    fclose(fo);
    return -1;
  }
  png_init_io(png, fo);
  png_set_IHDR(png, info, bitmapData->width, bitmapData->height, 8, type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  datap = static_cast<png_bytepp>(png_malloc(png, sizeof(png_bytep) * bitmapData->height));

  png_set_rows(png, info, datap);

  for(j = 0; j < bitmapData->height; j++){
    datap[j] = static_cast<png_bytep>(png_malloc(png, bitmapData->width * bitmapData->ch));
    memcpy(datap[j], bitmapData->data + j * bitmapData->width * bitmapData->ch, bitmapData->width * bitmapData->ch);
  }
  png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);

  for(j = 0; j < bitmapData->height; j++){
    png_free(png, datap[j]);
  }
  png_free(png, datap);

  png_destroy_write_struct(&png, &info);
  fclose(fo);
  return 0;
}


int freeBitmapData(BITMAPDATA_t *bitmap){
  if(bitmap->data != NULL){
    free(bitmap->data);
    bitmap->data = NULL;
  }
  return 0;
}