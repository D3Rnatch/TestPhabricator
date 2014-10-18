# Import some usefull libs
import cv2
import numpy as np

# *************************
# ***   Load the image  ***
# *************************
image_txt = input('Which image ? ')
image = cv2.imread(image_twt)

# *****************************
# ***   Show the base image ***
# *****************************
cv2.imshow('Base image', image)
cv2.waitKey(0)

# *****************************
# ***   Process the Image   ***
# *****************************
# Convert it in hsv mode
image_hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

# Find the color range
lower_red = np.array([150, 50, 50])
upper_red = np.array([170, 255, 255])
image_mask = cv2.inRange(image_hsv, lower_red, upper_red)

# *************************
# ***   Show the mask   ***
# *************************
cv2.imshow('Mask', image_mask)
cv2.waitKey(0)

# *********************************
# ***   Show the final result   ***
# *********************************
image_result = cv2.bitwise_and(image, image, mask= image_mask)
cv2.imshow('Result', image_result)
cv2.waitKey(0)
cv2.destroyAllWindows()
