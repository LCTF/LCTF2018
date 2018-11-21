DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

CC = gcc
#CFLAGS = -pipe -Wall -Wextra -std=c99 -Wcomment -I${DIR_SRC} -D_POSIX_C_SOURCE=200809L
#CFINFLAGS = -pipe -Wall -Wextra -lm -std=c99 -Wcomment -I${DIR_SRC} -D_POSIX_C_SOURCE=200809L

LD_LIBRARY = -lm
CFLAGS = -pipe -s -Wall -Wextra -std=c99 -Wcomment -I${DIR_SRC} -D_POSIX_C_SOURCE=200809L
CFINFLAGS = -pipe -s -Wall -Wextra -std=c99 -Wcomment -I${DIR_SRC} -D_POSIX_C_SOURCE=200809L

TARGET = b2w

BIN_TARGET = ${DIR_BIN}/${TARGET}

${BIN_TARGET}:${OBJ}
	$(CC) $(CFINFLAGS) $(OBJ) -o $@ $(LD_LIBRARY)

${DIR_OBJ}/%.o:${DIR_SRC}/%.c ${DIR_OBJ}
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_OBJ}:
	@mkdir -p $@

${DIR_BIN}:
	@mkdir -p $@

prof.txt:
	@pprof --text ./bin/v2w ./test.prof > ./prof.txt

out.mp4:
	@echo "Compose Video."
	@ffmpeg -loglevel 0 -stats -r 24 -i ./tmp/frames_proc/v-%05d.bmp -c:v libx264 -vf fps=24 -pix_fmt yuv420p ./out_tmp.mp4
	@echo "Integrate Audio."
	@ffmpeg -loglevel 0 -stats -i ./out_tmp.mp4 -i ./tmp/Sample.aac -acodec copy -vcodec copy out.mp4
	rm out_tmp.mp4

.PHONY:clean run

clean:
	rm -f ${DIR_OBJ}/*.o
	rm -rf ./tmp
	rm -f out.mp4
	rm -f ./tmp/Sample.aac
	rm -f ${DIR_BIN}/${TARGET}

run:
	@${BIN_TARGET}
