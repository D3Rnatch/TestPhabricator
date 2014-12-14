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
	exit()


# We wait for Autopilot to complete its loading...
end = 0
while(not end) :
	hand = net.read()
	# if hand.pop(0) == '0' :
	# 	end = 1
	# else :
	print 'Waiting for Autopilot check Hands'
	print str(hand)
	print 'Erf : ' + str(hand[0])
	if hand[0] == 0 :
		end = 1
		print 'It is a check hand status'
	

print 'Autopilot checked.'
raw_input('Please Press Enter to start test program. Press q to quit')
q = 0
while(not q) :
	# Sending start frame and manual state
	q = raw_input(' Press Enter to send Start and State Frame')
	# frame = net.create_data_frame(str(0),str(0),str(1),str(0),str(0),str(0))
	frame = net.create_packed_data_frame(chr(0),chr(0),chr(1),chr(0),chr(0),chr(0))
	net.send(str(frame))
	if q != 'q' :
		q = 0

	# Read Output line
	# For Debug, seriously it is awesome...
	# print 'New raw data : ' + str(net.read_line())

	# Trying to get response :
	frame = net.read_unpack()
	print 'New raw data : ' + str(frame)

	# Sending move frame
	# q = raw_input(' Press Enter to send move Frame')
	# frame = net.create_data_frame(str(0),str(0),str(1),str(0),str(0),str(0))
	# frame = net.create_packed_data_frame(chr(2),chr(50),chr(51),chr(50),chr(50),chr(30))
	# net.send(str(frame))
	# if q != 'q' :
	# 	q = 0

	# Read Output line
	# For Debug, seriously it is awesome...
	# print 'New raw data : ' + str(net.read_line())


	# Trying to get response :
	# frame = net.read_unpack()
	# print 'New raw data : ' + str(frame)	

