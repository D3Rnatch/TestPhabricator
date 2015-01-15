/**	ARDUINO !!!
*	Network manager : 
*    This class handles the network over the arduino controler on the robot.
* Copyleft PFE Hovercraft 1408 - 2014/2015
*/


#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>

#include "protocol_helper.h"

// PARAMETER NON SETTABLE
#define TIMEOUT 5 // 5 ms timeout time

#define W_BUFFER_SIZE 10

#define DBG
//#define RELEASE

class Network_manager
{
	public :
		Network_manager(int);
		
		/**
		*	\brief run_the_magic : updates read/write data
		*		Writes current writtable frames
		*		 & reads usb entry.
		*/
		void run_the_magic();
		
		int get_last_frame_id();
		
		/**
		*	\brief get_array : returns data frame array
		*	\return uint8_t : contains 6 items.
		*
		*/
		uint8_t * get_array();

		void send(byte,byte,byte,byte,byte,byte);
                void send_full(uint8_t,byte,byte,byte,byte,byte,byte);
		void send_packet(t_encap * packet);

		/**
		*	Preencap frames to be sent
		*
		*/
		void send_ready_packet(uint8_t errorCode);
	
	private :
		t_frame_bytes last_extraction;
		t_encap encapsulation_stack[10];
                boolean flag;
};

#endif
