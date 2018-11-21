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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <bmp.h>



/**************************************************

    BMP Basic Structs

**************************************************/

/* BMP Header Struct */
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
}__attribute__((packed)) bmp_header_t;

/* Magic Number in BMP Header (bfType) */
#define WINDOWS_BMP         0X4D42  /*BM*/
#define OS2_BITMAP_ARRAY    0X4142  /*BA*/
#define OS2_COLOR_ICON      0X4943  /*CI*/
#define OS2_COLOR_POINTER   0X5043  /*CP*/
#define OS2_ICON            0X4349  /*IC*/
#define OS2_POINTER         0X5450  /*PT*/

/* DIB Header Struct */
typedef struct {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
}bmp_info_t;

#define DIB_BITMAPCOREHEADER_SIZE   12
#define DIB_OS22XBITMAPHEADER_SIZE  16
#define DIB_BITMAPINFOHEADER_SIZE   40
#define DIB_BITMAPV2INFOHEADER_SIZE 52
#define DIB_BITMAPV3INFOHEADER_SIZE 56
#define DIB_OS22XBITMAPHEADER2_SIZE 64
#define DIB_BITMAPV4HEADER_SIZE     108
#define DIB_BITMAPV5HEADER_SIZE     124

#define COMPRESS_BI_RGB             0
#define COMPRESS_BI_RLE8            1
#define COMPRESS_BI_RLE4            2
#define COMPRESS_BI_BITFIELDS       3
#define COMPRESS_BI_JPEG            4
#define COMPRESS_BI_PNG             5
#define COMPRESS_BI_ALPHABITFIELDS  6
#define COMPRESS_BI_CMYK            11
#define COMPRESS_BI_CMYKRLE8        12
#define COMPRESS_BI_CMYKRLE4        13

#define CHANNEL_R 0
#define CHANNEL_G 1
#define CHANNEL_B 2
#define CHANNEL_A 3

#define CHANNEL_CNT 4
#define CHANNEL_ALL 4



/**************************************************

    BMP Strings

**************************************************/

static const char * string_unknown           = "Unknown";
static const char * string_windows_bmp       = \
 "Windows 3.1x bitmap (424D)";
static const char * string_os2_bitmap_array  = \
 "OS/2 struct bitmap array (4241)";
static const char * string_os2_color_icon    = \
 "OS/2 struct color icon (4349)";
static const char * string_os2_color_pointer = \
 "OS/2 const color pointer (4350)";
static const char * string_os2_icon          = \
 "OS/2 struct icon (4943)";
static const char * string_os2_pointer       = \
 "OS/2 pointer (5054)";


static const char * string_bitmapcoreheader   = \
 "BMP core header";
static const char * string_os22xbitmapheader  = \
 "OS/2 BMP header";
static const char * string_bitmapinfoheader   = \
 "Standard BMP header";
static const char * string_bitmapv2infoheader = \
 "Adobe Photoshop Externed BMP header ver.2";
static const char * string_bitmapv3infoheader = \
 "Adobe Photoshop Externed BMP header ver.3";
static const char * string_os22xbitmapheader2 = \
 "OS/2 BMP header ver.2";
static const char * string_bitmapv4header     = \
 "Standard BMP header ver.4";
static const char * string_bitmapv5header     = \
 "Standard BMP header ver.5";


static const char * string_compress_bi_rgb             = \
 "None";
static const char * string_compress_bi_rle8            = \
 "RLE 8-bit/pixel";
static const char * string_compress_bi_rle4            = \
 "RLE 4-bit/pixel";
static const char * string_compress_bi_bitfields       = \
 "RGBA (Perhaps Huffman 1D)";
static const char * string_compress_bi_jpeg            = \
 "JPEG image for printing";
static const char * string_compress_bi_png             = \
 "PNG image for printing";
static const char * string_compress_bi_alphabitfields  = \
 "RGBA bit field masks on Windows CE 5.0 with .NET 4.0 or later";
