/**
 * @file actuator_DynamixelXM.h
 * @brief actuator_DynamixelXM
 * @version 1.0
 *
 * Version 1.01 2022-04-14
 * 	-Première version
 */

#ifndef TEAMATLIBS_ACTUATOR_DYNAMIXEL_XM_H_
#define TEAMATLIBS_ACTUATOR_DYNAMIXEL_XM_H_

#include <TeamAt_H_Dynamixel/dynamixel_xm540address.h>
#include <TeamAt_H_Dynamixel/dynamixelProtocolV2.h>
#include "TeamAT_B_Robocore/robocore_actuator.h"
#include "CircularBuffer_V1_1.h"
//#include "uartManager_V1_0.h"

#define TX_DATA_SIZE 6
#define TX_BUFFER_SIZE (TX_DATA_SIZE*20)+1 // Max Pending data message of 6 bytes in memory

class actuator_DynamixelXM: public robocore_actuator {
public:
	actuator_DynamixelXM();
	virtual ~actuator_DynamixelXM();




	//------------------------------------------------------------------------//
	// Actuator managements functions
	//------------------------------------------------------------------------//

	/**
	 * @fn actuatorError_rct enableControl()
	 * @brief Enabler motor control
	 *
	 * @return status
	 */
	actuatorError_rct enableControl();

	/**
	 * @fn actuatorError_rct disableControl()
	 * @brief Disable motor control
	 *
	 * @return status
	 */
	 actuatorError_rct disableControl();

	 /**
	  * @fn actuatorError_rct setZeroPosition()
	  * @brief Set the current position as the Zero position
	  *
	  * @note: Control must be disabled for the function to work.
	  *
	  * @return status
	  */
	 actuatorError_rct setZeroPosition();

	//------------------------------------------------------------------------//
	// Actuator Data Set functions
	//------------------------------------------------------------------------//

		//MX 540  parameters Config
		#define XM540_POSITION_KP_MAX		16383.0
		#define XM540_POSITION_KP_MIN		0.0
		#define XM540_POSITION_KP_DEFAULT	800

		#define XM540_POSITION_KI_MAX		16383.0
		#define XM540_POSITION_KI_MIN		0.0
		#define XM540_POSITION_KI_DEFAULT	0.0

		#define XM540_POSITION_KD_MAX		16383.0
		#define XM540_POSITION_KD_MIN		0.0
		#define XM540_POSITION_KD_DEFAULT	0.0

		#define XM540_VELOCITY_KP_MAX		16383.0
		#define XM540_VELOCITY_KP_MIN		0.0
		#define XM540_VELOCITY_KP_DEFAULT	100

		#define XM540_VELOCITY_KI_MAX		16383.0
		#define XM540_VELOCITY_KI_MIN		0.0
		#define XM540_VELOCITY_KI_DEFAULT	1902

		#define XM540_FF1_GAIN_MAX		16383.0
		#define XM540_FF1_GAIN_MIN		0.0
		#define XM540_FF1_GAIN_DEFAULT	0.0

		#define XM540_FF2_GAIN_MAX		16383.0
		#define XM540_FF2_GAIN_MIN		0.0
		#define XM540_FF2_GAIN_DEFAULT	0.0

		/**
		 * @fn actuatorError_rct setKpGain(float)
		 * @brief Set position control P gain
		 *
		 * @param data Kp gain, value XM540_XM540_POSITION_KP_MIN to XM540_XM540_POSITION_KP_MAX
		 */
		actuatorError_rct setKpGain(float data);

		/**
		 * @fn actuatorError_rct setKiGain(float)
		 * @brief Set Position control I gain
		 *
		 * @param data Ki gain, value XM540_POSITION_KI_MIN to XM540_POSITION_KI_MAX
		 *
		 * @return
		 */
		actuatorError_rct setKiGain(float data);

		/**
		 * @fn actuatorError_rct setKdGain(float)
		 * @brief Set position D gain
		 *
		 * @param data kd gain value XM540_POSITION_KD_MIN to XM540_POSITION_KD_MAX
		 * @return
		 */
		actuatorError_rct setKdGain(float data);

		// Placeholder for Calculated values by user
		void setCalcAngularVelocity(angularVeocity_rct data);

	//------------------------------------------------------------------------//
	// Dynamixel specific functions
	//------------------------------------------------------------------------//

		/**
		 * @fn uint8_t getId()
		 * @brief Get the ID of the dynamixel actuator
		 *
		 * @return the las 8 bits of the id value
		 */
		uint8_t getId();

		/**
		 * @fn actuatorError_rct setLed(bool)
		 * @brief Turn actuator led on / off
		 *
		 * @param ledState True = on
		 * @return status
		 */
		actuatorError_rct setLed(bool ledState);

