# NET MANAGER IMPLEMENTATION FOR RASPBERRYPI AND ARDUINO COMMUNICATION
# 2014 PFE HOVERCRAFT
# Last Modified : D3Rnatch

import serial
import sys
from array import *

class Serial_Manager :
	
	def __init__ (self,port,baud) :
		# We try the opening of the interface
		try :
			self.ser = serial.Serial(port,baud)
		except :
			print 'USB port does not exists.'
			print 'Program is quitting.'
			exit()
	
	# \brief send : sends a frame over usb port
	# The frame is a 5 bytes long Id + data, folowed of a ending byte ('\n')
	def send(self,frame) :
		try :
			self.ser.write(frame + '\n')
	
		except :
			print 'Error on writing'

	# \brief read : reads a frame over usb port
	# The frame is a 5 bytes long id + data, followed of an ending byte '\n'
	# See frames creation functions for further detail.
	# \return Returns a list of 5 bytes 
	def read(self) :
		val = self.ser.readline()
		# test = array('B',val)
			
		# print str(' Read line is : ' + ret) 
		return val
		
	# \brief create_movement_frame : creates a frame according 
	# to the protocol_helper file on github repository and parameters.
	# \param id : frame id type. See NETWORK_PROTOCOL.txt file.
	# \param p1 : parameter 1
	# \param p2 to p5 are identical.
	def create_data_frame (self,id,p1,p2,p3,p4,p5) :
		pass
		# creating the frame
		#frame = array('c')
		
		# id
		#frame.insert(0,id)

		# p1
		#frame.insert(1,p1)
		#frame.insert(2,p2)
		#frame.insert(3,p3)
		#frame.insert(4,p4)
		#frame.insert(5,p5)

		# At end, when ready we return the present frame.
		#return frame

	def create_data_frame (self, id, p1, p2, p3, p4, p5) :
		frame = chr(id+48)
		frame += chr(p1+48)
		frame += chr(p2+48)
		frame += chr(p3+48)
		frame += chr(p4+48)
		frame += chr(p5+48)

		return frame

	# Creates a start and state packet. Unfreezes robot.
	# state : 0;1;2
	# start : 0;1
	def create_start_frame (self,state) :
		if state > 3 and state < 0 :
			state = 0
		frame = self.create_data_frame(0,state,1,0,0,0)
		return frame

	# Create a Move Frame
	def create_move_frame (self,p1,p2,p3,p4,p5) :
		frame = self.create_data_frame(2,p1, p2, p3, p4, p5)
		return frame
	
	# Create a Stop Frame
	def create_stop_frame (self) :
		frame = self.create_data_frame(1,1,0,0,0,0)
		return frame
			
	def create_point_frame (self) :
		pass

	def create_update_scaner_frame (self,angle) :
		frame = self.create_data_frame(4,angle,0,0,0,0)
		return frame


 
