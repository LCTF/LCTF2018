/********************************************************************************
The MIT License
Copyright (c) 2018 Yeonji
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
********************************************************************************/

#include <path.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <image.h>
#include <bmp.h>

extern inline float * image_pixel(image_t * img, int32_t x, int32_t y);

int gen_path(image_t * img, wave_t * wav, int frame_no)
{
	float * point;

	static int graph_size = 0;

	/* store point on origin graph */
	/* using static to avoid too much malloc */
	static int * origin_x_pos = NULL;
	static int * origin_y_pos = NULL;
	static int * origin_dist  = NULL; /* store distence from last point */
	static int * origin_avail = NULL; /* judge if is avaliable */

	/* origin point count */
	int point_cnt;

	/* scan point */
	int point_index;

	/* avail point */
	int point_avail;

	float point_distant;
	
	/* store sorted path */
	static int * sorted_x = NULL;
	static int * sorted_y = NULL;
	int sorted_length = 0;

	static int * wave_x = NULL;
	static int * wave_y = NULL;
	int wav_length = (48000 / 24);
	int wav_index;
	int wav_bias;

	float adjust_rate;
	float zoom_rate;

	/* store the last point */
	/* using static to make frames continuity */
	static int last_x = 0;
	static int last_y = 0;

	int dx,dy;

	int dist_min;
	int dist_min_index;

	/* scan graph */
	int scan_x;
	int scan_y;


	/* initlize some static variable */
	if (graph_size == 0) {
		graph_size = img->width * img->height;

		origin_x_pos = malloc(graph_size * sizeof(int));
		origin_y_pos = malloc(graph_size * sizeof(int));
		origin_dist  = malloc(graph_size * sizeof(int));
		origin_avail = malloc(graph_size * sizeof(int));
		
		sorted_x = malloc(graph_size * sizeof(int));
		sorted_y = malloc(graph_size * sizeof(int));

		wave_x = malloc(wav_length * sizeof(int));
		wave_y = malloc(wav_length * sizeof(int));
	}

	/* reset avail points */
	for (point_cnt = graph_size - 1; point_cnt >= 0; point_cnt --) {
		origin_avail[point_cnt] = 0;
	}

	point_cnt = 0;

	/* find all points */
	for (scan_x = 1; scan_x < img->width; scan_x++) {
		for (scan_y = 1; scan_y < img->height; scan_y++) {
			point = image_pixel(img, scan_x, scan_y);
			if (*point > 128){
				origin_x_pos[point_cnt] = scan_x;
				origin_y_pos[point_cnt] = scan_y;
				origin_dist[point_cnt]  = 65536;
				origin_avail[point_cnt] = 1;
				point_cnt ++;

			}
		}
	}

	/* parse every point */
	for (point_avail = point_cnt; point_avail > 0; point_avail--) {
		
		/* caculate distance of every available point */
		for (point_index = 0; point_index < point_cnt; point_index++) {
			
			if ( origin_avail[point_index] == 1 ) {

				dx = abs((origin_x_pos[point_index] - last_x));
				dy = abs((origin_y_pos[point_index] - last_y));

				point_distant = sqrt((float)dx * (float)dx + (float)dy * (float)dy);

				origin_dist[point_index] = (int)point_distant;
			}
		}

		/* find the nearist point */

		dist_min = img->width + img->height;
		dist_min_index = 0;

		for (point_index = 0; point_index < point_cnt; point_index++) {
			
			if ( origin_avail[point_index] == 1 ) {

				if ( origin_dist[point_index] < dist_min ) {
					dist_min = origin_dist[point_index];
					dist_min_index = point_index;
				}

			}
		}

		/* add to sorted array and remove from available */
		sorted_x[sorted_length] = origin_x_pos[dist_min_index];
		sorted_y[sorted_length] = origin_y_pos[dist_min_index];

		origin_avail[dist_min_index] = 0;

		last_x = origin_x_pos[dist_min_index];
		last_y = origin_y_pos[dist_min_index];

		sorted_length ++;

	}


	adjust_rate = (float)sorted_length / (float)wav_length;
	zoom_rate = 65535 / (float)(img->width);

	wav_bias = frame_no * (48000 / 24);

	for (wav_index = 0; wav_index < wav_length; wav_index++) {
		point_index = wav_index * adjust_rate;
		wav->data[0][wav_bias] = zoom_rate * (sorted_x[point_index] - (img->width)/2);
		wav->data[1][wav_bias] = zoom_rate * (sorted_y[point_index] - (img->height)/2);

		wav_bias ++;
	}

	return 0;
}

int __attribute__((weak)) main(void)
{
	image_t * img;
	wave_t  * wav;
	int frame;

	img = bmp_read("./tmp/frames_proc/v-00530.bmp");

	wav = wave_new(2, 48000, (48000 / 24) * 20);

	for (frame = 0; frame < 20; frame++) {
		printf("Frame_no: %d\n",frame);
		gen_path(img, wav, frame);
	}

	wave_save(wav, "./out.wav");

	return 0;
}
