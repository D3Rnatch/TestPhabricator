#include "protocol_helper.h"


/**
*	\brief encap_acq_data, encaps the acquisition data in format
*
*/
t_encap encap_acq_data(byte dx,byte dy,byte gz)
{
	t_encap t;

	t->array[0] = (byte)1;
	t->array[1] = dx;
	t->array[2] = dy;
	t->array[3] = gz;
	t->array[4] = '\n';

	return t;
}

/**
*	\brief encap_ready, encaps the ready packet
*
*/
t_encap encap_ready()
{
	t_encap t;

	t->array[0] = (byte)2;
	t->array[1] = 0;
	t->array[2] = 0;
	t->array[3] = 0;
	t->array[4] = '\n';
	
	return t;
}

/**
*	\brief encap_battery_data, encaps the battery data in format
*
*/
t_encap encap_battery_data(byte b1, byte b2)
{
	t_encap t;

	t->array[0] = (byte)2;
	t->array[1] = b1;
	t->array[2] = b2;
	t->array[3] = 0;
	t->array[4] = '\n';
	
	return t;
}
