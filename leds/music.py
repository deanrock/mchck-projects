#!/usr/bin/env python

# 8 bar Audio equaliser using MCP2307
 
import alsaaudio as aa

from time import sleep
from struct import unpack
import numpy as np
import serial

ser = serial.Serial(port='/dev/ttyACM1',baudrate=315200)

def refresh():
    for p in pixels:
        ser.write(chr(p.r))
        ser.write(chr(p.g))
        ser.write(chr(p.b))
            
class Color:
    def __init__(self, r=0,g=0,b=0):
        self.r = r
        self.g = g
        self.b = b

pixels = [Color() for i in range(25)]

# Set up audio
sample_rate = 44100
no_channels = 2
chunk = 8*8*8 # Use a multiple of 8
data_in = aa.PCM(aa.PCM_CAPTURE, aa.PCM_NORMAL)
data_in.setchannels(no_channels)
data_in.setrate(sample_rate)
data_in.setformat(aa.PCM_FORMAT_S16_LE)
data_in.setperiodsize(chunk)

def calculate_levels(data, chunk,sample_rate):
    # Convert raw data to numpy array
    data = unpack("%dh"%(len(data)/2),data)
    data = np.array(data, dtype='h')
    # Apply FFT - real data so rfft used
    fourier=np.fft.rfft(data)
    # Remove last element in array to make it the same size as chunk
    fourier=np.delete(fourier,len(fourier)-1)
    # Find amplitude
    power = np.log10(np.abs(fourier))**2
    # Araange array into 8 rows for the 8 bars on LED matrix
    power = np.reshape(power,(8,chunk/8))
    matrix= np.int_(np.average(power,axis=1)/4)
    return matrix

print "Processing....."

def px(i,j,c):
    while i < j:
        i=i+1
        #pixels[i] = c

        colors = [Color(10,10,10), Color(50,50,0), Color(50,0,50), Color(100,0,100), Color(100,100,0),Color(180,150,40), Color(190,200,0), Color(255,0,0)]
        pixels[i] = colors[c.r]

while True:
    # Read data from device    
    l,data = data_in.read()
    data_in.pause(1) # Pause capture whilst RPi processes data
    if l:
        # catch frame error
        #try:
        if 1==1:
            matrix=calculate_levels(data, chunk,sample_rate)
            for i in range (0,8):

                level = (1<<matrix[i])-1 #0-7
                print level
                #x = level*10

                #if x <= 20:
                #    x = 20

                #if x > 255:
                #    x = 255

                x=int(level/1.3)
                if x > 7:
                    x = 7

                if x == 1:
                    x= 0

                if i == 0:
                    px(0,3, Color(x,x,x))
                elif i == 1:
                    px(4,7, Color(x,x,x))
                elif i == 2:
                    px(8,11, Color(x,x,x))
                elif i == 3:
                    px(12,15, Color(x,x,x))
                elif i == 4:
                    px(15,18, Color(x,x,x))
                elif i == 5:
                    px(19,22, Color(x,x,x))
                else:
                    px(23,24, Color(x,x,x))

                #Set_Column((1<<matrix[i])-1,0xFF^(1<<i))
            refresh()

        #except:
        #    print "h"
        #    pass

    sleep(0.001)
    data_in.pause(0) # Resume capture