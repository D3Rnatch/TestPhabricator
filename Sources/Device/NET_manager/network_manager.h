/**	ARDUINO !!!
*	Network manager : 
*    This class handles the network over the arduino controler on the robot.
* Copyleft PFE Hovercraft 1408 - 2014/2015
*/


// /!\ Mostly needs to be implemented.

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

// PARAMETER NON SETTABLE
#define TIMEOUT 5 // 5 ms timeout time

#define DBG
//#define RELEASE

class Network_manager
{
	public :
		Network_manager(int);
	
		void run_the_magic();
	
		void send_dbg_message_to_computer();
		void send_dbg_message_to_user();
		void send_data_to_computer();
		void send_data_to_user();
	
		char read_data_from_buffer();
		void write_data_to_buffer();
	
	private :

	// This is mainly usefull in case of high communication level.
	char tmp_read[64] ; // Reading FILE : stores every entry information from computer.
	int tmp_read_status;
	char tmp_write[64]; // Writing FILE : stores every information to be sent to computer.
	int tmp_write_status;
};

#endif