static const char * string_compress_bi_cmyk            = \
 "Windows Metafile CMYK";
static const char * string_compress_bi_cmykrle8        = \
 "Windows Metafile CMYK with RLE 8-bit/pixel";
static const char * string_compress_bi_cmykrle4        = \
 "Windows Metafile CMYK with RLE 4-bit/pixel";


const char * get_bmp_type_string(const int16_t type_word)
{
    switch(type_word)
    {
        case WINDOWS_BMP       : return string_windows_bmp       ; break;
        case OS2_BITMAP_ARRAY  : return string_os2_bitmap_array  ; break;
        case OS2_COLOR_ICON    : return string_os2_color_icon    ; break;
        case OS2_COLOR_POINTER : return string_os2_color_pointer ; break;
        case OS2_ICON          : return string_os2_icon          ; break;
        case OS2_POINTER       : return string_os2_pointer       ; break;
        default                : return string_unknown;
    };
    return string_unknown;
}

const char * get_bmp_dib_string(uint32_t size)
{
    switch(size)
    {
        case DIB_BITMAPCOREHEADER_SIZE   :
            return string_bitmapcoreheader   ; break;
        
        case DIB_OS22XBITMAPHEADER_SIZE  :
            return string_os22xbitmapheader  ; break;
        
        case DIB_BITMAPINFOHEADER_SIZE   :
            return string_bitmapinfoheader   ; break;
        
        case DIB_BITMAPV2INFOHEADER_SIZE :
            return string_bitmapv2infoheader ; break;
        
        case DIB_BITMAPV3INFOHEADER_SIZE :
            return string_bitmapv3infoheader ; break;
        
        case DIB_OS22XBITMAPHEADER2_SIZE :
            return string_os22xbitmapheader2 ; break;
        
        case DIB_BITMAPV4HEADER_SIZE     :
            return string_bitmapv4header     ; break;
        
        case DIB_BITMAPV5HEADER_SIZE     :
            return string_bitmapv5header     ; break;
        
        default:
            return string_unknown;
    }

    return string_unknown;
}

const char * get_bmp_comp_string(uint32_t method){
    switch(method)
    {
        case COMPRESS_BI_RGB            :
            return string_compress_bi_rgb            ; break;
        
        case COMPRESS_BI_RLE8           :
            return string_compress_bi_rle8           ; break;
        
        case COMPRESS_BI_RLE4           :
            return string_compress_bi_rle4           ; break;
        
        case COMPRESS_BI_BITFIELDS      :
            return string_compress_bi_bitfields      ; break;
        
        case COMPRESS_BI_JPEG           :
            return string_compress_bi_jpeg           ; break;
        
        case COMPRESS_BI_PNG            :
            return string_compress_bi_png            ; break;
        
        case COMPRESS_BI_ALPHABITFIELDS :
            return string_compress_bi_alphabitfields ; break;
        
        case COMPRESS_BI_CMYK           :
            return string_compress_bi_cmyk           ; break;
        
        case COMPRESS_BI_CMYKRLE8       :
            return string_compress_bi_cmykrle8       ; break;
        
        case COMPRESS_BI_CMYKRLE4       :
            return string_compress_bi_cmykrle4       ; break;
        
        default:
            return string_unknown;
    }
    return string_unknown;
}



/**************************************************

    BMP File Basic Struct

**************************************************/

typedef struct{
    FILE           *fp;
    image_t        *img;

    bmp_header_t    header;
    bmp_info_t      info;
    uint8_t        *data;

    int32_t         data_offset;
    int32_t         data_size;
    int32_t         img_width;
    int32_t         img_height;
    int32_t         color_depth;
    int32_t         width_fixed;
} bmp_file_t;



/**************************************************

    BMP Data Encode && Decode

**************************************************/

