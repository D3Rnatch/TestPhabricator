/**
 * \file protocol_helper.h
 * \brief Protocol helper utils functions
 * \author Alexandre Brand
 */

#ifndef PROTOCOL_HELPER_H
#define PROTOCOL_HELPER_H

#include <Arduino.h>

#define MAX_FRAME_SIZE 5

/**
 * \struct t_encap
 * \brief Frame encapsulation.
 * 
 * Frame structure:
 * ID|--|--|--|--|--|
 * 0 |1 |2 |3 |4 |5 |
 *
 */
typedef struct send_encap
{
	byte array[7];/*!< Frame array */
}t_encap;

// ========================================================================
//		FRAME DEFINITION ZONE
// This is totally temporary, waiting to find a better solution to c++ 
// void * compilers' problem.
// ========================================================================
 
/**
 * \struct t_frame_bytes
 * This defines an unsigned integer 8 bits composed frame.
 * (corresponding to ID : 0,1,2 & 3)
*/
typedef struct recept_frame_bytes
{
	uint8_t type;
	uint8_t array[6];
}t_frame_bytes;

/* This defines a double composed frame.
// (corresponds to ID : 4)
*/
typedef struct recept_frame_doubles
{
	uint8_t type;
	double array[2];
}t_frame_doubles;

// ========================================================================
//		SENDING PROTOCOL HELPER (arduino side)
//	Features :
//		-> Ready Frame
//		-> Data Frame
//		-> Battery Status Frame 
// ========================================================================
/**
 * \fn t_encap encap_acq_data(byte, byte, byte)
 * \brief encap_acq_data, encaps the acquisition data in format
 *
 */
t_encap encap_acq_data(byte, byte, byte, byte);

/**
 * \fn t_encap encap_ready()
 * \brief encap_ready, encaps the ready packet
 *
 */
t_encap encap_ready();


/**
 * \fn t_encap encap_battery_data(byte, byte)
 * \brief encap_battery_data, encaps the battery data in format
 *
 */
t_encap encap_battery_data(byte, byte);

// ========================================================================
//		EXTRACTION PROTOCOL HELPER (Arduino side)
//	Features :
//		-> State & Start Frame id : 0
//		-> Stop Frame : id 1
//		-> Move Frame : id 2
//		-> Power Frame : id 3
//		-> Point Frame : id 4
//	/!\ : Frame size = 5 bytes
// ========================================================================

/**
*	\brief extract_data, stores every bytes into an encap type variable
*	\param frame : array of 5 bytes aka a frame.
*/
// t_recept_encap extract_data(byte*frame);


// ========================================================================
//		ENCAPSULATION PROTOCOL HELPER (Rasp side)
//	Features :
//		-> State & Start Frame id : 0
//		-> Stop Frame : id 1
//		-> Move Frame : id 2
//		-> Power Frame : id 3
//		-> Point Frame : id 4
//	/!\ : Frame size = 5 bytes
// ========================================================================

/**
 *	\fn t_encap encap_special_frames(uint8, byte*, uint8_t)
 *	\brief encap_special_frames : encaps raspberry side frames
 *	\param uint8_t : frame type wanted
 *	\param byte *  : Array of values
 *	\param uint8_t :
 * 	################################### TODO
 */
t_encap encap_special_frames(uint8_t, byte*, uint8_t);


// ========================================================================
//		EXTRACTION PROTOCOL HELPER (Raspberry PI side)
//	Features :
//		-> Ready Frame; id 0
//		-> Data Frame; id 1
//		-> Battery Status Frame; id 2
//	/!\ : Frame size = 5 bytes
// ========================================================================

/**
 *	\fn uint8_t get_frame_type(byte*)
 * 	\brief get_frame_type. Returns frame read id (integer).
 *	\param byte* The read frame.
 *	\return uint8_t Unsigned short int.
 */
uint8_t get_frame_type(byte *);

/**
 *	\fn t_frame_bytes extract_data_bytes(byte*)
 *	\brief extract_data_bytes : extracts and concatenates values for id [0;4]
 *	\param byte * : raw input data
 *	\param t_frame_bytes : extraction frame
 */
t_frame_bytes extract_data_bytes(byte *);

/**
 *	\fn t_frame_doubles extract_data_doubles(bytes*)
 *	\brief extract_data_doubles : extracts and concatenates values for id 5
 *	\param byte * : raw input data
 *	\param t_frame_bytes : extraction frame (double types)
 */
t_frame_doubles extract_data_doubles(byte *);

#endif
