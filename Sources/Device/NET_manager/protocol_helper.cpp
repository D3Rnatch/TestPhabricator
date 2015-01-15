#include "protocol_helper.h"


/**
*	\brief encap_acq_data, encaps the acquisition data in format
*
*/
t_encap encap_acq_data(byte dx,byte dy,byte gz)
{
	t_encap t;

	t.array[0] = (byte)1;
	t.array[1] = (char)dx;
	t.array[2] = (char)dy;
	t.array[3] = (char)gz;
	t.array[4] = (char)0;
	t.array[5] = (char)0;
	t.array[6] = (char)'\n';

	return t;
}

/**
*	\brief encap_ready, encaps the ready packet
*
*/
t_encap encap_ready()
{
	t_encap t;

	t.array[0] = (byte)0;
	t.array[1] = (char)10;
	t.array[2] = (char)10;
	t.array[3] = (char)10;
	t.array[4] = (char)10;
	t.array[5] = (char)10;
	t.array[6] = (char)'\n';
	
	return t;
}

/**
*	\brief encap_battery_data, encaps the battery data in format
*
*/
t_encap encap_battery_data(byte b1, byte b2)
{
	t_encap t;

	t.array[0] = (byte)2;	
	t.array[1] = (char)b1;
	t.array[2] = (char)b2;
	t.array[3] = (char)0;
	t.array[4] = (char)0;
	t.array[5] = (char)0;
	t.array[6] = (char)'\n';
	
	return t;
}

// return frame's id.
uint8_t get_frame_type(byte *frame)
{
	uint8_t t = frame[0] - 48; // Transformation into integer
	if (t > 255) t = 255;
	if (t < 0) t = 0;
	return t;
}


// extracts for frame id [0;4]
t_frame_bytes extract_data_bytes(byte *frame)
{
	t_frame_bytes extract;
	// retrieving extraction data type
	extract.type = get_frame_type(frame);
	
	// extraction rest of values...
	for(uint8_t i=1; i<6;i++) {
		int t = frame[i] - 48;
		if(t < 0) t = 0;
		if(t > 255) t = 255;
		extract.array[i-1] = t;
	}

	return extract;
}

// Extracts for frame id = 5
t_frame_doubles extract_data_doubles(byte *frame)
{
	t_frame_doubles extract;
	extract.type = get_frame_type(frame);
	// Point frame
/*
	uint16_t t = frame[1] << 8;
	t = t | frame[2];
	extract.array[0] = (double)t;
	t = frame[2] << 8; 
	t |= frame[3];
	extract.array[1] = (double)t;
	return extract;
// */
}

