
#include "PI_lib.h"

PIController :: PIController(float Kp, float Ki, float set)
{
	this->Kp = Kp;
	this->Ki = Ki;
	this->error = 0.0;
	this->input = 0.0;
	this->setPoint = set;
	this->coefKp = 0.0;
	this->coefKi = 0.0;
	this->output = 0.0;
	this->Dt = 0;
	this->actual = 0;
	this->prec = 0;
}

void PIController ::calculateError()
{
	error = setPoint - input;
}

void PIController ::calculateProportionalComponent()
{
	this->coefKp = error;
}

void PIController ::calculateIntegrativeComponent()
{
	this->coefKi = this->coefKi + this->error * this->Dt;
}

void PIController ::run_the_magic(input)
{
	this->input = input;
	this->calculateError();
	this->updateDeltaTime();
	this->calculateProportionalComponent();
	this->calculateIntegrativeComponent();
	this->output = Ki * this->coefKi + Kp*this->coefKp;
}

void PIController :: updateDetaTime()
{
	this->actual = millis();
	this->Dt = this->actual - this->prec;
	this->prec = this->actual;
}