import wave

print "Input File Name"
filename = raw_input()

filename += ".wav"

try:
    fp = wave.open(filename,"rb")
except Exception as err:
        print(err)

param = fp.getparams()

print "WAV param"
print "channels = %d, sampwidth = %d, framerate = %d, nframes = %d, compname = %s" % ( param.nchennels, param.sampwidth, param.framerate , param.nframes , param.compname )