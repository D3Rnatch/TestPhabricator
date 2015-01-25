import cv2
import numpy as np
import time
import math

cap = cv2.VideoCapture(0)

compteur = 20
max2 = 0

print "calibrate\n"

while compteur > 0:
    ret, image = cap.read()
    if ret:
        compteur = compteur - 1
        if compteur < 20:
            b, g, r = cv2.split(image)
            max = np.amax(r)
            if max > max2:
                max2 = max
            high_value = np.array([255, 255, 255])
            low_value = np.array([0, 0, max2])
            image2 = cv2.inRange(image, low_value, high_value)
            cv2.imshow('video', image)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

if max2 > 254:
    max2 = 254

print "go !\n"
while(1):
    ret, image = cap.read()
    if ret :
        high_value = np.array([255, 255, 255])
        low_value = np.array([0, 0, max2])
        image2 = cv2.inRange(image, low_value, high_value)
        rows, col = image2.shape
        liste = [j for j in xrange(col) if image2.item(int(cap.get(4)*0.63), j)==255]
        point = np.mean(liste)
        if not math.isnan(point):
            cv2.circle(image, (int(point), int(cap.get(4)*0.63)), 5, (0, 0, 255) ,2)
        cv2.line(image, (0, int(cap.get(4)*0.63)), (int(cap.get(3)), int(cap.get(4)*0.63)), (255, 0, 0), 1)

        cv2.imshow('video', image)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
