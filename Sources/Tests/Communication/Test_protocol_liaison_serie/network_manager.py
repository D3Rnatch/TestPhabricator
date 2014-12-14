# NET MANAGER IMPLEMENTATION FOR RASPBERRYPI AND ARDUINO COMMUNICATION
# 2014 PFE HOVERCRAFT
# Last Modified : D3Rnatch

import serial
import sys
from array import array
from struct import *

class Network_Manager :
	
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
		test = array('B',val)
			
		# print str(' Read line is : ' + ret) 
		return test

	# We assume the reading shit is always same god damn size
	def read_unpack(self) :
		val = self.ser.readline()
		size = sys.getsizeof(val)
		st = ''
		print 'DBG : size is : ' + str(size)
		stt = ''
		for i in range(0,size/8) :
			st = st + 'c'
			stt += val[i]		
		print 'DBG : ' + st
		frame = unpack(st,stt)
		
		print 'Reading incomming frame : ' + str(frame)
		print 'ID is ' + frame[0]
		
		return frame


	def read_line(self) :
		return self.ser.readline()
	
	# \brief create_movement_frame : creates a frame according 
	# to the protocol_helper file on github repository and parameters.
	# \param id : frame id type. See NETWORK_PROTOCOL.txt file.
	# \param p1 : parameter 1
	# \param p2 to p5 are identical.
	def create_data_frame (self,id,p1,p2,p3,p4,p5) :
		# creating the frame
		frame = array('c')
		
		# id
		frame.insert(0,id)

		# p1
		frame.insert(1,p1)
		frame.insert(2,p2)
		frame.insert(3,p3)
		frame.insert(4,p4)
		frame.insert(5,p5)

		# At end, when ready we return the present frame.
		return frame

	def create_packed_data_frame (self,idd,p1,p2,p3,p4,p5) :
		frame = pack('cccccc',idd,p1,p2,p3,p4,p5)
		
		print 'Encaped Data : ' + frame

		return frame

		
