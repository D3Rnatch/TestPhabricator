# Import some libs
import picamera
import picamera.array
import time
import numpy as np
import io

# Define our class
# Fields :
#   self.camera : camera object to use
class picture(object):

    # Constructor
    def __init__():
        self.camera = picamera.PiCamera()
        self.camera.resolution = (1280, 720)
        self.camera.iso = 200
        time.sleep(2)
        self.camera.shutter_speed = self.camera.exposure_speed
        self.camera.exposure_mode = 'off'
        camera.awb_mode = 'off'
        camera.awb_gains = (3, 0)
    
    # Set values for the camera
    # iso : iso value
    # red : awb_gain for red 
    # blue : awb_gain for blue
    def set_configs(iso, red, blue):
        self.camera.iso = iso
        self.camera.awb_gains = (red, blue)

    # Take a picture and store it in the jpeg format where the script is run with the name given
    def take_picture(name):
        self.camera.capture(name + '.jpg')

    # Take a picture and stor it in a cv2 object
    def take_cv_picture():
        with picamera.array.PiRGBArray(self.camera) as stream:
            self.camera.capture(stream, format='bgr')
            return stream.array
