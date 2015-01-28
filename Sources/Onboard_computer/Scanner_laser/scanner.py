## @file Scanner.py
#  @brief Scanner laser module.
#  @author Loic Dalloz
#  @version 3.2
#
#import picamera
#import picamera.array
import cv2
import numpy as np
import time
import math

## Scanner laser module.
# Fields :
#   - self.x         => x resolution
#   - self.y         => y resolution
#   - self.half      => half of the image (where the distance is calculated in the plan
#   - self.camera    => THE camera !
#   - self.image     => Where the pictures are stored
#   - self.mask      => Where the mask is located
#   - self.upper_red => Max color (for the mask)
#   - self.lower_red => Min color (for the mask)
#   - self.d        |
#   - self.k        |=> Scanner calibration coefs
#   - self.N        |
#   - self.cap       => Opencvcapture object
#
# Usages :
#   Calibration :
#       s = Scanner.Scanner()
#       s.start_module()
#       s.calibration()
#       exit()
#
#   Scanning :
#       s = Scanner.Scanner()
#       s.load_configs()
#       s.start_module()
#       while 1:
#           print Str(s.get_plan_distance())
#       exit()
#
class Scanner:
    ## Constructor.
    #  @param self The object pointer.
    def __init__(self):
        self.x = 0
	self.compteur = 0
        self.y = 0
        self.half = 0
	self.kernel_erode = np.ones((1, 1), np.uint8)
	self.kernel_dilate = np.ones((6, 6), np.uint8)

    ## Start everything we need to capture photos.
    #  @param self The object pointer.
    def start_module(self):
        # Start the camera.
        self.cap = cv2.VideoCapture(0)
	ret = self.cap.set(3, 670)
	ret = self.cap.set(4, 480)
	ret = self.cap.set(11, 0.5)
        compteur = 30
        max2 = 0
        # Calibrate
        #while compteur > 0:
        #    ret, image = self.cap.read()
        #    if ret:
        #        compteur = compteur - 1
        #        if compteur > 10:
        #            b, g, r = cv2.split(image)
        #            max = np.amax(r)
        #            if max > max2:
        #                max2 = max

        # if max2 > 254:
        #    max2 = 254

        # Init mask threshold.
	max2 = 253
        self.lower_red = np.array([0, 0, max2])
        self.upper_red = np.array([255, 255, 255])

        # Init resolution
        self.x = int(self.cap.get(4))
        self.y = int(self.cap.get(3))
        self.half = round(self.y * 0.5)

    ## Properly close.
    #  @param self The object pointer.
    def close(self):
	self.cap.release()

    ## Take a picture.
    #  @param self The object pointer.
    def take_picture(self):
        ret, self.image = self.cap.read()
        while ret == False:
            ret, self.image = self.cap.read()
        ret, self.image = self.cap.read()
        while ret == False:
            ret, self.image = self.cap.read()
        ret, self.image = self.cap.read()
        while ret == False:
            ret, self.image = self.cap.read()
        ret, self.image = self.cap.read()
        while ret == False:
            ret, self.image = self.cap.read()
        ret, self.image = self.cap.read()
        while ret == False:
            ret, self.image = self.cap.read()

    ## build the mask.
    #  @param self The object pointer.
    def make_mask(self):
        mask_temp = cv2.inRange(self.image, self.lower_red, self.upper_red)
	mask_eroded = cv2.erode(mask_temp, self.kernel_erode)
	self.mask = cv2.dilate(mask_eroded, self.kernel_dilate, iterations=1)

    ## Get U (position of the laser in image)
    #  @param self The object pointer
    #  @return The position of the laser (in pixel)
    #  Find the position of the laser on the self.half row of the image.
    def get_U(self):
        #rows, col = self.mask.shape
        #liste = [j for j in xrange(col) if self.mask.item(self.half, j)==255]
        #return np.mean(liste)
	retour = 0
	mean = 0
	f_x = 0
	f_y = 0
	f_w = 0
	f_h = 0
	compteur = 0
	contours, hierarchy = cv2.findContours(self.mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
	return_value = 0
	for contour in contours:
		x, y, w, h = cv2.boundingRect(contour)
		#if (w < h + 3) and (w > h - 3) and w > 0 and h > 0 and y > 180 and y < 280 and x > 200:
		if y > 180 and y < 280 and x > 200:
			compteur = compteur + 1
			mean = mean + x
			mean = mean + x + w
			f_x = x
			f_y = y
			f_w = w
			f_h = h
			cv2.rectangle(self.image, (x, y), (x+w, y+h), (0, 255, 0), 2)
		else:
			cv2.rectangle(self.image, (x, y), (x+w, y+h), (255, 0, 0), 2)
	cv2.rectangle(self.image, (200, 180), (640, 280), (255, 255, 255), 1)
	cv2.putText(self.image, str(compteur), (50, 20), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 2)
	cv2.imwrite("capture" + str(self.compteur) + ".png", self.image)
	self.compteur = self.compteur + 1
	if compteur == 1:
		retour =  f_x + (f_w/2)
	elif compteur == 0:
		retour =  0
	else:
		retour = (mean/2)/compteur
	return retour
	

    ## Calibrate the scanner
    #  @param self The object pointer.
    #  Calibrate the scanner and store le calibration coeffs in a file "scan_config.txt"
    def calibration(self):
        print " "
        print " ________________________ "
        print "|                        |"
        print "|    Start calibration   |"
        print "|________________________|"
        print " "
        raw_input("Bring the target to 50cm of the camera then press ENTER...")
        print "Calibrating 50cm..."
        self.take_picture()
        self.make_mask()
        u1 = self.get_U()
	cv2.line(self.image, (u1, 0), (u1, int(self.x)), (255, 255, 255), 1) 
	cv2.imwrite("50cm.png", self.image)
	print "u = %s" % (str(u1))
        raw_input("Bring the target to 150cm of the camera then press ENTER...")
        print "Calibrating 150cm..."
        self.take_picture()
        self.make_mask()
        u2 = self.get_U()
	cv2.line(self.image, (u2, 0), (u2, int(self.x)), (255, 255, 255), 1) 
	cv2.imwrite("150cm.png", self.image)
	print "u = %s" % (str(u2))
        print "computing coefs..."
        self.d = 1500 - 500
        self.k = (u2*1.4)*1.5 - (u1*1.4)*0.5
        self.N = ((u1*1.4) - (u2*1.4))*1.5*500
        print "\td -> ", self.d
        print "\tk -> ", self.k
        print "\tN -> ", self.N
        print "Saving..."
        self.save_configs()
        cv2.destroyAllWindows()

    ## Get the distance (in cm)
    #  @param self The object pointer.
    #  @return The computed distance in the plan (in cm).
    #  The distance is calculated with the coeffs stored in the file scan_config.txt.
    def get_plan_distance(self):
        self.take_picture()
        self.make_mask()
        u = self.get_U()
	if u != 0:
        	dist = (self.N/((u*1.4)-self.k))/10
	else:
		dist = 0
        return dist

    ## Load the calibration coefs
    #  @param self The object pointer.
    def load_configs(self):
        file = open("scan_config.txt", "r")
        self.x = int(file.readline())
        self.y = int(file.readline())
        self.d = float(file.readline())
        self.k = float(file.readline())
        self.N = float(file.readline())
        file.close()
        print ""
        print "Loaded scanner values :"
        print "\td -> " + str(self.d)
        print "\tk -> " + str(self.k)
        print "\tN -> " + str(self.N)

    ## Save calibration coefs
    #  @param The object pointer.
    # File format :
    #   ________________
    #   |x resolution   |
    #   |y resolution   |
    #   |d              |
    #   |k              |
    #   |N              |
    #   |EOF            |
    #   |_______________|
    def save_configs(self):
        file = open("scan_config.txt", "w")
        file.write(str(self.x) + "\n")
        file.write(str(self.y) + "\n")
        file.write(str(self.d) + "\n")
        file.write(str(self.k) + "\n")
        file.write(str(self.N) + "\n")
        file.close()







