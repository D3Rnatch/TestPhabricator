// =========================================================================
// 			THIS IS A TEST PROTOCOL PROGRAM
// Its goal is to test the network modules and verify the data integrity.
// (On Arduino reception side)
// =========================================================================

#include <Arduino.h>

#include "protocol_helper.h"
#include "motor_handler.h"

typedef struct control
{
	Motor_handler *motor;
	bool start;
	uint8_t state;
}control_t;

control_t Controller;

t_encap sending_frame;

String mystring = "";

void setup()
{
	// Setting up environment values
	Controller.start = false;
	Controller.state = -1;	

	// Starting network module
	Serial.begin(9600);

	// Motor handler
	Controller.motor = new Motor_handler(0,100,0,2,4,6,8);
	
	delay(1000);

	// extract motor data :
	  for (uint8_t i=0;i<5;i++) {
		if(i == 4) 
			Controller.motor->activate_motor(i,30);
		else
	  		Controller.motor->activate_motor(i,30);
	}

	Controller.motor->run_the_magic();

	delay(1000);

	// extract motor data :
	  for (uint8_t i=0;i<5;i++) {
		if(i == 4) 
			Controller.motor->activate_motor(i,90);
		else
	  		Controller.motor->activate_motor(i,90);
	}

	Controller.motor->run_the_magic();

	delay(1000);

	// extract motor data :
	  for (uint8_t i=0;i<5;i++) {
		if(i == 4) 
			Controller.motor->activate_motor(i,30);
		else
	  		Controller.motor->activate_motor(i,30);
	}

	Controller.motor->run_the_magic();

	// Send handcheck : frame : controller Ready
	sending_frame = encap_ready();
	Serial.write(sending_frame.array,5);
}

uint16_t cpt = 0;

void Process_Com(byte * value)
{
	t_frame_bytes e = extract_data_bytes(value);     
	
	switch(e.type)
	{
		// It is a state and start signal
		case 0 :
			Controller.state = (uint8_t)e.array[1];
			Controller.start = (uint8_t)e.array[2];
		break;
		// Stop frame
		case 1 :
			Controller.start = 0;
		break;
		// Move call...
		case 2 :
			if((Controller.state ==0 || Controller.state == 1 )&&(Controller.start == 1)) {
			// Controller.motor->update_activation_matrix(e.array[0]==0?0:1,e.array[1]==0?0:1,e.array[2]==0?0:1,e.array[3]==0?0:1,1);
			Controller.motor->update_power_matrix(e.array[0],e.array[1],e.array[2],e.array[3],30); }
		break;
	}
}

void loop()
{
	// On récupère l'entrée utilisateur
	int value2;
  
  	//while(Serial.available() > 0)
  	//{
	  // We get the entry in 8bytes max.
   	  /* char value[7]; 
          Serial.readBytes(value, 7);
    	 
		
	 // Serial.print("Frame read : ");
	 // Serial.println(++cpt,DEC);

	  // Extraction and setting of motors...
	  Process_Com((byte*)value);

	  Controller.motor->run_the_magic();

	  for (int i=0;i<7;i++)
	  	Serial.print(value[i]);
	  
	  Serial.flush();
          */
          // printing values 
	  /*
          uint8_t b1 = 128;
          uint8_t b2 = 126;
          uint8_t b3 = 123;
          sending_frame = encap_acq_data(b1,b2,b3);
          Serial.write(sending_frame.array,5); */
        // }
	
	// Net update
	Controller.motor->run_the_magic();
}


