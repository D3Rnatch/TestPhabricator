# Import some lib
import cv2
import numpy as np

# Load base image
image = cv2.imread('200cm.jpg')
# cv2.imshow('base_image', image)
# cv2.waitKey(0)

# turn to hsv value
# hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
lower_red = np.array([0, 0, 100])
upper_red = np.array([255, 255, 255])

# get hue 
b, g, r = cv2.split(image)
# cv2.imshow('Red', r)
# cv2.imshow('Blue', b)
# cv2.imshow('Green', g)
# cv2.waitKey(0)

# build the mask
mask = cv2.inRange(image, lower_red, upper_red)
# ret, mask = cv2.threshold(h, 120, 255, cv2.THRESH_BINARY)
# mask = cv2.inRange(h, 120, 130, cv2.THRESH_BINARY)

# Show the mask
#cv2.imshow('mask', mask)
#cv2.waitKey(0)

# Try to get a better mask
rows, col = mask.shape
pos2 = 0
distance = 0
for i in xrange(rows):
    compteur = 0
    pos = 0
    for j in xrange(col):
        k = mask.item(i, j)
        if k==255:
            compteur = compteur + 1
            pos = pos + j
            mask.itemset((i, j), 0)

    if compteur != 0:
        moy = pos/compteur
        mask.itemset((i, moy), 255)
        if i == 1000:
            distance = ((1210000)/(((moy*1.4)/1000)*1500-1575))/10
            pos2 = moy

# Show the mask
# cv2.imshow('mask 2', mask)
# cv2.waitKey(0)

# Get the distance

#rows, col = mask.shape
#for i in xrange(col):
#    k = mask.item(10, i)
#    if k == 255:
#        distance = ((1210000)/(((i*1.4)/1000)*1500-1575))/10
        

# Edge mask
# edge_mask = cv2.Canny(image,5, 100)
# cv2.imshow('edges', edge_mask)
# cv2.waitKey(0)

# mix both mask
# mix_mask = cv2.bitwise_and(edge_mask, mask)
# cv2.imshow('mixed mask', mix_mask)
# cv2.waitKey(0)

# find lines
#lines = cv2.HoughLinesP(mask, 1, np.pi/180, 5, minLineLength= 10, maxLineGap= 5)
#for x1,y1,x2,y2 in lines[0]:
#    cv2.line(image,(x1,y1),(x2,y2),(0,255,0),2)
    
print str(pos)

cv2.circle(image, (pos2,1000), 10, (0, 255, 0), 3, cv2.CV_AA)

cv2.putText(image, str(distance) + 'cm', (pos2+15, 1000), cv2.FONT_HERSHEY_SIMPLEX, 4,(255,255,255),2, cv2.CV_AA)

# cv2.imwrite("Pos_300.jpg", image)
cv2.imshow('Lines 3', cv2.resize(image, (0, 0), fx=0.3, fy=0.3))

cv2.waitKey(0)
cv2.destroyAllWindows()
