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

#include <stdio.h>
#include <stdlib.h>

#define __USE_MISC 1
#include <dirent.h>

#include <utils.h>

int create_tmp_dir(void)
{
	fprintf(stderr, "Creating Temp Dir.\n");
	/* Windows command lines are different from others */

#if defined(WIN32) || defined(WIN64) || defined(WINNT)

	/* Add Windows command lines here */

#else /* ! WIN32 || WIN64 || WINNT */

	system("mkdir -p ./tmp");
	system("mkdir -p ./tmp/frames");
	system("mkdir -p ./tmp/frames_proc");

#endif /* WIN32 || WIN64 || WINNT */

	return 0;
}

int remove_tmp_dir(void)
{
	/* Windows command lines are different from others */

#if defined(WIN32) || defined(WIN64) || defined(WINNT)

	/* Add Windows command lines here */

#else /* ! WIN32 || WIN64 || WINNT */

	system("rm -rf ./tmp");

#endif /* WIN32 || WIN64 || WINNT */

	return 0;
}

int transform_video(const char * input_path)
{
	char commands[255];

	fprintf(stderr, "Extracting Frames.\n");

	sprintf(commands, "ffmpeg -loglevel 0 -stats -i %s -vf fps=24 ./tmp/frames/v-%%05d.bmp", input_path);

	system(commands);

	fprintf(stderr, "Extracting Audio Track.\n");

	sprintf(commands, "ffmpeg -loglevel 0 -stats -i %s -vn -ar 44100 -ac 2 -ab 192k -f adts ./tmp/Sample.aac", input_path);

	system(commands);

	return 0;
}

int get_frame_cnt(void)
{
	int frame_count = 0;
	DIR * dirp;
	struct dirent * entry;

	dirp = opendir("./tmp/frames/");
	
	while ((entry = readdir(dirp)) != NULL) {
	    if (entry->d_type == DT_REG) {
	         frame_count++;
	    }
	}

	closedir(dirp);

	return frame_count;
}
