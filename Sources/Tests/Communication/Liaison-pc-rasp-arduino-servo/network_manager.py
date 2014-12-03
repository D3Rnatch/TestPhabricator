# NET MANAGER IMPLEMENTATION FOR RASPBERRYPI AND ARDUINO COMMUNICATION
# 2014 PFE HOVERCRAFT
# Last Modified : D3Rnatch

import serial

from array import array

class Network_Manager :
	
	def __init__ (self,port,baud) :
		# We try the opening of the interface
		try :
			ser = serial.Serial(port,baud)
		except :
			print 'USB port does not exists.'
			print 'Program is quitting.'
			exit()
	
	# \brief send : sends a frame over usb port
	# The frame is a 5 bytes long Id + data, folowed of a ending byte ('\n')
	def send(self,frame) :
		try :
			ser.write(frame + '\n')
	
		except :
			print 'Error on writing'

	# \brief read : reads a frame over usb port
	# The frame is a 5 bytes long id + data, followed of an ending byte '\n'
	# See frames creation functions for further detail.
	def read(self) :
		try : 
			val = ser.readline()
		except : 
			print 'Error on reading'
			
		return val
	
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
		