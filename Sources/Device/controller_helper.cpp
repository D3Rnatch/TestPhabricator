#include "controller_helper.h"


Controller :: Controller()
{
    int start = millis();
    int actual = 0;
  	motor_last_set = false;
	motor_off_cpt = 0;
	scaner_last_set = false;
	scaner_off_cpt = 0;
  
    gridsize = 10; // 10 cm
	coef_di_cos = 1;
	coef_di_sin = 1;
	this-> delta_angle = 0;
	this-> deltaX = 0;
	this-> deltaY = 0;
	this-> precX = 0;
	this-> precY = 0;
	this-> actualX = 0;
	this-> actualY = 0;
	this-> precT = 0;
	this-> actualT = 0;

	this->acq_x = 0;
	this->acq_y = 0;
	this->acq_g = 0;
	
	this->net = new Network_manager(115200);
        
	this->escenter[0].attach(3);
    	this->escenter[1].attach(5);
    	this->escenter[2].attach(7);
    	this->escenter[3].attach(11);
        this->escenter[4].attach(12);
	
        this->arm();
        
        delay(10);
        //Serial.println("ESC ARMED.");
        this->imu = new MPU_Handler();
        
        this->LaserScaner.attach(9);
	// Starting The Acq manager
	this->acq = new ACQ_handler();
	this->acq_system = 0;

        this->calibrate_laserscaner();
        //Serial.println("End of INIT Go for IMU.");
        actual = millis();
       //Serial.println("Loop time is : ");
       //Serial.println(actual-start);
}

void Controller :: init()
{
	this->controllerState = Idle;

	// Controller finished init 
	// Sending OK ready packet.

        // ////Serial.print("TEST");
        uint8_t errorCode = 0;
        errorCode = this->imu->getErrorCode();
	this->net->send_ready_packet(errorCode);
}

//////////////////////////////////
//				//
//	DEFINITIONS		//
//				//
//////////////////////////////////

void Controller ::  setSpeedcent(int a, int spd){
  //On envoie la vitesse désirée
  int vit = map(spd, 0, 100, 0, 180);
  this->escenter[a].write(vit);
}

void Controller ::  setSpeedFans(uint8_t * b)
{
  int vit[5]; 
 for(int i=0;i<5;i++) {
     if(b[i]>MAX) b[i] = MAX;
     else if (b[i] < MIN) b[i] = MIN;
     vit[i] = map(b[i],0,100,0,180);
  }
 for(int j=0;j<5;j++)
     this->escenter[j].write(vit[j]);  
}

void Controller ::  setAngleLaserScaner(uint8_t angle)
{
	// int vit = map(angle,0,180,1,179);
	this->LaserScaner.write(angle);
}

void Controller ::  calibrate_laserscaner()
{
	for(int i=0;i<180;i++){
            setAngleLaserScaner(i);
            delay(10);
        }
        for(int i=180;i>0;i--){
            setAngleLaserScaner(i);
            delay(10);
        }
}

void Controller ::  arm(){
 // Serial.println("Starting ARM : ");
  delay(5000);
  setSpeedcent(0,MIN);
  setSpeedcent(1,MIN);
  setSpeedcent(2,MIN);
  setSpeedcent(3,MIN);
  setSpeedcent(4,MIN);
//Serial.println("Begin MIN.");
  delay(5000);
  setSpeedcent(0,MAX);
  setSpeedcent(1,MAX);
  setSpeedcent(2,MAX);
  setSpeedcent(3,MAX);
  setSpeedcent(4,MAX);
//Serial.println("After MAX.");
  delay(5000);
  setSpeedcent(0,MIN);
  setSpeedcent(1,MIN);
  setSpeedcent(2,MIN);
  setSpeedcent(3,MIN);
  setSpeedcent(4,MIN);
  delay(1302);
//Serial.println("After ARM.");
}

void Controller ::  Process_Motor(uint8_t * b)
{
	if (this->motor_last_set == false) this->motor_last_set = true;
		       
        // we set fans power.
        setSpeedFans(b);
        // Serial.println("Fans Set.");
                
        for(int i=0;i<5;i++) {
               b[i] = 0;
        }
}

void Controller ::  Process_Scan(uint8_t * b)
{
	if (this->scaner_last_set == false){
		 // Setting to point 0 the scaner
		 // setAngleLaserScaner(0);		
		 this->scaner_last_set = true;
	}
	setAngleLaserScaner (b[0]);
	/*for(int i=0;i<4;i++) {
		b[i] = 0;
	}*/

}

