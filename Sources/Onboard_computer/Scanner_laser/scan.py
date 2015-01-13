# import the module
from scanner import Scanner
import time

# Load the module
s = Scanner()
s.load_configs()
s.start_module()
# Scan
while 1:
    print str(s.get_plan_distance())
    inp = raw_input("Enter for continue...")

# Exit safely (never gets here... Too bad)
exit()
