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
#include <string.h>
#include <math.h>
#include <image_edge_dect.h>
#include <image.h>


int image_edge_dect(image_t * img)
{
	int x0,y0;

	float conv_tmpx,conv_tmpy,conv_tmp;
	float * img_tmp;

	static image_t * img_out = NULL;

    int Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int Gy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

	if (img_out == NULL) {	
		img_out = image_new(img->width, img->height, IMG_MODEL_BGR);
	}

	for(x0 = 1; x0 < img->width - 1; x0++){
        for(y0 = 1; y0 < img->height - 1; y0++){
            /* ergodic pixel in image */
            conv_tmpx = 0;
            conv_tmpy = 0;

            conv_tmpx += image_pixel(img, x0 - 1, y0 - 1)[0] * Gx[0][0];
            conv_tmpy += image_pixel(img, x0 - 1, y0 - 1)[0] * Gy[0][0];

            conv_tmpx += image_pixel(img, x0, y0 - 1)[0] * Gx[0][1];
            conv_tmpy += image_pixel(img, x0, y0 - 1)[0] * Gy[0][1];

            conv_tmpx += image_pixel(img, x0 + 1, y0 - 1)[0] * Gx[0][2];
            conv_tmpy += image_pixel(img, x0 + 1, y0 - 1)[0] * Gy[0][2];

            conv_tmpx += image_pixel(img, x0 - 1, y0)[0] * Gx[1][0];
            conv_tmpy += image_pixel(img, x0 - 1, y0)[0] * Gy[1][0];

            conv_tmpx += image_pixel(img, x0 + 1, y0)[0] * Gx[1][2];
            conv_tmpy += image_pixel(img, x0 + 1, y0)[0] * Gy[1][2];

            conv_tmpx += image_pixel(img, x0 - 1, y0 + 1)[0] * Gx[2][0];
            conv_tmpy += image_pixel(img, x0 - 1, y0 + 1)[0] * Gy[2][0];

            conv_tmpx += image_pixel(img, x0, y0 + 1)[0] * Gx[2][1];
            conv_tmpy += image_pixel(img, x0, y0 + 1)[0] * Gy[2][1];

            conv_tmpx += image_pixel(img, x0 + 1, y0 + 1)[0] * Gx[2][2];
            conv_tmpy += image_pixel(img, x0 + 1, y0 + 1)[0] * Gy[2][2];
            
            conv_tmp = (float)fabs(conv_tmpx) * (float)fabs(conv_tmpx) + (float)fabs(conv_tmpy) * (float)fabs(conv_tmpy);

            img_tmp  = image_pixel(img_out, x0, y0);

            img_tmp[0] = conv_tmp;
            img_tmp[1] = conv_tmp;
            img_tmp[2] = conv_tmp;
            
        }
    }

    memcpy(img->data, img_out->data, 3 * img->width * img->height * sizeof(float));

	return 0;
}
