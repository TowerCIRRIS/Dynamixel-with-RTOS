/**
 * @file robocore_actuator.cpp
 * @brief Classe de d'actionneur de base pour robotCore
 * @version 1.2
 *
 * Version 1.1 2022-01-10
 * 	-Ajout orientation configurable
 * Version 1,2 2022-04-14
 *	modification suite a l'ajout de dynamixel
 *	correction bug Data safety flag mélangé avec dara ready flag
 */



#include  "robocore_actuator.h"

//------------------------------------------------------------------------//
// Actuator Command Get functions
//------------------------------------------------------------------------//
angularVeocity_rct robocore_actuator::getRxAngularVelocity(){

	clearDataReadyFlag(DRF_ANGULAR_VELOCITY_RX);	// Clear the data ready flag when read

	return m_angularVelocityRx * info.motorOrientation;
}

angleRad_rct robocore_actuator::getAngle(){

	clearDataReadyFlag(DRF_ANGLE_RX);	// Clear the data ready flag when read

	return m_angleRx * info.motorOrientation;
}


float robocore_actuator::getCurrent(){

	clearDataReadyFlag(DRF_CURRENT_RX);	// Clear the data ready flag when read

	return m_currentRx * info.motorOrientation;
}

float robocore_actuator::getVoltage(){

	clearDataReadyFlag(DRF_VOLTAGE_RX);	// Clear the data ready flag when read

	return m_voltageRx;
}

float robocore_actuator::getTorque(){

	clearDataReadyFlag(DRF_TORQUE_RX);	// Clear the data ready flag when read

	return m_torqueRx * info.motorOrientation;
}

float robocore_actuator::getTemperature(){

	clearDataReadyFlag(DRF_TEMPERATURE_RX);	// Clear the data ready flag when read

	return m_temperatureRx ;
}

float robocore_actuator::getPwm(){

	clearDataReadyFlag(DRF_PWM_RX);	// Clear the data ready flag when read

	return m_pwmRx * info.motorOrientation;
}

//------------------------------------------------------------------------//
// Actuator Command Set functions
//------------------------------------------------------------------------//



actuatorError_rct robocore_actuator::setAngularVelocityCommand(angularVeocity_rct data){
 // Set Received velocity from actuator

	if(m_controlMode == operatingMode_t::CONTROL_MODE_VELOCITY_ANGULAR)
	{
		m_angularVelocityCommand = data * info.motorOrientation;// / info.gearRatio; // TODO valider

		setDataReadyFlag(DRF_ANGULAR_VELOCITY_COMMAND);
		return ERROR_ROBOCORE_ACTUATOR_NONE;
	}

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE;
}

actuatorError_rct robocore_actuator::setAngleCommand(angleRad_rct data)
{

	if(m_controlMode == operatingMode_t::CONTROL_MODE_POSITION_ANGULAR ||
		m_controlMode == operatingMode_t::CONTROL_MODE_POSITION_TORQUE_ANGULAR )
	{
		m_angleCommand = data.rad() * info.gearRatio * info.motorOrientation;// Only works for rotary actuators

		setDataReadyFlag(DRF_ANGLE_COMMAND);
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
	}

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE;
}

actuatorError_rct robocore_actuator::setTorqueCommand(float data)
{

	if(m_controlMode == operatingMode_t::CONTROL_MODE_TORQUE ||
			m_controlMode == operatingMode_t::CONTROL_MODE_POSITION_TORQUE_ANGULAR )
	{
		m_torqueCommand = data * info.motorOrientation;

		setDataReadyFlag(DRF_TORQUE_COMMAND);
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
	}

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE;
}

actuatorError_rct robocore_actuator::setCurrentCommand(float current_mA)
{
	if(m_controlMode == operatingMode_t::CONTROL_MODE_CURRENT)
	{
		m_currentCommand_mA = current_mA * info.motorOrientation;

		setDataReadyFlag(DRF_CURRENT_COMMAND);
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
	}

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE;

}

actuatorError_rct robocore_actuator::setLinearVelocityCommand(float data){

	if(m_controlMode == operatingMode_t::CONTROL_MODE_VELOCITY_LINEAR)
	{
		m_linearVelocityCommand = data;
		setDataReadyFlag(DRF_LINEAR_VELOCITY_COMMAND);
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
	}

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE;
}

actuatorError_rct robocore_actuator::setLinearPositionCommand(float data){

	if(m_controlMode == operatingMode_t::CONTROL_MODE_POSITION_LINEAR)
	{
		m_linearPositionCommand = data;
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
		setDataReadyFlag(DRF_LINEAR_POSITION_COMMAND);
	}

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE;
}

actuatorError_rct robocore_actuator::setPwmCommand(float pwm)
{
	if(m_controlMode == operatingMode_t::CONTROL_MODE_PWM)
	{
		if(pwm > 100.0)
		{
			pwm = 100.0;
		}
		if(pwm < -100.0)
		{
			pwm = -100.0;
		}

		m_pwmCommand = pwm * info.motorOrientation;
		setDataReadyFlag(DRF_PWM_COMMAND);
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
	}

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE;
}


