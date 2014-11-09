# import some libs
import picamera
import picamera.array
import numpy as np
import cv2
import time

# Start the camera
camera = picamera.PiCamera()
camera.resolution = (640, 480)
time.sleep(2)
print 'Camera ready.'
camera.awb_gains = (4, 2)

while 1:
    # Capture an image
    stream = picamera.array.PiRGBArray(camera)
    camera.capture(stream, format='bgr', use_video_port=True)
    image = stream.array

    # Get the Threshold
    lower_red = np.array([0, 0, 90])
    upper_red = np.array([255, 255, 255])
    b, g, r = cv2.split(image)
    mask = cv2.inRange(image, lower_red, upper_red)

    # Clean the mask
    rows, col = mask.shape
    #for i in xrange(rows):
    compteur = 0
    pos = 0
    for j in xrange(col):
        k = mask.item(216, j)
        if k==255:
            compteur = compteur + 1
            pos = pos + j
            # mask.itemset((1000, j), 0)

    if compteur != 0:
        moy = pos/compteur
        #mask.itemset((1000, moy), 255)
        # Calcul de la position
        distance = ((166950)/((moy*1.4)-219.1))/10
        pos2 = moy
    else:
	distance = 0

    # show the position
    print str(distance)


