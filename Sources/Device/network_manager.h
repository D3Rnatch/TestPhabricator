#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

/**
 * \file network_manager.h
 * \brief Serial class definitions.
 * \author Alexandre Brand
 * \version 1.0
 */

#include <Arduino.h>
#include "protocol_helper.h"

/**
 * \def TIMEOUT
 * \brief Timeout time.
 *
 * Set at 5ms timeout.
 */
#define TIMEOUT 5 

/**
 * \def W_BUFFER_SIZE
 * \brief Serial buffer size.
 *
 * Set at a size of 10 bytes.
 */
#define W_BUFFER_SIZE 10

#define DBG
//#define RELEASE

/**
 * \class Network_manager
 * \brief Serial manager for the device.
 */
class Network_manager
{
    public :
    	/**
	 * \brief Constructor.
	 */
	Network_manager(int);
	
	/**
	 * \brief Run a magical code on the serial port.
	 * 
	 * Updates read/write data Writes current writtable frames & reads usb entry.
	 * 
	*/
	void run_the_magic();

	/**
	 * \brief Returns the las received frame's ID.
	 * \return The last frame ID.
	 */
	int get_last_frame_id();

	/**
	* \brief Get data received array.
	* \return Returns a 6 bytes array.
	*/
	uint8_t * get_array();
	
	/**
	 * \brief Send data over serial port.
	 * \param b0 First byte to send.
	 * \param b1 Second byte to send.
	 * \param b2 Third byte to send.
	 * \param b3 Fourth byte to send.
	 * \param b4 Fifth byte to send.
	 * \param b5 sixth byte to send.
	 */
	void send(byte b0,byte b1, byte b2, byte b3, byte b4, byte b6);

	/**
	 * \brief Send data over serial port.
	 * \param id Frame id to send.
	 * \param b0 First byte to send.
	 * \param b1 Second byte to send.
	 * \param b2 Third byte to send.
	 * \param b3 Fourth byte to send.
	 * \param b4 Fifth byte to send.
	 * \param b5 sixth byte to send.
	 */
	void send_full(uint8_t id,byte b0, byte b1, byte b2, byte b3, byte b4, byte b5);

	/**
	 * \brief Send a packet over serial port.
	 * \param packet The packet to send.
	 */
	void send_packet(t_encap * packet);
	
	/**
	* \brief Preencap frames to be sent
	* \param errorCode Error code to send.
	*/
	void send_ready_packet(uint8_t errorCode);
	
	/**
	* \brief Reply by sending data packet to computer
	* \param : v1, adns X value
	* \param : v2, adns Y value
	* \param : v3, Gyro Z sensor
	*/
	void send_data_packet(uint8_t v1, uint8_t v2, uint8_t v3);
	
    private :
	t_frame_bytes last_extraction;/*!< Last extracted frame.*/
	t_encap encapsulation_stack[10];/*!< Encapsulated frames stack. */
	boolean flag;/*!< A simple flag, because why not ? */
};

#endif