		/**
		 * @fn actuatorError_rct sendIdChangeMessage(uint8_t)
		 * @brief Change the ID of the actuator.
		 * @note The motor will stop to respond, code needs to be update with the new ID afterward
		 *
		 * @param id the new ID
		 * @return
		 */
		actuatorError_rct sendIdChangeMessage(uint8_t id);

		/**
		 * @fn actuatorError_rct setBaudRate(dynamixelBaudrate_t)
		 * @brief Configure the baudrate of the actuator.
		 * @note The motor will stop to respond, code needs to be update with the new baudrate afterward
		 *
		 * @param baud See the dynamixelBaudrate_t enum in dynamixelProtocolV2.h
		 * @return status
		 */
		actuatorError_rct setBaudRate(dynamixelBaudrate_t baud);

		/**
		 * @fn actuatorError_rct setDriveMode(uint8_t)
		 * @brief Configure the drive mode.
		 * @ see https://emanual.robotis.com/docs/en/dxl/x/xm540-w270/#drive-mode10
		 * @note Motor control needs to be disabled
		 *
		 * @param dMode Drive mode config bits
		 * @return status
		 */
		actuatorError_rct setDriveMode(uint8_t dMode);

		/**
		 * @fn actuatorError_rct setOperatingMode(operatingMode_t)
		 * @brief Configure operating mode
		 * @see https://emanual.robotis.com/docs/en/dxl/x/xm540-w270/#operating-mode11
		 * @note Motor control needs to be disabled
		 *
		 * @param opMode config bits
		 * @return status
		 */
		actuatorError_rct setOperatingMode(operatingMode_t opMode);

		/**
		 * @fn actuatorError_rct setOffsetAngle_HW(angleRad_rct)
		 * @briefChange the offset angle of the motor
		 *
		 * @note Motor control needs to be disabled
		 * @warning Make sure to update you control angle to matche the offset to avoid unwanted movement.
		 *
		 * @param offsetAngle the angle
		 * @return status
		 */
		actuatorError_rct setOffsetAngle_HW(angleRad_rct offsetAngle);

		/**
		 * @fn actuatorError_rct setMovingTreshold_HW(angularVeocity_rct)
		 * @brief Set the speed at which the actuator sets the "moving" flag to 1
		 *
		 * @param velocity the threshold value
		 * @return status
		 */
		actuatorError_rct setMovingTreshold_HW(angularVeocity_rct velocity);

		/**
		 * @fn actuatorError_rct setCurrentLimit_HW(float)
		 * @brief Limit the current control command limit value
		 * @note Actuator control must be disabled to change
		 * @note Configured and managed in the Dynamixel actuator
		 *
		 * @param current_mA Limit value ( needs to be > 0)
		 * @return	status
		 */
		actuatorError_rct setCurrentLimit_HW(float current_mA);

		/**
		 * @fn actuatorError_rct setVelocityLimit_HW(angularVeocity_rct)
		 * @brief Limit the velocity control command limit value
		 * @note Actuator control must be disabled to change
		 * @note Configured and managed in the Dynamixel actuator
		 *
		 * @param velocity in in deg/s or rad/se ( type angularVeocity_rct)
		 * @return	status
		 */
		actuatorError_rct setVelocityLimit_HW(angularVeocity_rct velocity);

		/**
		 * @fn actuatorError_rct setPwmLimit_HW(float)
		 * @brief Se the limit of the PWM command ( 0 to 100%)
		 * @note Actuator control must be disabled to change
		 * @note Configured and managed in the Dynamixel actuator
		 *
		 * @param pwm the limit for 0.0% to 100.0%
		 * @return status
		 */
		actuatorError_rct setPwmLimit_HW(float pwm);

		/**
		 * @fn actuatorError_rct setMinPosition_HW(angleRad_rct)
		 * @brief Configure the Minimum position ( works in 1 turn mode only)
		 * @note Actuator control must be disabled to change
		 * @note Configured and managed in the Dynamixel actuator
		 *
		 * @param angle the position value
		 * @return status
		 */
		actuatorError_rct setMinPosition1Turn_HW(angleRad_rct angle);

		/**
		 * @fn actuatorError_rct setMaxPosition_HW(angleRad_rct)
		 * @brief Configure the Maximum position ( works in 1 turn mode only)
		 * @note Actuator control must be disabled to change
		 * @note Configured and managed in the Dynamixel actuator
		 *
		 * @param angle the position value
		 * @return status
		 */
		actuatorError_rct setMaxPosition1Turn_HW(angleRad_rct angle);

		/**
		 * @fn actuatorError_rct setVelocityKiGain(float)
		 * @brief  Set the velocity I gain
		 *
		 * @param data gain value
		 * @return status
		 */
		actuatorError_rct setVelocityKiGain_HW(float data);

