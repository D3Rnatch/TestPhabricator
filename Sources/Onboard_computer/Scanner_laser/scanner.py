# Import some libs
import picamera
import picamera.array
import cv2
import numpy as np

class Scanner:
    # Create the class
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
    def __init__(self):
        self.x = 0
        self.y = 0
        self.half = 0

    # Set the camera resolution
    def set_resolution(self, x, y):
        self.x = x
        self.y = y
        self.half = round(y * 0.45)

    # Start everything we need to capture photos
    def start_module(self):
        # Start the camera
        self.camera = picamera.PiCamera()
        self.camera.resolution = (self.x, self.y)
        self.camera.awb_gains = (4, 2)

        # Init mask threshold
        self.lower_red = np.array([0, 0, 90])
        self.upper_red = np.array([255, 255, 255])

    # Get the picture
    def take_picture(self):
        stream = picamera.array.PiRGBArray(self.camera)
        self.camera.capture(stream, format='bgr', use_video_port=True)
        self.image = stream.array

    # build the mask
    def make_mask(self):
        self.mask = cv2.inRange(self.image, self.lower_red, self.upper_red)

    # Get U (position in image)
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
        print str(u) + '\n'
        return u

    # Calibrate the scanner
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
        self.d = 150 - 500
        self.k = (u2*1.4)*1.5 - (u1*1.4)*0.5
        self.N = ((u1*1.4) - (u2*1.4))*1.5*500
        print "\td -> ", self.d
        print "\tk -> ", self.k
        print "\tN -> ", self.N
        print "Saving..."
        self.save_configs()

    # Get the distance (in cm)
    def get_plan_distance(self):
        self.take_picture()
        self.make_mask()
        u = self.get_U()
        dist = ((self.N)/((u*1.4)-self.k))/10
        return dist

    # Load the calibration coefs
    def load_configs(self):
        file = open("scan_config.txt", "r")
        self.x = int(file.readline())
        self.y = int(file.readline())
        self.d = float(file.readline())
        self.k = float(file.readline())
        self.N = float(file.readline())
        file.close()

    # Save calibration coefs
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







