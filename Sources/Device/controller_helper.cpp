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
	coef_di_cos = (int)((20*cos(90))*1024);
	coef_di_sin = (int)((20*sin(90))*1024);
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
        this->prec_g = 0;
	
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
						
                                                // Calculate World Coodinates
                                                this->Process_Acq();
                                                this->calculate_Position();
                                                /*
                                                x = this->acq_x;
						y = this->acq_y;*/
						// g = this->acq_g;
						x = this->actualX;
                                                y = this->actualY;
                                                g = this->actualT;

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
                                                Serial.print("ACQ request : ");
                                                Serial.print(x,DEC);
                                                Serial.print(";");
                                                Serial.print(y,DEC);
                                                Serial.print(";");
                                                Serial.print(g,DEC);
                                                Serial.print(";");
                                                Serial.println(this->acq_g,DEC);
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
	/*Serial.print(" ::::::::::");
        Serial.print(delta_angle,DEC);*/
        this->acq_g = this->imu->getGValue();
	this->acq_x = this->acq->get_MoveX();
	this->delta_angle = this->acq_g - this->prec_g; // Delta Theta
        this->acq_y = this->acq->get_MoveY();/*
	Serial.print(" ::::::::::");
        Serial.print(acq_x,DEC);
        *///Serial.print(" ::::::::::");
        // Serial.println(delta_angle,DEC);
        this->prec_g = this->acq_g;
	
}


void Controller :: calculate_Position()
{

	int grid = gridsize; // 10 cm
	int dicos = (int)coef_di_cos;
	int disin = (int)coef_di_sin;
        int dx = this->deltaX;
	int dy = this->deltaY;
	int px = this->precX;
	int py = this->precY;
	int pt = this->precT;
	int ax;
	int ay;
	int at;

        // trasnform mouse data to cm data :
        this->acq_x *= (0.001727*10000);
        this->acq_y *= (0.001727*10000);
        
        // Etape une changement de repère :
        float angle = this->delta_angle * (3,14/180);
        angle = 3,14/2 - angle;
        // new x and y calculation :
        float dxm = this->acq_x*cos(angle) + this->acq_y*sin(angle);
        dxm /= 10000;
        float dym = this->acq_y*cos(angle) - this->acq_x*sin(angle);
        dym /= 10000;
        // In world coefficients are :
        float dTheta = dxm/20; // 20cm is the distance from center to optical mouse
        float dxWorld = dym*(dTheta/2);
        float dyWorld = dym*(1-(dTheta/6));
        
        // updating actual X,Y and T :
        this->actualX = this->precX + dxWorld;
        this->actualY = this->precY + dyWorld;
        this->actualT = this->precT + dTheta;
        /*
        Serial.print("ACQ;");
        Serial.print(dxWorld,DEC);
        Serial.print(";");
        Serial.print(dxWorld,DEC);
        Serial.print(";");
        Serial.print(dTheta,DEC);
        Serial.print(";");
        Serial.print(actualX,DEC);
        Serial.print(";");
        Serial.print(actualY,DEC);
        Serial.print(";");
        Serial.print(actualT,DEC);
        Serial.print(";");
        Serial.print(this->acq_x,DEC);
        Serial.print(";");
        Serial.print(this->acq_y,DEC);
        Serial.print(";");
        Serial.println(this->acq_g,DEC);*/
        
/*	
	// calculation of delta-distance :
	int alt = (disin * ( delta_angle - this->acq_y ))/1024;
	int alt2 = (dicos * this->acq_x)/1024;
	dx = alt + alt2;
	alt = (dicos * ( this->acq_y - delta_angle ))/1024;
	alt2 = (this->acq_x * disin)/1024;
	dy = alt + alt2;
	
	// calculation of position :
	alt = (int)(cos(pt)*1024);
	alt2 = (int)(sin(pt)*1024);
	ax = px + (dx*alt - dy*alt2)/1024;
	ay = py + (dx*alt2 + dy*alt)/1024;
	at = pt + delta_angle;
	
        /*Serial.print(dx,DEC);
        Serial.print(":");
        Serial.print(dy,DEC);
        Serial.print(":");
        Serial.print(pt,DEC);
        Serial.print(":");
        Serial.print(delta_angle);
        Serial.print(":");
        
	// Setting up inner data :
	this->actualX = ax;///this->gridsize;
	this->actualY = ay;///this->gridsize;
	this->actualT = at;*/
        //Serial.print("DX,DY,DO :");
        //Serial.print(ax,DEC);
        //Serial.print(":");
        //Serial.print(ay,DEC);
        //Serial.print(":");
        //Serial.println(at,DEC);
        this->delta_angle = 0;
	this->precX = this->actualX;
	this->precY = this->actualY;
	this->precT = this->actualT;
}

int Controller :: getXCoord()
{
	int t = this->actualX;
	return t;
}

int Controller :: getYCoord()
{
	int t = this->actualY;
	return t;
}

int Controller :: getTCoord()
{
	int t = this->actualT;
	return t;
}

