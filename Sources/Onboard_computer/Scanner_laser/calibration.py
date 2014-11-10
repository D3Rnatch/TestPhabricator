# import the module
from scanner import Scanner

# Load the module
s = Scanner.Scanner()
s.set_resolution(640, 480)
s.start_module()
s.calibration()
exit()