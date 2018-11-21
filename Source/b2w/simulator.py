import wave
import math
import numpy as np
import matplotlib.pyplot as pl
from matplotlib import animation
import time

print "Input File Name"
#filename = raw_input()
filepath = "bin/"
filename = filepath+"out"
filename += ".wav"

try:
    fp = wave.open(filename,"rb")
except Exception as err:
        print(err)
        exit(0)

param = fp.getparams()
nchannels,sampwidth,framerate,nframes = param[:4]
print "WAV param"

print "channels = %d, sampwidth = %d, framerate = %d, nframes = %d, compname = %s" % ( nchannels,sampwidth,framerate,nframes,param[4] )

fp_data = fp.readframes(nframes)

fp.close()

wave_data = np.fromstring(fp_data,dtype=np.short)
wave_data.shape = -1, 2

wave_data = wave_data.T
frame = len(wave_data[0])/53

time = np.arange(0, nframes) * (1.0 / framerate)
n = 0

fig = pl.figure()
pic = fig.add_subplot(111)
line, = pic.plot(wave_data[1][n*frame:(n+1)*frame], wave_data[0][n*frame:(n+1)*frame],marker = ".")
line.set_marker(".")
def animate(i):
    line.set_ydata(wave_data[0][i*frame:(i+1)*frame])
    line.set_xdata(wave_data[1][i*frame:(i+1)*frame])
    return line,
def init():
    pic.set_xlim(-30000,30000)
    pic.set_ylim(-30000,30000)
    


ani = animation.FuncAnimation(fig=fig, func=animate, init_func = init ,frames=100)
pl.show()
#time.sleep(1)
