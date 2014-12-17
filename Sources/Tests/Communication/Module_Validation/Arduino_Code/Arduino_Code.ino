
#include <Arduino.h>
#include <Servo.h>
#include "network_manager.h"


Network_manager *net;
unsigned long time;
unsigned long last_time;
boolean motor_last_set = false;

#define MIN 10
#define MAX 90

Servo escenter[4];

void setSpeedcent(int a, int spd){
  //On envoie la vitesse désirée
  int vit = map(spd, 0, 100, 0, 180);
  escenter[a].write(vit);
}

void setSpeedFans(uint8_t * b)
{
  int vit[4]; 
 for(int i=0;i<4;i++) {
     if(b[i]>MAX) b[i] = MAX;
     else if (b[i] < MIN) b[i] = MIN;
     vit[i] = map(b[i],0,100,0,180);
  }
 for(int j=0;j<4;j++)
     escenter[j].write(vit[j]);  
}

void arm(){
  setSpeedcent(0,MIN);
  setSpeedcent(1,MIN);
  setSpeedcent(2,MIN);
  setSpeedcent(3,MIN);
  delay(1200);
  setSpeedcent(0,MAX);
  setSpeedcent(1,MAX);
  setSpeedcent(2,MAX);
  setSpeedcent(3,MAX);
  delay(1200);
  setSpeedcent(0,MIN);
  setSpeedcent(1,MIN);
  setSpeedcent(2,MIN);
  setSpeedcent(3,MIN);
  delay(1200);
}

void Process_Com(uint8_t id, uint8_t * b)
{
	if(id == 2) {
                if (motor_last_set == false) motor_last_set = true;
		
                net->send(b[0],b[1],b[2],b[3],b[4],'\n');
                
                // we set fans power.
                setSpeedFans(b);
                
                for(int i=0;i<4;i++) {
                  b[i] = 0;
                }                
	}
        else if( motor_last_set == false) {
           motor_last_set = true;
           uint8_t c[5] = {MIN,MIN,MIN,MIN,MIN};
           setSpeedFans(c); 
        }
}

void setup()
{
	escenter[0].attach(2);
    	escenter[1].attach(4);
    	escenter[2].attach(6);
    	escenter[3].attach(8);
    	arm();
	net = new Network_manager(9600);
}

uint8_t cpt = 0;
void loop()
{ 
              //if(cpt%5) {
              //  cpt = 0;
		// acquire data
		net->run_the_magic();
		
		// resend data
		uint8_t id = net->get_last_frame_id();
		uint8_t * b = net->get_array();
		
		// DBG : pong
		// net->send(b[0],b[1],b[2],b[3],b[4],'\n');
		
		Process_Com(id, b);
             // }
             // cpt ++;
}