int bmp_decode_data_24(bmp_file_t *bmp)
{
    int32_t line    = bmp->info.biHeight;
    int32_t column  = 0;

    uint8_t *src;

    for (line--; line >= 0; line--) {
        src = bmp->data + line * bmp->width_fixed;
        for(column = 0; column < bmp->img->width; column++) {
            image_pixel(bmp->img, column, (bmp->img->height - line - 1))[IMG_CHANNEL_B] = *src;
            src ++;
            image_pixel(bmp->img, column, (bmp->img->height - line - 1))[IMG_CHANNEL_G] = *src;
            src ++;
            image_pixel(bmp->img, column, (bmp->img->height - line - 1))[IMG_CHANNEL_R] = *src;
            src ++;
        }
    }

    return 0;
}

int bmp_encode_data_24(bmp_file_t *bmp)
{
    int32_t line    = bmp->img->height;
    int32_t column  = 0;

    int32_t s;
    uint8_t *dst;

    int32_t width;

    bmp->width_fixed = width = bmp->img->width * 3;

    if (bmp->width_fixed % 4 != 0) {
        bmp->width_fixed = (bmp->width_fixed - bmp->width_fixed % 4 + 4);
    }

    bmp->data_size = bmp->width_fixed * line * sizeof(uint8_t);

    bmp->data = malloc(bmp->data_size);

    dst = bmp->data;

    for (line = 0; line < bmp->img->height; line++) {
        for(column = 0; column < bmp->img->width; column++) {
            s = round(image_pixel(bmp->img, column, (bmp->img->height - line - 1))[IMG_CHANNEL_B]);
            if (s > 255) s = 255;
            if (s < 0) s = 0;
            *dst = s;
            dst ++;

            s = round(image_pixel(bmp->img, column, (bmp->img->height - line - 1))[IMG_CHANNEL_G]);
            if (s > 255) s = 255;
            if (s < 0) s = 0;
            *dst = s;
            dst ++;

            s = round(image_pixel(bmp->img, column, (bmp->img->height - line - 1))[IMG_CHANNEL_R]);
            if (s > 255) s = 255;
            if (s < 0) s = 0;
            *dst = s;
            dst ++;
        }

        column = column * 3;

        while (column < bmp->width_fixed) {
            *dst = 0;
            column ++;
            dst ++;
        }
    }

    return 0;
}

int bmp_encode_data(bmp_file_t *bmp)
{
    return bmp_encode_data_24(bmp);
}

int bmp_decode_data(bmp_file_t *bmp)
{
    return bmp_decode_data_24(bmp);
}


/**************************************************

    BMP Srtuct Read

**************************************************/

int bmp_read_header(bmp_file_t *bmp)
{
    /* Read Header */
    fseek(bmp->fp, 0, SEEK_SET);
    if(fread(&bmp->header, sizeof(bmp->header), 1, bmp->fp) != 1){
        fprintf(stderr, "Broken File: Can not read header.\n");
        return -1;
    }

    /* Check if Valid BMP file */
    if(bmp->header.bfType != WINDOWS_BMP){
        fprintf(stderr, "Unsupported Format: %s.\n", \
            get_bmp_type_string(bmp->header.bfType));
        return -1;
    }

    bmp->data_offset = bmp->header.bfOffBits;

    return 0;
}

int bmp_read_dib(bmp_file_t *bmp)
{
    /* Read DIB Info Chunk */
    fseek(bmp->fp, 14, SEEK_SET);

    /* get bmp information header */
    if( fread(&bmp->info, sizeof(bmp->info), 1, bmp->fp) != 1 ) {
        fprintf(stderr, "Cannot Read DIB info.\n");
        return -1;
    }

    /* unsupported bmp format */
    if(bmp->info.biSize < DIB_BITMAPINFOHEADER_SIZE){
        fprintf(stderr, "Unsupported DIB Format: %s.\n", \
                get_bmp_dib_string(bmp->info.biSize));
        return -1;
    }

    bmp->img_width   = bmp->info.biWidth;
    bmp->img_height  = bmp->info.biHeight;

    bmp->color_depth = bmp->info.biBitCount;

    bmp->width_fixed = (bmp->info.biWidth * bmp->color_depth) / 8;

    if (bmp->width_fixed % 4 != 0) {
        bmp->width_fixed = (bmp->width_fixed - bmp->width_fixed % 4 + 4);
    }

    bmp->data_size   = bmp->width_fixed * bmp->info.biHeight;

    return 0;
}

