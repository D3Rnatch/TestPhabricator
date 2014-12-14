import time
import os
import sys
import socket
import serial
from network_manager import *

# init socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('localhost', 5000))

raw_input 'Please Press Enter to connect to Robot'

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
			sending_data = ii * 10
			#if sending_data > 60:
			#	sending_data = 60
			#if sending_data < 20:
			#	sending_data = 20

			print str(sending_data)
			ser.write(str(sending_data) + '\n')
			#print "wait ln\n"
			print ser.readline()
			#time.sleep(0.05)
			#print "wait ended\n"
