# =========================================================================
# 			THIS IS A TEST PROTOCOL PROGRAM
# Its goal is to test the network modules and verify the data integrity.
# (On Raspberry reception side)
# =========================================================================

import time
import os
import sys
from network_manager import *

# init serial connection
try :
	net = Network_Manager("/dev/ttyACM0",9600)
except :
	print 'Please connect an arduino.'


# We wait for Autopilot to complete its loading...
while(not end) :
	hand = net.read()
	if hand.pop(0) == '0' :
		end = 1
	else :
		print 'Waiting for Autopilot check Hands'

print 'Autopilot checked.'
raw_input('Please Press Enter to start test program. Press q to quit')
q = 0
while(not q) :
	# Sending start frame and manual state
	q = raw_input(' Press Enter to send Start and State Frame')
	frame = net.create_data_frame(0,0,1,0,0)
	net.write(str(frame))
	if q != 'q' :
		q = 0

	# Sending move frame
	q = raw_input(' Press Enter to send move Frame')
	frame = net.create_data_frame(2,50,51,50,50)
	net.write(str(frame))
	if q != 'q' :
		q = 0
	

