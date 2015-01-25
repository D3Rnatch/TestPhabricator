# NET MANAGER IMPLEMENTATION FOR RASPBERRYPI AND ARDUINO COMMUNICATION
# 2014 PFE HOVERCRAFT
# Last Modified : D3Rnatch

import serial
import sys
from array import *

## Serial communication manager.
#  
#  This module is here to manage the serial communication between the arduino and the onboard computer.
#
#  Fields :
#	- self.ser  => Serial object.
#
class Serial_Manager :

	## Class constructor.
	#  @param self The object pointer.
	#  @param port The usb port.
	#  @param baud Baud rate to use.	
    def __init__ (self,port,baud) :
		# We try the opening of the interface
		try :
			self.ser = serial.Serial(port,baud)
			handshack = self.read()
			print "handshack : " + str(handshack)

			# Testing handshack
			if handshack[0] == 0 and handshack[1] == 10 :
				print 'Controller is Ready.'
				print 'Error Code is : '
				print handshack[1]
			
		except :
			print 'USB port does not exists.'
			print 'Program is quitting.'
			exit()
	
	## Sends a frame over usb port.
	#  The frame is a 5 bytes long Id + data, folowed of a ending byte ('\n')
	#  @param self The object pointer.
	#  @param frame Fame to send.
    def send(self,frame):
		try :
			self.ser.flushOutput()
			self.ser.write(frame + '\n')
		except :
			print 'Error on writing'

	## Reads a frame over usb port.
	#  The frame is a 5 bytes long id + data, followed of an ending byte '\n'.
	#  See frames creation functions for further detail.
	#  @param self The object pointer.
	#  @return Returns a list of 5 bytes. 
    def read(self):
        val = self.ser.readline()
        self.ser.flushInput()
		# test = array('B',val)	
		# print str(' Read line is : ' + ret)
        return val
		
	## Creates a frame according. 
	#  to the protocol_helper file on github repository and parameters.
	#  @param id frame id type. See NETWORK_PROTOCOL.txt file.
	#  @param p1 parameter 1
	#  @param p2 parameter 2
	#  @param p3 parameter 3
	#  @param p4 parameter 4
	#  @param p5 parameter 5
	#  @return The created frame.
    def create_data_frame (self, id, p1, p2, p3, p4, p5):
        frame = chr(id+48)
        frame += chr(p1+48)
        frame += chr(p2+48)
        frame += chr(p3+48)
        frame += chr(p4+48)
        frame += chr(p5+48)
        return frame

	## Creates a start and state packet. Unfreezes robot.
	#  state : 0;1;2
	#  start : 0;1
	#  @param self The object pointer.
	#  @param state State to send.
	#  @return The created frame.
    def create_start_frame(self,state):
        if state < 0 :
            state_temp = 0
        else:
            state_temp = state
        frame = self.create_data_frame(1,state_temp,1,0,0,0)
        return frame

	## Create a Move Frame.
	#  @param id frame id type. See NETWORK_PROTOCOL.txt file.
	#  @param p1 parameter 1.
	#  @param p2 parameter 2.
	#  @param p3 parameter 3.
	#  @param p4 parameter 4.
	#  @param p5 parameter 5.
	#  @return The crated frame.
    def create_move_frame (self,p1,p2,p3,p4,p5):
        frame = self.create_data_frame(3,p1, p2, p3, p4, p5)
        return frame
    
    ## Create a Stop frame.
    #  @param self The object pointer.
    #  @return The frame created.
    def create_stop_frame(self):
        frame = self.create_data_frame(2,1,0,0,0,0)
        return frame
    
    ## Create point frame (Not implemented).
    #  @param self The object pointer.
    #  @param position Position tuple to go to (x, y).
    #  @return The created frame.
    def create_point_frame(self, position):
        if position[0] > 200:
            x = 200
        elif position[0] < 0:
            x = 0
        else:
            x = position[0]
        
        if position[1] > 200:
            y = 200
        elif position[1] < 0:
            y = 0
        else:
            y = position[1]
        frame = self.create_data_frame(5, 0, x, 0, y, 0)
        return frame

    ## Create a scanner servo frame.
    #  @param self The object pointer.
    #  @param angle Angle to send.
    #  @return The created frame.
    def create_update_scaner_frame(self, angle):
	frame = self.create_data_frame(6,angle,0,0,0,0)
	return frame
	
    ##########################################################
    #
    #	EXTRACTION ZONE
    #
    ##########################################################

    ## Create the read request frame.
    #  @param self The object pointer.
    #  @param id The id to read.
    #  @return The created frame.
    def create_read_frame (self, id) :
	frame = self.create_data_frame(7, id, 0, 0, 0, 0)
        return frame
				
