  //################################################"
// AUTOPILOT MAIN PROGRAM
//################################################"

#include <Arduino.h>

#include "ADNS2610_arduino_driver.h"
// #include "Drivers/MPU6050_arduino_driver.h"

// #include <PID_v1.h>


// class PID;

typedef struct s_ADNS2610
{
	double dx;
	double dy;
}ADNS2610;

typedef struct s_Points
{
        double setX;
        double setY;
}SetPoints;

//MPU6050 mpu; // IMU item
ADNS2610 opticSensor; // Optic sensor
SetPoints setPoints;

double outDx, outDy, outGz;
double setpDx, setpDy, setpGz;
int inX;
//PID *myPID;
//PID *myPID2;
int waitTime = 0;

void setup()
{
	// Setup of the interface ( Serial )
	Serial.begin(38400);
        Serial.println("Serial established.");
        Serial.flush();
/*        
        Serial.println("DBG d, d*, i, i* : ");
        Serial.print(sizeof(double));
        Serial.print(":");
        Serial.print(sizeof(double *));
        Serial.print(":");
        Serial.print(sizeof(int));
        Serial.print(":");
        Serial.print(sizeof(int *));
        Serial.println("");
*/ // DEBUG         
	// Setup of the optic sensor (linear odometry)
	mouseInit();
        opticSensor.dx = 0;
        opticSensor.dy = 0; // Setting up initial Dx and Dy values...

	// Setup of the I2C bus

	// Setup of the MPU6050 sensor (G sensor)

	// PID inits :
	setpDx = 50; // MAX VALUE REACHABLE ON LINEAR FILTERED DEP.
	setpDy = 0;
	setpGz = 0;
        outDx = 0.0;
        outDy = 0.0;
        /*myPID = new PID(&opticSensor.dx, &outDx, &setpDx, 2.0, 5.0, 1.0, DIRECT); // PID calculus DX
  	myPID2 = new PID(&opticSensor.dy, &outDy, &setpDy, 2.0, 5.0, 1.0, DIRECT); // PID calculus DY
        myPID->SetMode(AUTOMATIC);
  	myPID2->SetMode(AUTOMATIC);
        myPID->SetOutputLimits(-128, 127);
        myPID2->SetOutputLimits(-128, 127); */

	// Sends OK to raspberryPI :
	Serial.println("OK. Startup is complete.");
}


void loop()
{
	// ACQ : G sensor
        unsigned long now = millis();
        unsigned long lastTime = now;

	// ACQ : Optic Sensor
	opticSensor.dx = /*(double)*/ getFilteredValueX();
	opticSensor.dy = /*(double)*/ getFilteredValueY();
        //inX = getFilteredValueX();	
        //opticSensor.dx = getTrueValue((int)readRegister(regDeltaX));
        //opticSensor.dy = getTrueValue((int)readRegister(regDeltaY));

	// Output debug 
	//Serial.print("Input is : ");
	Serial.print((int)opticSensor.dx, DEC);
	Serial.print(";"); 
	Serial.print((int)opticSensor.dy, DEC);
        Serial.print(";");
        //Serial.print(getSqual(), DEC);
        //Serial.print(";");
        
	// Calculating PIDs
	// myPID->Compute();
	// myPID2->Compute();

	Serial.print("O is ;");
	Serial.print(outDx, DEC);
	Serial.print(";"); 
	Serial.print(outDy, DEC);

	// delay ? Or not...
//        delay(100);
                
        delay(12);
        now = millis();
        lastTime = now - lastTime;
        //Serial.print("Time : ");
        Serial.println(lastTime, DEC);
}
