# This file permlits the test in manual mode of the robot
# Not yet finished.

import time
import os
import sys
import socket

from network_manager import *

# init socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('localhost', 5000))

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

# Sending start frame and manual state
frame = net.create_data_frame(0,0,1,0,0)
net.write(str(frame))

# waiting the connection
s.listen(1)
while(1):
	(client, addr) = s.accept()
	run = True

	# Run the app
	while (run):
		data = client.recv(1024)
		if not data:
			run = False
	
		else: 
			datas = data.split(':')
	
			jj = int(datas[0])
			ii = int(datas[1])
			vvv = int(datas[2])
	
			#print those datas
			os.system('clear')
			print '--------------------------------------------------------------   ---'
			for i in range(0, 20):
				sys.stdout.write('-')
				for j in range(0, 20):
					if ((j == jj) & (i == ii)):
						sys.stdout.write(' x ')
					else:
						sys.stdout.write('   ')
		
				sys.stdout.write('-   -')
				if (vvv == i):
					sys.stdout.write('x')
				else:
					sys.stdout.write(' ')
				sys.stdout.write('-\n')
			print '--------------------------------------------------------------   ---'

			# Send them to the arduino
			data1 = ii * 10
			data2 = jj * 10
			data3 = vvv * 10
			
			if data1 >= 0 :
				byte1 = data1
				byte2 = data1
			else :
				byte2 = data1
				byte3 = data1
				
			if data2 >= 0 :
				byte1 = data2
				byte2 = data2
			else :
				byte4 = data2
				byte3 = data2
			
			byte5 = 254
			
			frame = net.create_data_frame(2, byte1, byte2, byte3, byte4, byte5)
			
			print str(frame)
			net.send(str(frame))
			
			#print "wait ln\n"
			print ser.readline()
			#time.sleep(0.05)
			#print "wait ended\n"