void Controller ::  reset_Services()
{
	// TO BE IMPLEMENTED

	switch(this->controllerState)
	{
		case Idle :
                        setSpeedcent(4,MIN);
			break;

		case Scan :
                        setSpeedcent(4,MIN);
			break;

		case Manual :
			break;

		case Manual_Acquisition :
			break;
		
		default :
			break;
	}

        if(this->lastState == Scan) {
             // reattachinterrupt on IMU :
             // attachInterrupt(0,MPU_Handler::dmpDataReady,RISING);
             // this->lastState = Idle; 
        }
}

void Controller ::  Process_Com(uint8_t id, uint8_t * b)
{
  int x = 0;
  int y = 0;
  int g = 0;
  uint8_t g2 = 0;
	if (id != 200) {
  //Serial.println("Packet Received.");
  //Serial.println(id);
		if(id != 7 && id != 0 && id != (uint8_t)'\n'){
			this->net->send_full(id+48,b[0]+48,b[1]+48,b[2]+48,b[3]+48,b[4]+48,'\n');
                        //Serial.println("In If");
                 }
              switch(id) {
         
					case 2 : // Stop frame
						this->controllerState = Idle;
						break;
					case 3 : // Move frame
						this->controllerState = Manual;
                                                //Serial.println("Controller State to Manual.");
						break;
					case 4 : // Move + Acq frame
						// this->controllerState = Manual_Acquisition;
						break;
					case 5 : // Move + Acq frame
                                                //Serial.println("Controller State to Manual Acquisition.");
						this->controllerState = Manual_Acquisition;
						break;
					case 6 : // Scanning
                                                //Serial.println("Controller State to Scanning.");
						this->controllerState = Scan;
                                                // Obligation to detach the IMU interrupt.
                                                // detachInterrupt(0);
                                                // this->lastState = Scan;
						break;
					case 7 : // Request for acquisition
						// Rechange the data :
						// x = this->acq->get_MoveX();
						// y = this->acq->get_MoveY();
						// g = this->imu->getGValue();
						x = this->acq_x;
						y = this->acq_y;
						g = this->acq_g;
						
						if (x < 0) x = 255-x;
						if (y < 0) y = 255 - y;
						// Send packet ...
                                                // G sensor value trasnformation into 2 parts (360 being to high for a byte data : split into two)
                                                if(g < 0)
                                                    g = g + 360; // 0 à 360°
                                                if(g > 255) {
                                                  g2 = g-255; // on sature à 255 et on enlève le reste que l'on stocke dans g2
                                                  g  = g-g2; // ici : on récupère la prmeière partie
                                                }
                                                
                                                //Serial.print("ACQ request : ");
                                                //Serial.println(x,DEC);
                                                //Serial.println(y,DEC);
                                                this->net->send_data_packet(x,y,g,g2);
                                                //Serial.println("Sent.");
						break;
					default :
						this->controllerState = Idle;
						break; 
          
                  }
	
    	}
}

void Controller ::  Process_Acq()
{
	// ACQUISITION UPDATE
	// this->acq->run_the_magic(); // update values...
        // this->imu->run_the_magic();
	this->acq_system++;
	// Process PID
	
	// Update ACQuisition
	this->delta_angle = this->imu->getGValue() - this->acq_g; // Delta Theta
	this->acq_g = this->imu->getGValue();
	this->acq_x = this->acq->get_MoveX();
	this->acq_y = this->acq->get_MoveY();
}


void Controller :: calculate_Position(int angle)
{
	int grid = gridsize; // 10 cm
	float dicos = coef_di_cos;
	float disin = coef_di_sin;
	int dx = deltaX;
	int dy = deltaY;
	int px = precX;
	int py = precY;
	int pt = precT;
	int ax = actualX;
	int ay = actualY;
	int at = actualT;
	
	// calculation of delta-distance :
	int alt = (int)((disin * ( angle - this->acq_y )) << 5);
	alt = alt/1000;
	int alt2 = (int)((dicos * this->acq_x) << 5);
	alt2 = alt2/1000;
	dx = alt + alt2;
	alt = dicos * ( this->acq_y - angle );
	alt2 = this->acq_x * disin;
	dy = alt + alt2;
	
	// calculation of position :
	alt = (int)(cos(pt) << 5);
	alt = alt/1000;
	alt2 = (int)(sin(pt) << 5);
	alt2 = alt2/1000;
	ax = px + dx*alt - dy*alt2;
	ay = py + dx*alt2 + dy*alt;
	at = pt + angle;
	
	// Setting up inner data :
	this->actualX = ax;
	this->actualY = ay;
	this->actualT = at;
}

int Controller :: getXCoord()
{
	int t = this->actualX;
	this->precX = this->actualX;
	return t;
}

int Controller :: getYCoord()
{
	int t = this->actualY;
	this->precY = this->actualY;
	return t;
}

int Controller :: getTCoord()
{
	int t = this->actualT;
	this->precT = this->actualT;
	return t;
}

