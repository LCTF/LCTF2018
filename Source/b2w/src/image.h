/******************************************************************************
The MIT License
Copyright (c) 2017-2018 Yeonji
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_ 1

#include <stdint.h>

typedef struct {
    int32_t     magic;      /* Should Always Be 0x474D4959 */
    int32_t     width;
    int32_t     height;
    int32_t     model;
    int32_t     nChannels;
    int32_t     lStep;
    int32_t     pStep;
    float      *data;
} image_t;

/* image magic number */
#define IMG_MAGIC           0x474D4959

/* image model */
#define IMG_MODEL_CIEXYZ    0
#define IMG_MODEL_BGR       1
#define IMG_MODEL_BGRA      2
#define IMG_MODEL_YUV       3
#define IMG_MODEL_YCBCR     4
#define IMG_MODEL_HSV       5
#define IMG_MODEL_HSL       6
#define IMG_MODEL_CYMK      7
#define IMG_MODEL_GRAY      8
#define IMG_MODEL_Y         9

#define IMG_MODEL_RGB       IMG_MODEL_BGR

/* image channel rgba */
#define IMG_CHANNEL_B       0
#define IMG_CHANNEL_G       1
#define IMG_CHANNEL_R       2
#define IMG_CHANNEL_A       3

image_t * image_new(int32_t width, int32_t height, int32_t model);

inline float * image_pixel(image_t * img, int32_t x, int32_t y)
{
    return (img->data) + x * (img->pStep) + y * (img->lStep);
}

int  image_save(image_t * img, const char * path);
void image_free(image_t * img);

#endif /* _IMAGE_H_ */
