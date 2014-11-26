## @file Scanner.py
#  @brief Scanner laser module.
#  @author Loïc Dalloz
#  @version 1.0
#
import picamera
import picamera.array
import cv2
import numpy as np
import time

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
#
# Usages :
#   Calibration :
#       s = Scanner.Scanner()
#       s.set_resolution(640, 480)
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
        self.y = 0
        self.half = 0

    ## Set the camera resolution.
    #  @param self The object pointer.
    #  @param x The x resolution.
    #  @param y The y resolution.
    def set_resolution(self, x, y):
        self.x = x
        self.y = y
        self.half = round(y * 0.45)

    ## Start everything we need to capture photos.
    #  @param self The object pointer.
    def start_module(self):
        # Start the camera.
        self.camera = picamera.PiCamera()
        self.camera.resolution = (self.x, self.y)
        self.camera.awb_gains = (4, 2)

        # Init mask threshold.
        self.lower_red = np.array([0, 0, 120])
        self.upper_red = np.array([255, 255, 255])

    ##  Take a picture.
    #   @param self The object pointer.
    def take_picture(self):
        stream = picamera.array.PiRGBArray(self.camera)
        self.camera.capture(stream, format='bgr', use_video_port=True)
        self.image = stream.array

    ## build the mask.
    #  @param self The object pointer.
    def make_mask(self):
        self.mask = cv2.inRange(self.image, self.lower_red, self.upper_red)

    ## Get U (position of the laser in image)
    #  @param self The object pointer
    #  @return The position of the laser (in pixel)
    #  Find the position of the laser on the self.half row of the image.
    def get_U(self):
        rows, col = self.mask.shape
        compteur = 0
        pos = 0
        u = 0
        for j in xrange(col):
            k = self.mask.item(self.half, j)
            if k == 255:
                compteur = compteur + 1
                pos = pos + j
        if compteur != 0:
            u = pos/compteur
        return u

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
        raw_input("Bring the target to 150cm of the camera then press ENTER...")
        print "Calibrating 150cm..."
        self.take_picture()
        self.make_mask()
        u2 = self.get_U()
        print "computing coefs..."
        self.d = 1500 - 500
        self.k = (u2*1.4)*1.5 - (u1*1.4)*0.5
        self.N = ((u1*1.4) - (u2*1.4))*1.5*500
        print "\td -> ", self.d
        print "\tk -> ", self.k
        print "\tN -> ", self.N
        print "Saving..."
        self.save_configs()

    ## Get the distance (in cm)
    #  @param self The object pointer.
    #  @return The computed distance in the plan (in cm).
    #  The distance is calculated with the coeffs stored in the file scan_config.txt.
    def get_plan_distance(self):
        print "start loop"
        temp = time.clock()
        total = temp
        self.take_picture()
        temp2 = time.clock()
        print "Take picture = ", temp2 - temp
        self.make_mask()
        temp = time.clock()
        print "Make mask = ", temp - temp2
        u = self.get_U()
        temp2 = time.clock()
        print "GetU = ", temp2 - temp
        dist = (self.N/((u*1.4)-self.k))/10
        temp = time.clock()
        print "Copute distance = ", temp - temp2
        print "Total = ", temp - total
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
	self.half = round(self.y * 0.45)
        file.close()
	print ""
	print "Loaded scanner values :"
	print "\tx -> " + str(self.x)
	print "\ty -> " + str(self.y)
	print "\td -> " + str(self.d)
	print "\tk -> " + str(self.k)
	print "\tN -> " + str(self.N)
	print "\thalf = " + str(round(self.y * 0.45))

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







