#include some libs
import picamera
import picamera.array
import cv2
import numpy as np
import time

#start the camera
print 'Start Camera...'
camera = picamera.PiCamera()
camera.resolution = (640, 480)
time.sleep(2)
camera.awb_gains = (4, 2)
print 'Camera OK.'

#Take the first photo
raw_input('Take the target to 50cm...')
stream = picamera.array.PiRGBArray(camera)
camera.capture(stream, format='bgr', use_video_port=True)
image = stream.array
b, g, r = cv2.split(image)
lower_red = np.array([0, 0, 90])
upper_red = np.array([255, 255, 255])
mask = cv2.inRange(image, lower_red, upper_red)
rows, col = mask.shape
cv2.imwrite('mask.jpg', mask)
compteur = 0
pos = 0
u1 = 0
u2 = 0
for j in xrange(col):
	k = mask.item(216, j)
	if k == 255:
		compteur = compteur + 1
		pos = pos + j
if compteur != 0:
	u1 = pos/compteur
	print 'U1 = ' + str(u1)
else:
	print 'Error in capture 1'

#Take the second photo
raw_input('Take the target to 150cm...')
stream = picamera.array.PiRGBArray(camera)
camera.capture(stream, format='bgr', use_video_port=True)
image = stream.array
b, g, r = cv2.split(image)
mask = cv2.inRange(image, lower_red, upper_red)
rows, col = mask.shape
compteur = 0
pos = 0
for j in xrange(col):
	k = mask.item(216, j)
	if k == 255:
		compteur = compteur + 1
		pos = pos + j
if compteur != 0:
	u2 = pos/compteur
	print 'U2 = ' + str(u2)
else:
	print 'Error in capture 1'

# Calibrate !
d = 1500 - 500
k = (u2*1.4)*1.500 - (u1*1.4)*0.5
N = ((u1*1.4) - (u2*1.4))*1.5*500

# show calibration result
print "d = " + str(d)
print "k = " + str(k)
print "N = " + str(N)

