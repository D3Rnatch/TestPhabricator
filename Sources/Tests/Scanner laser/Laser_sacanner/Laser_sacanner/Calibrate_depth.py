import picamera

with picamera.PiCamera() as camera:
    camera.resolution = (640, 480)
    camera.start_preview()
    camera.start_recording('foo.h264')
    ret = input("Enter to stop...")
    camera.stop_recording()
    camera.stop_preview()