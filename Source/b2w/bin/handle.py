import os

for i in xrange(45):
	cmd = ("ffmpeg -i %02d.bmp -vf pad=160:280:20:20:white %02d_pad.bmp") % (i,i)
	print cmd
	os.system(cmd)
