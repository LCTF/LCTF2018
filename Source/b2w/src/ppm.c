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


#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <image.h>

int ppm_read_header(FILE * fp, int32_t *width, int32_t *height, int32_t *depth)
{
    int32_t data[3] = {0};
    int     i = 0;
    int     ch = 0;

    while( i < 3 ) {
        ch = fgetc(fp);
        if ( ch == '#' ) {
            while ( (ch = fgetc(fp)) != EOF ) {
                if ( ch == '\n' ) break;
                if ( ch == '\r' ) {
                    ch = fgetc(fp);
                    if ( ch == '\n' ) {
                        break;
                    }
                    ungetc(ch, fp);
                    break;
                }
            }
        } else {
            ungetc(ch, fp);
            fscanf(fp, "%d", &data[i]);
            i++;
        }
    }

    *width = data[0];
    *height = data[1];

    if (data[2] < 255) {
        return -1;
    } else if (data[2] <= 255) {
        *depth = 8;
    } else if  (data[2] <= 65535) {
        *depth = 16;
    } else {
        *depth = 32;
    }

    return 0;
}

image_t * ppm_p3_read(FILE * fp)
{
    int32_t width;
    int32_t height;
    int32_t depth;
    int32_t x;
    int32_t y;
    int32_t R;
    int32_t G;
    int32_t B;

    image_t *img;

    fprintf(stderr, "Format: Portable PixMap in ASCII (P3)\n");

    /* Read Header */
    ppm_read_header(fp, &width, &height, &depth);

    fprintf(stderr, "Image Size: %d x %d.\n", width, height);
    fprintf(stderr, "Color Depth: %d bit.\n", 3 * depth);

    img = image_new(width, height, IMG_MODEL_BGR);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            fscanf(fp, "%d", &R);
            fscanf(fp, "%d", &G);
            fscanf(fp, "%d", &B);
            image_pixel(img, x, y)[IMG_CHANNEL_R] = R;
            image_pixel(img, x, y)[IMG_CHANNEL_G] = G;
            image_pixel(img, x, y)[IMG_CHANNEL_B] = B;
        }
    }

    return img;
}

#if 0

image_t * ppm_p1_read(FILE * fp)
{
    fprintf(stderr, "Format: Portable Bitmap in ASCII (P1)\n");
    return NULL;
}

image_t * ppm_p2_read(FILE * fp)
{
    fprintf(stderr, "Format: Portable GrayMap in ASCII (P2)\n");
    return NULL;
}

image_t * ppm_p4_read(FILE * fp)
{
    fprintf(stderr, "Format: Portable Bitmap in Binary (P4)\n");
    return NULL;
}

image_t * ppm_p5_read(FILE * fp)
{
    fprintf(stderr, "Format: Portable GrayMap in Binary (P5)\n");
    return NULL;
}

image_t * ppm_p6_read(FILE * fp)
{
    fprintf(stderr, "Format: Portable GrayMap in Binary (P6)\n");
    return NULL;
}
#endif

image_t * ppm_read(FILE * fp)
{
    image_t    *img = NULL;
    int         num = 0;

    fseek(fp, 0, SEEK_SET);

    /* Read Magic */
    if(!fscanf(fp, "P%d\n", &num)) {
        fprintf(stderr, "Not PPM Format\n");
        return NULL;
    }

#if 0
    switch ( num ) {
        case 1: img = ppm_p1_read(fp); break;
        case 2: img = ppm_p2_read(fp); break;
        case 3: img = ppm_p3_read(fp); break;
        case 4:
        case 5:
        case 6:
        default:
            fprintf(stderr, "Not PPM Format\n");
    }
#endif

    img = ppm_p3_read(fp);

    return img;
}


int ppm_save(image_t * img, const char * path)
{
    FILE       *fp = NULL;

    int32_t     x = 0;
    int32_t     y = 0;

    int32_t     R = 0;
    int32_t     G = 0;
    int32_t     B = 0;

    fp = fopen(path, "wb");

    fprintf(fp, "P3\n");
    fprintf(fp, "# Generate by YCIL\n");
    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "255\n");

    for (y = 0; y < img->height; y++) {
        for (x = 0; x < img->width; x++) {
            R = round(image_pixel(img, x, y)[IMG_CHANNEL_R]);
            G = round(image_pixel(img, x, y)[IMG_CHANNEL_G]);
            B = round(image_pixel(img, x, y)[IMG_CHANNEL_B]);
            if (R > 255) R = 255;
            if (G > 255) G = 255;
            if (B > 255) B = 255;
            if (R < 0) R = 0;
            if (G < 0) G = 0;
            if (B < 0) B = 0;

            fprintf(fp, "%d %d %d\n", R, G, B);
        }
    }

    return 0;
}
