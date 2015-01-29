#ifndef CONTROLLER_HELPER
#define CONTROLLER_HELPER

/**
 * \file controller_helper.h
 * \brief Main controller functions.
 * \author Alexandre Brand
 * \version 1.0
 *
 * This file contains main functions used in the device.
 * Those functions will manage every device module.
 * Writent in C in a C plus plus header... What a shame !
 *
 */
 
#include <Servo.h>

#include "network_manager.h"
#include "acq_handler_adns2051.h"
#include "mpu_handler.h"

#define MIN 10
#define MAX 90

/**
 * \enum enum State
 * \brief State enumerator.
 *
 * Describes the robot state.
 *
 */
enum State
{
   Idle, /*!< Waiting mode. */
   Manual, /*!< Manual mode. */
   Manual_Acquisition, /*!< Manual aquisition mode. */
   Scan, /*!< Scanning mode. */
   Automatic /*!< Auto mode. */
};



/**
 * \struct Controller_t
 * \brief Strore everything for the robot.
 */

class Controller
{

	public :
	Controller();

	void init();

	/**
 	* \fn void arm()
 	* \brief Configure motors.
 	*/
	void arm();

	/**
 	* \fn void setSpeedcent(int a, int spd)
 	* \brief Set desired speed.
 	* \param a Motor number.
 	* \param spd Desired speed.
 	*/
	void setSpeedcent(int a, int spd);
	
	/**
 	* \fn void setSpeedFans(uint* b)
 	* \brief Set fan speed.
 	* \param b Byte array from frame.
 	*/
	void setSpeedFans(uint8_t* b);

	/**
	 * \fn void setAngleLaserScaner(uint8_t angle)
	 * \brief Set and angle for the laser scaner servo.
 	* \param angle Angle to set.
 	*/
	void setAngleLaserScaner(uint8_t angle);

	/**
 	* \fn void calibrate_laserscaner()
 	* \brief Calibrate laser scaner servo.
 	*/
	void calibrate_laserscaner();

	/**
 	* \fn void Process_Motor(uint8_t* b)
 	* \brief Set motors speed.
 	* \param b Frame array.
 	*/
	void Process_Motor(uint8_t * b);

/**
 * \fn void Process_Scan(uint8_t* b)
 * \brief Send scaner angle position.
 * \param b Byte array from the Frame.
 */
void Process_Scan(uint8_t * b);

/**
 * \fn void Process_Acq()
 * \brief Process acquisition.
 */
void Process_Acq();

/**
 * \fn void reset_Services()
 * \brief Reset every services.
 */
void reset_Services();

/**
 * \fn void Process_Com(uint8_t id, uint8_t* b)
 * \brief Process frame messages.
 * \param id Frame id.
 * \param b Frame.
 */
void Process_Com(uint8_t id, uint8_t * b);

	// Servo :
	Servo escenter[5];/*!< Servo speed map. */
	Servo LaserScaner;/*!< Scanner servo. */
	
	// State :	
	State controllerState;/*!< Current state save. */

	// Services
	Network_manager *net;/*!< Network service */
	ACQ_handler *acq;/*!< Acquisition service */
	MPU_Handler *imu;/*!< IMU Service */
        uint8_t acq_system;

	unsigned long time;/*!< Time save */
	unsigned long last_time;/*!< Save last time */
	boolean motor_last_set;/*!< Is motors on ? */
	uint8_t motor_off_cpt;/*!< Motor off since. */
	boolean scaner_last_set;/*!< Is scanner on ? */
	uint8_t scaner_off_cpt;/*!< Scanner on since. */

        State lastState;

};


#endif