		/**
		 * @fn actuatorError_rct setVelocityKpGain_HW(float)
		 * @brief  Set the velocity I gain
		 *
		 * @param data gain value
		 * @return status
		 */
		actuatorError_rct setVelocityKpGain_HW(float data);

		/**
		 * @fn actuatorError_rct setFeedForward1Gain(float)
		 * @brief Set the feedForward #1 gain
		 *
		 * @param data gain value
		 * @return status
		 */
		actuatorError_rct setFeedForward1Gain(float data);

		/**
		 * @fn actuatorError_rct setFeedForward2Gain(float)
		 * @brief Set the feedForward #2 gain
		 *
		 * @param data gain value
		 * @return status
		 */
		actuatorError_rct setFeedForward2Gain(float data);

		/**
		 * @fn actuatorError_rct setVelocityBaseProfileAcceleration(angleRad_rct)
		 * @brief set the acceleration for the profile  /see https://emanual.robotis.com/docs/en/dxl/x/xm540-w270/#profile-acceleration

		 *
		 *	@warning, must only be used when drive mode is Velocity-based Profile
		 * @param accelValue the acceleration in rad/s2
		 * @return status
		 */
		actuatorError_rct setVelocityBaseProfileAcceleration(angleRad_rct accelValue);

		/**
		 * @fn actuatorError_rct setVelocityBaseProfileVelocity(angularVeocity_rct)
		 * @brief set the velocity for the profile  /see https://emanual.robotis.com/docs/en/dxl/x/xm540-w270/#profile-velocity
		 *
		 * @warning, must only be used when drive mode is Velocity-based Profile
		 * @param accelValue the acceleration in rad/s
		 * @return status
		 */
		actuatorError_rct setVelocityBaseProfileVelocity(angularVeocity_rct velocity);

		/**
	     * @fn actuatorError_rct setTimeBaseProfileAcceleration(uint32_t)
		 * @brief Se the acceleration for  time base profile.
		 *
		 * @note  untested / needs to be tested / completed
		 * @param time_ms
		 *
		 * @return status
		 */
		actuatorError_rct setTimeBaseProfileAcceleration(uint32_t time_ms);

		/**
		 * @fn actuatorError_rct setTimeBaseProfileVelocity(angularVeocity_rct)
		 * @brief
		 *
		 * @note  untested / needs to be tested / completed
		 * @param velocity
		 *
		 * @return status
		 */
		actuatorError_rct setTimeBaseProfileVelocity(angularVeocity_rct velocity);


		/// Dynamixel Core functions and data management functions

		/**
		 * @fn actuatorError_rct writeDxlData(uint16_t, uint8_t*)
		 * @brief Core fucntion to write to the actuator. Usefull to acces not implemented functions
		 *
		 * @param dataAddress EEprom/ram address see XM540_ ... _ADDR in dynamixel_mx540address.h
		 * @param data	4 bytes of data ( if data has a lenght of 2, package in in a 4 byte array)
		 * @return	error status
		 */
		actuatorError_rct writeDxlData(uint16_t dataAddress,uint8_t data[4]);

		/**
		 * @fn uint16_t getDataToSend(uint16_t&, uint16_t&, uint8_t*)
		 * @brief I data to send in buffer, return the data in an array with a specific format
		 *
		 * @param dataAddress	Memory address of the data
		 * @param dataLen		Lenght of the data
		 * @param dataBuffer	The data ( 1 to 4 bytes) bu a 4 bytes buffer always used
		 *
		 * @return data length
		 */
		uint16_t getDataToSend(uint16_t &dataAddress, uint16_t &dataLen, uint8_t *dataBuffer);

		/**
		 * @fn int getOutBufferDataCount()
		 * @brief Get the ni=umber of data in the output buffer
		 *
		 * @return the number of data
		 */
		int getOutBufferDataCount();

		/**
		 * @fn actuatorError_rct sendCommandRequest()
		 * @brief Package a move command to be sent to the actuator.
		 *
		 * 	The type of data packaged depend on the current mode configured with
		 * 	setControlMode() function \see operatingMode_t
		 *
		 * @return status
		 */
		actuatorError_rct sendCommandRequest();	//testé


	private:

		//uartManager *m_uartManager;

		float m_kp = 0;
		float m_ki = 0;
		float m_kd = 0;

		float m_velocity_kp = 0;
		float m_velocity_ki = 0;

		bool m_ledStatus = false;

		angularVeocity_rct m_angularVelocityCalc;

		CircularBuffer m_outDataCircularBuffer;
		uint8_t m_canOutDataBuffer[TX_BUFFER_SIZE]; // 8 messages max buffer

};

#endif /* TEAMATLIBS_ACTUATORTMOTORSAK_H_ */
