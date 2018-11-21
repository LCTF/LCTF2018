# Arch

0. Getting fps and length by calling the ffmpeg program
1. Transform video to BMP frame files by calling the ffmpeg program
2. Read one BMP file to image struct
3. Thresholding
4. Edge Detection by convolution and transform to absolute value
5. Thresholding
6. Select starting point (random for the first, and nearest for privies frame)
7. Get Route Array by doing graph traversal using depth first search
8. Adjust Route Array to fixed length using cubic Hermite interpolator
9. Loop from 2 to 8 to generate all Route Arrays for every frame
10. Merge Route Arrays to one Array
11. transform Route Array to wav file