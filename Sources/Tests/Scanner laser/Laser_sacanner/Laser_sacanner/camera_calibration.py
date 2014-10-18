# Import libs
import picture
import cv2
import numpy as np

# Startup the camera
print 'Startup the camera...\n'
camera = picture.picture()

# Take the first picture
print 'Take the first picture'
camera.take_picture('First')

# Loop th the three movable parameters
print 'Start the loop !!!\n'
for iso in range(1, 10):
    for red in range(0, 8):
        for blue in range(0, 8):
            print 'Iso = ' + iso + ' red = ' + red + ' blue = ' + blue
            camera.set_configs(iso, red, blue)
            camera.take_picture('iso' + iso + '_red' + red + '_blue' + blue)

# Test the cv2 picture
image = camera.take_cv_picture()
image = image[:, :, ::-1]
cv2.imwrite('cv2_test.jpg', image)

# End the script
print 'Script finished !\n'