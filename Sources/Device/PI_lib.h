/*
PI Library
*/

#define DEFAULT_KI 1.0
#define DEFAULT_KP 0.0

#include <Arduino.h>

class PIController
{
	public :
	PIController(float Kp, float Ki, float setpoint);
	
	void calculateError();

	void calculateProportionalComponent();

	void calculateIntegrativeComponent();
	
	void run_the_magic(float input);
	
        void updateDeltaTime();

	inline void setKp(float n){ this->Kp = n; }
	inline void setKi(float b){ this->Ki = b; }
	inline void setInput(float i){ this->input = i; }
	inline float getOutput(){ return this->output; }
	
	float Kp;
	float Ki;
	float error;
	float input;
	float setPoint;
	float coefKp;
	float coefKi;
	float output;
	int actual;
	int prec;
	int Dt; 
};