int bmp_read_data(bmp_file_t *bmp)
{
    /* locate file stream */
    if(fseek(bmp->fp, bmp->data_offset, SEEK_SET) != 0){
        return -1;
    }

    bmp->data = malloc((bmp->data_size) * sizeof(uint8_t));

    if(bmp->data == NULL){
        return -1;
    }

    /* read bmp data */
    if(fread(bmp->data, bmp->data_size, 1, bmp->fp) != 1){
        free(bmp->data);
        return -1;
    }

    return 0;
}



/**************************************************

    BMP Read/Write

**************************************************/


image_t * bmp_read(const char * path)
{
    FILE * fp;
    bmp_file_t      bmp;
    
    fp = fopen(path, "rb");
    if ( fp == NULL ) {
        perror("Cannot open file");
        return NULL;
    }

    bmp.fp = fp;

    if( bmp_read_header(&bmp) != 0) {
        fprintf(stderr, "Cannot read BMP header\n");
        return NULL;
    }

    if( bmp_read_dib(&bmp) != 0) {
        fprintf(stderr, "Cannot read BMP DIB\n");
        return NULL;
    }

    if( bmp_read_data(&bmp) != 0) {
        fprintf(stderr, "Cannot read BMP data\n");
        return NULL;
    }

    bmp.img = image_new(bmp.img_width, bmp.img_height, IMG_MODEL_BGR);

    if( bmp_decode_data(&bmp) != 0) {
        fprintf(stderr, "Cannot decode BMP data\n");
        return NULL;
    }

    fclose(fp);
    free(bmp.data);

    return bmp.img;
}

int bmp_save(image_t * img, const char * path)
{
    bmp_file_t      bmp;

    uint32_t  data_offset;
    uint32_t  file_size;

    if ( img == NULL ) return -1;

    memset(&bmp, 0, sizeof(bmp_file_t));

    /* open file */
    bmp.fp = fopen(path, "wb");
    if(bmp.fp == NULL){
        perror("Cannot open file for write.");
        return -1;
    }

    bmp.img = img;

    if(bmp_encode_data(&bmp) != 0) {
        fclose(bmp.fp);
        return -1;
    }

    data_offset = sizeof(bmp_header_t) + sizeof(bmp_info_t);
    file_size   = data_offset + bmp.data_size;

    bmp.header.bfType        = WINDOWS_BMP;
    bmp.header.bfSize        = file_size;
    bmp.header.bfOffBits     = data_offset;

    bmp.info.biSize          = DIB_BITMAPINFOHEADER_SIZE;
    bmp.info.biWidth         = bmp.img->width;
    bmp.info.biHeight        = bmp.img->height;
    bmp.info.biPlanes        = 1; /* This shoule always be 1 */
    bmp.info.biBitCount      = 24;
    bmp.info.biCompression   = COMPRESS_BI_RGB;
    bmp.info.biSizeImage     = bmp.data_size;
    bmp.info.biXPelsPerMeter = 72;
    bmp.info.biYPelsPerMeter = 72;
    bmp.info.biClrUsed       = 0;
    bmp.info.biClrImportant  = 0;

    fwrite(&bmp.header, sizeof(bmp_header_t), 1, bmp.fp);
    fwrite(&bmp.info, sizeof(bmp_info_t), 1, bmp.fp);
    fwrite(bmp.data, bmp.data_size, 1, bmp.fp);

    fclose(bmp.fp);

    free(bmp.data);

    return 0;
}
