import serial
from time import sleep
import cmath
import math

ser = serial.Serial(port='/dev/ttyACM0',baudrate=115200)
#
#ser.open()

class Color:
	def __init__(self, r=0,g=0,b=0):
		self.r = r
		self.g = g
		self.b = b

	def __repr__(self):
		return "%d %d %d" %(self.r,self.g,self.b)

pixels = [Color() for i in range(25)]

def refresh():
	for p in pixels:
		ser.write(chr(p.r))
		ser.write(chr(p.g))
		ser.write(chr(p.b))

def colorWipe(c, wait):
	for i in range(0,25):
		pixels[i] = c
		refresh()
		sleep(0.05)

def wheel(b):
	if b < 85:
		return Color(b*3, 255-b*3, 0)
	elif b < 170:
		b -= 85
		return Color(255-b*3, 0, b*3)
	else:
		b -= 170
		return Color(0, b*3, 255-b*3)
		

def myeffect():
	pass

def rainbow(wait):
	for j in range(256):
		for i in range(25):
			pixels[i] = wheel((i+j)%255)
		print pixels[24]
		refresh()
		sleep(0.05)

def rainbowCycle(wait):
	for j in range(256*5):
		for i in range(25):
			pixels[i] = wheel( ((i*256/25)+j)%256)

		refresh()
		sleep(0.05)


while 1:

	colorWipe(Color(255,0,0), 50)
	colorWipe(Color(0,255,0), 50)
	colorWipe(Color(0,0,255), 50)
	#rainbow(20)
	
	sleep(1)
	
