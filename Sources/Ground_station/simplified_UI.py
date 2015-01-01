import pygame
import time
import os
import sys
import select
import socket
import argparse
import json

# Init arg parse
parser = argparse.ArgumentParser(
	description='This program intend to control the Hovercraft with a joystick.\nIt connects to the port 5000.',
	epilog='Examples comming soon')
parser.add_argument("address", help="Ip address of the robot on the network")
args = parser.parse_args()

# init pygame
pygame.init()
pygame.joystick.init()
if pygame.joystick.get_init() == False :
	print 'No no ! No joystick !'
	exit()

# Some vars
x = 0.0
y = 0.0
v = 0.0
i = 0
j = 0
vv = 0
ii = 0
jj = 0
vvv = 0
robot_x = 0
robot_y = 0
robot_z = 0
connected = False
compteur = 250
recv_data = ""
recv_message_type = ""
recv_message = ""
send_message_type = ""
send_message = ""
state = ""
ai = ""

# Run the app
while (1):
	# init the joystick
	pygame.joystick.Joystick(0).init()
	if pygame.joystick.Joystick(0).get_init() == False :
		print 'No no ! No joystick initialised !'
		exit()

	# purge events
	pygame.event.pump()

	# Get infos
	x = pygame.joystick.Joystick(0).get_axis(0)
	y = pygame.joystick.Joystick(0).get_axis(1)
	v = pygame.joystick.Joystick(0).get_axis(3)
	if pygame.joystick.Joystick(0).get_button(0) == True :
		print('Bye bye')
		exit()
	
	# Manage connection
	if (compteur < 10):
		compteur = compteur + 1
	if ((pygame.joystick.Joystick(0).get_button(8) == True) & (compteur > 5)) :
		compteur = 0
		if connected == False:
			try:
				s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				s.connect((args.address, 5000)) 
				send_message = "hello"
				send_message_type = "system"
			except:
				print 'Error while connecting ! '
			connected = True
		else:
			s.close()
			connected = False
	if ((pygame.joystick.Joystick(0).get_button(1) == True) & (compteur > 5)) :
		compteur = 0
		send_message = "stop"
		send_message_type = "system"
	if ((pygame.joystick.Joystick(0).get_button(2) == True) & (compteur > 5)) :
		compteur = 0
		if state == "scan" || state == "wait":
			send_message = "move"
			send_message_type = "set_state"
		else:
			send_message = "scan"
			send_message_type = "set_state"

	# determine real x and y in the referential
	for i in range(0, 20):
		if ((x*10 <= i - 10) & (x*10 > i -11)):
			jj = i

	for j in range(0, 20):
		if ((y*10 <= j - 10) & (y*10 > j - 11)):
			ii = j

	for vv in range(0, 20):
		if ((v*10 <= vv - 10) & (v*10 > vv - 11)):
			vvv = vv

	# If connected, check for data !
	if (connected == True):
		# check if data received
		try:
			inputready, outputready, exceptready = select.select([s], [], [], 0)
		except select.error, e:
            	    print 'Fatal error with select :\n\t', e, '\n\nExiting.\n'
            	    exit()
        	except socket.error, e:
            	    print 'Fatal error with socket while selecting inputs :\n\t', e, '\n\nExiting\n'
            	    exit()
        	except Exception, e:
                    print 'Fatal unknown error while selecting inputs :\n\t', e, '\n\nExiting\n'
                    exit()
		for inp in inputready:
			if inp == s:
				data = s.recv(1024)
				if data == "":
					s.close()
					connected = False
				elif ord(data[0]) == 123 :
					recv_data = data
					try: 
						decoded_data = json.loads(recv_data)
					except Exception, e:
						print "Error while decoding received data : \n\t" + str(e)
						exit()
					try:
						robot_x = decoded_data['robot']['X']
						robot_y = decoded_data['robot']['Y']
						robot_z = decoded_data['robot']['R']
					except Exception, e:
						print "Bad formed JSON : \n\tMiss " + str(e)
					try:
						recv_message = decoded_data['message']['content']
						recv_message_type = decoded_data['message']['type']
						# store current states in local vars
						if recv_message_type == "state_info":
							state = recv_message
						elif recv_message_type == "ai_info":
							ai = recv_message
					except Exception, e:
						print "No message received."
				else:
					print "Data : " + data
					exit()

	# If connected, send data !
	if (connected == True):
		if send_message == "":
			message = ""
		else:
			message = ',"message":{"type":"' + send_message_type + '","content":"' + send_message + '"}'
			send_message_type = ""
			send_message = ""
		s.send('{"robot":{"X":' + str(jj) + ',"Y":' + str(ii) + ',"T":' + str(vvv) + '}' + message + '}')

	# print those
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
	if connected == False :
		print 'Not connected'
	else:
		print 'Connected'	
	print "Robot info :\n\t x = " + str(robot_x) + "\n\t y = " + str(robot_y) + "\n\t r = " + str(robot_z)
	print "Last message from the robot :\n\t" + recv_message_type + "\t" + recv_message + "\n"
	print "State : " + state + "\t\tAI : " + ai + "\n\n"
	# Wait a little time
	time.sleep(0.1)
