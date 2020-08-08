/*
 * find_x.c: simple LSB steganography
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

/*
 * Simplifies access to a one-dimensional array
 * functioning in two-dimensions
 */
#define PIXEL_R (data[(y*w*n)+(x*n)+0])
#define PIXEL_G (data[(y*w*n)+(x*n)+1])
#define PIXEL_B (data[(y*w*n)+(x*n)+2])
#define PIXEL_A (data[(y*w*n)+(x*n)+3])

int main(int argc, char **argv){
  int x, y,
      w, h,
      n, i;
  FILE *fp;

usage:;
  if(argc < 2){
    printf("\x1b[36mUsage: find_x [mode]\n\x1b[33mWhere mode is one of:\n \x1b[35mencode\n decode\n diff\n\x1b[0m");
    return 0;
  }

  if(strcmp(argv[1], "encode") == 0){
    unsigned char *data, *temp, *input;

    data = stbi_load(argv[2], &w, &h, &n, 0);
    if(data == NULL){
      printf("\x1b[31mFailed to open file \x1b[35m\"%s\"\x1b[31m for reading.\n\x1b[0m", argv[2]);
      return 1;
    }
    
    puts("\x1b[34mInput data to be encoded:\x1b[0m");
    temp = malloc(w*h);
    input = malloc(w*h);
    fgets(temp, w*h, stdin);
    sprintf(input, " %s", temp);

    i = 0;
    for(y=0;y<h;y++){
      for(x=0;x<w;x++){
        if(i++ < strlen(input)){
          /*
           * Store upper two bits in red channel,
           * next two in green,
           * next two in blue,
           * and final two in alpha
           */
          PIXEL_R = (PIXEL_R & 0xfc) | ( input[i] >> 6);
          PIXEL_G = (PIXEL_G & 0xfc) | ((input[i] & 0x30) >> 4);
          PIXEL_B = (PIXEL_B & 0xfc) | ((input[i] & 0x0c) >> 2);
          PIXEL_A = (PIXEL_A & 0xfc) | ( input[i] & 0x03);
        } else if(i-1 == strlen(input)){
          /*
           * "Magic" signalling end of data stream
           */
          PIXEL_R = (PIXEL_R & 0xfc) | 0;
          PIXEL_G = (PIXEL_G & 0xfc) | 1;
          PIXEL_B = (PIXEL_B & 0xfc) | 2;
          PIXEL_A = (PIXEL_A & 0xfc) | 3;
          goto end_encode;
        }
      }
    }

end_encode:;
    stbi_write_png("output.png", w, h, n, data, w*n);

    printf("\x1b[32mSuccessfully wrote output file \x1b[35m\"output.png\"\x1b[32m.\n\x1b[0m");

    free(data);
    free(temp);
    free(input);
  } else if(strcmp(argv[1], "decode") == 0){
    unsigned char *data, *output;

    data = stbi_load(argv[2], &w, &h, &n, 0);
    if(data == NULL){
      printf("\x1b[31mFailed to open file \x1b[35m\"%s\"\x1b[31m for reading.\n\x1b[0m", argv[2]);
      return 1;
    }
    
    output = malloc(w*h);
    
    i = 0;
    for(y=0;y<h;y++){
      for(x=0;x<w;x++){
        /*
         * Detect "magic" and break
         * out of loop
         */
        if((PIXEL_R & 0x03) == 0 &&
           (PIXEL_G & 0x03) == 1 &&
           (PIXEL_B & 0x03) == 2 &&
           (PIXEL_A & 0x03) == 3){
          output[i] = '\0';
          goto end_decode;
        } else {
          /*
           * Retrieve data from lower
           * two bits of each channel
           */
          output[i++] =
            ((PIXEL_R & 0x03) << 6) | 
            ((PIXEL_G & 0x03) << 4) |
            ((PIXEL_B & 0x03) << 2) |
            (PIXEL_A & 0x03);
        }
      }
    }

end_decode:;
    if(argv[3] == NULL){
      printf("%s", output);
    } else {
      fp = fopen(argv[3], "w");
      if(fp == NULL){
        printf("\x1b[31mFailed to open file \x1b[35m\"%s\"\x1b[31m for reading.\n\x1b[0m", argv[3]);
      }
      fwrite(output, i, 1, fp);
      fclose(fp);
      printf("\x1b[32mSuccessfully wrote output file \x1b[35m\"%s\"\x1b[32m.\n\x1b[0m", argv[3]);
    }

    free(data);
    free(output);
  } else if(strcmp(argv[1], "diff") == 0){
    unsigned char *inp, *out;

    inp = stbi_load(argv[2], &w, &h, &n, 0);
    if(inp == NULL){
      printf("\x1b[31mFailed to open file \x1b[35m\"%s\"\x1b[31m for reading.\n\x1b[0m", argv[2]);
      return 1;
    }

    out = stbi_load(argv[3], &w, &h, &n, 0);
    if(out == NULL){
      printf("\x1b[31mFailed to open file \x1b[35m\"%s\"\x1b[31m for reading.\n\x1b[0m", argv[3]);
      return 1;
    }

    for(i=0;i<w*h*n;i++){
      if(out[i] != inp[i]){
        /*
         * Amplify color difference,
         * otherwise pixels are
         * near-invisible
         */
        out[i] <<= 6;
      } else {
        out[i] = 0;
      }
    }

    stbi_write_png("diff.png", w, h, n, out, w*n);

    printf("\x1b[32mSuccessfully wrote output file \x1b[35m\"diff.png\"\x1b[32m.\n\x1b[0m");
  } else {
    argc = 0;
    goto usage;
  }

  return 0;
}
