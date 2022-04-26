/**
 * @file actuator_DynamixelXM.cpp
 * @brief actuator_DynamixelXM
 * @version 1.0
 *
 * Version 1.01 2022-04-14
 * 	-Première version
 */

#include <TeamAt_H_Dynamixel/actuator_DynamixelXM.h>


actuator_DynamixelXM::actuator_DynamixelXM() {

	m_outDataCircularBuffer.init(m_canOutDataBuffer, TX_BUFFER_SIZE, CircularBufferMode::MODE_FIXED_LENGHT, TX_DATA_SIZE);

	m_kd = 0;
	m_kp = 0;
	m_angleCommand = 0;
	m_angularVelocityCommand = 0;
	m_torqueCommand = 0;

}

actuator_DynamixelXM::~actuator_DynamixelXM() {

}



//------------------------------------------------------------------------//
// Actuator managements functions
//------------------------------------------------------------------------//

actuatorError_rct actuator_DynamixelXM::enableControl()
{
	m_ConstrolEnableStatus = true;

	uint8_t dxlData;

	dxlData = XM540_Torque_Enable_ON;
	return writeDxlData(XM540_Torque_Enable_ADDR,(uint8_t*)&dxlData);
}

actuatorError_rct actuator_DynamixelXM::disableControl()
{
	m_ConstrolEnableStatus = false;

	uint8_t dxlData;

	dxlData = XM540_Torque_Enable_OFF;
	return writeDxlData(XM540_Torque_Enable_ADDR,(uint8_t*)&dxlData);
}

actuatorError_rct actuator_DynamixelXM::setZeroPosition()
{
	actuatorError_rct errorStatus;

	angleRad_rct offsetAngle;
	offsetAngle = getAngle();
	errorStatus = setOffsetAngle_HW(offsetAngle);

	this->setAngleCommand(0);

	return errorStatus;
}



//------------------------------------------------------------------------//
// Actuator Config Set functions
//------------------------------------------------------------------------//
actuatorError_rct actuator_DynamixelXM::setKpGain(float data)
{

	if(data < XM540_POSITION_KP_MIN || data > XM540_POSITION_KP_MAX)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	m_kp = data;

	uint32_t intKp = (uint32_t)data;

	return writeDxlData(XM540_Position_P_Gain_ADDR,(uint8_t*)&intKp);

}

actuatorError_rct actuator_DynamixelXM::setKiGain(float data)
{
	if(data < XM540_POSITION_KI_MIN || data > XM540_POSITION_KI_MAX)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	m_ki = data;

	uint32_t intKi = (uint32_t)data;

	return writeDxlData(XM540_Position_I_Gain_ADDR,(uint8_t*)&intKi);

}

actuatorError_rct actuator_DynamixelXM::setKdGain(float data)
{
	if(data < XM540_POSITION_KD_MIN || data > XM540_POSITION_KD_MAX)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	m_kd = data;

	uint32_t intKd = (uint32_t)data;

	return writeDxlData(XM540_Position_D_Gain_ADDR,(uint8_t*)&intKd);

}

actuatorError_rct actuator_DynamixelXM::setVelocityKiGain_HW(float data)
{
	if(data < XM540_VELOCITY_KI_MIN || data > XM540_VELOCITY_KI_MAX)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	m_velocity_ki = data;

	uint32_t gain = (uint32_t)data;

	return writeDxlData(XM540_Velocity_I_Gain_ADDR,(uint8_t*)&gain);
}

actuatorError_rct actuator_DynamixelXM::setVelocityKpGain_HW(float data)
{
	if(data < XM540_VELOCITY_KP_MIN || data > XM540_VELOCITY_KP_MAX)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	m_velocity_kp = data;

	uint32_t gain = (uint32_t)data;

	return writeDxlData(XM540_Velocity_P_Gain_ADDR,(uint8_t*)&gain);
}

actuatorError_rct actuator_DynamixelXM::setFeedForward1Gain(float data)
{
	if(data < XM540_FF1_GAIN_MIN || data > XM540_FF1_GAIN_MAX)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	uint32_t gain = (uint32_t)data;

	return writeDxlData(XM540_Feedforward_1st_Gain_ADDR,(uint8_t*)&gain);
}

actuatorError_rct actuator_DynamixelXM::setFeedForward2Gain(float data)
{
	if(data < XM540_FF2_GAIN_MIN || data > XM540_FF2_GAIN_MAX)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	uint32_t gain = (uint32_t)data;

	return writeDxlData(XM540_Feedforward_2nd_Gain_ADDR,(uint8_t*)&gain);
}

actuatorError_rct actuator_DynamixelXM::setVelocityBaseProfileAcceleration(angleRad_rct accelValue)
{

		float fData =accelValue.rad()*572.958;//rad/s2 to rpm2

		fData = fData / 214.577; //dx units

		int dxlData_int;

		dxlData_int = (int)(fData);

		return writeDxlData(XM540_Profile_Acceleration_ADDR,(uint8_t*)&dxlData_int);
}

actuatorError_rct actuator_DynamixelXM::setVelocityBaseProfileVelocity(angularVeocity_rct velocity)
{
	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	float fData =velocity.rad()* 9.549296585514; // rad to  rpm

	if(fData < 0.0)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	fData = fData / 0.229;
	int dxlData_int;


	dxlData_int = (int)(fData);

	return writeDxlData(XM540_Profile_Velocity_ADDR,(uint8_t*)&dxlData_int);
}



actuatorError_rct actuator_DynamixelXM::setTimeBaseProfileAcceleration(uint32_t time_ms)
{
	uint32_t dxlData_int;

	dxlData_int = time_ms;

	return writeDxlData(XM540_Profile_Acceleration_ADDR,(uint8_t*)&dxlData_int);
}

actuatorError_rct actuator_DynamixelXM::setTimeBaseProfileVelocity(angularVeocity_rct velocity)
{
	float fData =velocity.rad()* 9.549296585514; // rad to  rpm

	if(fData < 0.0)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	fData = fData / 0.229;
	int dxlData_int;


	dxlData_int = (int)(fData);

	return writeDxlData(XM540_Profile_Velocity_ADDR,(uint8_t*)&dxlData_int);

}

//------------------------------------------------------------------------//
// Actuator Calculated data Set functions
//------------------------------------------------------------------------//
void actuator_DynamixelXM::setCalcAngularVelocity(angularVeocity_rct data){

	m_angularVelocityCalc = data;
}

//------------------------------------------------------------------------//
// Actuator Data exchange functions
//------------------------------------------------------------------------//

actuatorError_rct actuator_DynamixelXM::sendCommandRequest()
{
	uint8_t dataSend[4];
	int dxlData_int;

	float fData;

	if(m_ConstrolEnableStatus)
	{
		switch(m_controlMode)
		{
			case operatingMode_t::CONTROL_MODE_POSITION_ANGULAR:

				fData = m_angleCommand.degConvert() /0.088;
				dxlData_int = (int)(fData);

				writeDxlData(XM540_Goal_Position_ADDR,(uint8_t*)&dxlData_int);

				clearDataReadyFlag(DRF_ANGLE_COMMAND);

				break;

			case operatingMode_t::CONTROL_MODE_VELOCITY_ANGULAR:

				dxlData_int = (int)((m_angularVelocityCommand.degConvert() * 0.1666666)/0.229); //  convert deg/s to RPM then to .229rpm/unit

				writeDxlData(XM540_Goal_Velocity_ADDR,(uint8_t*)&dxlData_int);

				clearDataReadyFlag(DRF_ANGULAR_VELOCITY_COMMAND);

				break;

			case CONTROL_MODE_CURRENT:

				//Unit	Value Range
				//about 2.69[mA]	0 ~ 2,047

				fData = m_currentCommand_mA / 2.69;//m_angleCommand.degConvert()/0.088;
				dxlData_int = (int)(fData);

				dataSend[0] = ((uint8_t*)&dxlData_int)[0];
				dataSend[1] = ((uint8_t*)&dxlData_int)[1];
				dataSend[2] = 0;
				dataSend[3] = 0;
				writeDxlData(XM540_Goal_Current_ADDR,dataSend);

				clearDataReadyFlag(DRF_CURRENT_COMMAND);

				break;

			case CONTROL_MODE_PWM:

				fData = m_pwmCommand / 0.113 ;
				dxlData_int = (int)(fData);

				dataSend[0] = ((uint8_t*)&dxlData_int)[0];
				dataSend[1] = ((uint8_t*)&dxlData_int)[1];
				dataSend[2] = 0;
				dataSend[3] = 0;
				writeDxlData(XM540_Goal_PWM_ADDR,dataSend);

				clearDataReadyFlag(DRF_PWM_COMMAND);

				break;

			default:
				return ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE;
				break;

		}
	}

	return ERROR_ROBOCORE_ACTUATOR_NOT_ENABLED;
}

actuatorError_rct actuator_DynamixelXM::setOperatingMode(operatingMode_t controlMode)
{
	m_controlMode = controlMode;
	uint32_t dxlData;

	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}
	switch(m_controlMode)
	{

		case CONTROL_MODE_POSITION_ANGULAR:

			dxlData = XM540_OPERATING_MODE_POSITION_MULTITURN;

			break;

		case CONTROL_MODE_VELOCITY_ANGULAR:
			dxlData = XM540_OPERATING_MODE_VELOCITY;
			break;



		case CONTROL_MODE_CURRENT:
			dxlData = XM540_OPERATING_MODE_CURRENT;
			break;

		case CONTROL_MODE_PWM:
			dxlData = XM540_OPERATING_MODE_PWM;
			break;

		case CONTROL_MODE_POSITION_TORQUE_ANGULAR:
			dxlData = XM540_OPERATING_MODE_POSITION_CURRENT;
			break;

		case CONTROL_MODE_UNDEFINED:
		case CONTROL_MODE_TORQUE:
		case CONTROL_MODE_POSITION_LINEAR:
		case CONTROL_MODE_VELOCITY_LINEAR:

			return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_UNDEFINED;
			break;
	}

	writeDxlData(XM540_OPERATING_MODE_ADDR,(uint8_t*)&dxlData);

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
}


actuatorError_rct actuator_DynamixelXM::setLed(bool ledState)
{
	m_ledStatus = ledState;

	uint8_t dxlData[4];

	if(ledState)
	{
		dxlData[0] = XM540_LED_ON;
	}
	else
	{
		dxlData[0] = XM540_LED_OFF;
	}

	return writeDxlData(XM540_LED_ADDR,dxlData);

}

actuatorError_rct actuator_DynamixelXM::sendIdChangeMessage(uint8_t id)
{
	uint8_t dxlData[4];

	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	dxlData[0] = id;
	dxlData[1] = 0;
	dxlData[2] = 0;
	dxlData[3] = 0;

	return writeDxlData(XM540_ID_ADDR,dxlData);

}

actuatorError_rct actuator_DynamixelXM::setBaudRate(dynamixelBaudrate_t baud)
{
	uint8_t dxlData[4];

	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	dxlData[0] = (uint8_t)baud;
	dxlData[1] = 0;
	dxlData[2] = 0;
	dxlData[3] = 0;

	return writeDxlData(XM540_BAUD_RATE_ADDR,dxlData);
}


actuatorError_rct actuator_DynamixelXM::setDriveMode(uint8_t dMode)
{

	uint8_t dxlData[4];

	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	dxlData[0] = dMode;
	dxlData[1] = 0;
	dxlData[2] = 0;
	dxlData[3] = 0;

	return writeDxlData(XM540_DRIVE_MODE_ADDR,dxlData);
}


actuatorError_rct actuator_DynamixelXM::setOffsetAngle_HW(angleRad_rct offsetAngle)
{

	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	float fData;
	int dxlData_int;


	fData = offsetAngle.degConvert()/0.088;
	dxlData_int = (int)(fData);

	return writeDxlData(XM540_HOMING_OFFSET_ADDR,(uint8_t*)&dxlData_int);

}

actuatorError_rct actuator_DynamixelXM::setMovingTreshold_HW(angularVeocity_rct velocity)
{
	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	float fData =velocity.degConvert()*0.16667; // rotation in rpm

	if(fData < 0.0)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	fData = fData / 0.229;
	int dxlData_int;


	dxlData_int = (int)(fData);

	return writeDxlData(XM540_MOVING_THRESHOLD_ADDR,(uint8_t*)&dxlData_int);
}

actuatorError_rct actuator_DynamixelXM::setCurrentLimit_HW(float current_mA)
{
	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	if(current_mA < 0.0)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	float fData = current_mA/2.69;
	int dxlData_int;


	dxlData_int = (int)(fData);

	return writeDxlData(XM540_CURRENT_LIMIT_ADDR,(uint8_t*)&dxlData_int);
}

actuatorError_rct actuator_DynamixelXM::setVelocityLimit_HW(angularVeocity_rct velocity)
{

	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	float fData =velocity.degConvert()*0.16667; // rotation in rpm

	if(fData < 0.0)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER;
	}

	fData = fData / 0.229;
	int dxlData_int;


	dxlData_int = (int)(fData);

	return writeDxlData(XM540_VELOCITY_LIMIT_ADDR,(uint8_t*)&dxlData_int);
}

actuatorError_rct actuator_DynamixelXM::setPwmLimit_HW(float pwm)
{
	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	if(pwm < 0.0)
	{
		pwm = 0.0;
	}

	if(pwm > 100.0)
	{
		pwm = 100.0;
	}

	float fData = pwm/0.113;
	int dxlData_int;

	dxlData_int = (int)(fData);

	return writeDxlData(XM540_PWM_LIMIT_ADDR,(uint8_t*)&dxlData_int);
}

actuatorError_rct actuator_DynamixelXM::setMinPosition1Turn_HW(angleRad_rct angle)
{

	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	float fData =angle.degConvert() / 0.088;

	int dxlData_int;

	dxlData_int = (int)(fData);

	return writeDxlData(XM540_MIN_POSITION_LIMIT_ADDR,(uint8_t*)&dxlData_int);
}

actuatorError_rct actuator_DynamixelXM::setMaxPosition1Turn_HW(angleRad_rct angle)
{

	if(m_ConstrolEnableStatus)
	{
		return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_BUSY;
	}

	float fData =angle.degConvert() / 0.088;

	int dxlData_int;

	dxlData_int = (int)(fData);

	return writeDxlData(XM540_MAX_POSITION_LIMIT_ADDR,(uint8_t*)&dxlData_int);
}



int actuator_DynamixelXM::getOutBufferDataCount()
{
	return (m_outDataCircularBuffer.getUsedSpace()/TX_DATA_SIZE);
}

uint8_t actuator_DynamixelXM::getId()
{
	return (uint8_t)(info.ProductID & 0x000000FF);
}


uint16_t actuator_DynamixelXM::getDataToSend(uint16_t &dataAddress, uint16_t &dataLen, uint8_t *dataBuffer)
{

	uint8_t dataSend[6];

	if (m_outDataCircularBuffer.readBytes(dataSend))
	{
		dataAddress = *(uint16_t*)&dataSend[0];

		for(int i = 0; i < 4; i++)
		{
			dataBuffer[i] = dataSend[2+i];
		}

		switch(dataAddress)
		{
			case XM540_FIRMWARE_VERSION_ADDR:
			case XM540_Torque_Enable_ADDR:
			case XM540_LED_ADDR:
			case XM540_ID_ADDR:
			case XM540_BAUD_RATE_ADDR:
			case XM540_RETURN_DELAY_TIME_ADDR:
			case XM540_DRIVE_MODE_ADDR:
			case XM540_OPERATING_MODE_ADDR:
			case XM540_SECONDARY_SHADOW_ID_ADDR:
			case XM540_PROTOCOL_TYPE_ADDR:
			case XM540_TEMPERATURE_LIMIT_ADDR:
			case XM540_EXTERNAL_PORT_MODE1_ADDR:
			case XM540_EXTERNAL_PORT_MODE2_ADDR:
			case XM540_EXTERNAL_PORT_MODE3_ADDR:
			case XM540_STARTUP_CONFIGURATION_ADDR:
			case XM540_SHUTDOWN_ADDR:
			case XM540_Status_Return_Level_ADDR:
			case XM540_Bus_Watchdog_ADDR:
			case XM540_Indirect_Data_1_ADDR:
			case XM540_Indirect_Data_2_ADDR:
			case XM540_Indirect_Data_3_ADDR:
			case XM540_Indirect_Data_26_ADDR:
			case XM540_Indirect_Data_27_ADDR:
			case XM540_Indirect_Data_28_ADDR:
			case XM540_Indirect_Data_29_ADDR:
			case XM540_Indirect_Data_30_ADDR:
			case XM540_Indirect_Data_31_ADDR:
			case XM540_Indirect_Data_54_ADDR:
			case XM540_Indirect_Data_55_ADDR:
			case XM540_Indirect_Data_56_ADDR:

				dataLen = 1;
				break;

			case XM540_MODEL_NUMBER_ADDR:
			case XM540_MAX_VOLTAGE_LIMIT_ADDR:
			case XM540_MIN_VOLTAGE_LIMIT_ADDR:
			case XM540_PWM_LIMIT_ADDR:
			case XM540_CURRENT_LIMIT_ADDR:
			case XM540_Velocity_I_Gain_ADDR:
			case XM540_Velocity_P_Gain_ADDR:
			case XM540_Position_D_Gain_ADDR:
			case XM540_Position_I_Gain_ADDR:
			case XM540_Position_P_Gain_ADDR:
			case XM540_Feedforward_2nd_Gain_ADDR:
			case XM540_Feedforward_1st_Gain_ADDR:
			case XM540_Goal_PWM_ADDR :
			case XM540_Goal_Current_ADDR :
			case XM540_External_Port_Data_1_ADDR:
			case XM540_External_Port_Data_2_ADDR:
			case XM540_External_Port_Data_3_ADDR:
			case XM540_Indirect_Address_1_ADDR:
			case XM540_Indirect_Address_2_ADDR:
			case XM540_Indirect_Address_3_ADDR:
			case XM540_Indirect_Address_26_ADDR:
			case XM540_Indirect_Address_27_ADDR:
			case XM540_Indirect_Address_28_ADDR:
			case XM540_Indirect_Address_29_ADDR:
			case XM540_Indirect_Address_30_ADDR:
			case XM540_Indirect_Address_31_ADDR:
			case XM540_Indirect_Address_54_ADDR:
			case XM540_Indirect_Address_55_ADDR:
			case XM540_Indirect_Address_56_ADDR:

				dataLen = 2;
				break;

			case XM540_MODEL_INFORMATION_ADDR:
			case XM540_HOMING_OFFSET_ADDR :
			case XM540_MOVING_THRESHOLD_ADDR:
			case XM540_VELOCITY_LIMIT_ADDR:
			case XM540_MAX_POSITION_LIMIT_ADDR:
			case XM540_MIN_POSITION_LIMIT_ADDR:
			case XM540_Goal_Velocity_ADDR :
			case XM540_Profile_Acceleration_ADDR:
			case XM540_Profile_Velocity_ADDR:
			case XM540_Goal_Position_ADDR :


				dataLen = 4;
				break;

			default:
				dataLen = 0;
				break;
		}
	}
	else
	{
		dataLen = 0;
	}


	return dataLen;
}


actuatorError_rct actuator_DynamixelXM::writeDxlData(uint16_t dataAddress,uint8_t data[4])
{
	uint8_t dataSend[6];
	cbError_t errorStatus ;

	dataSend[0] = ((uint8_t)(((uint64_t)(dataAddress)) & 0xff));
	dataSend[1] = ((uint8_t)((((uint64_t)(dataAddress)) >> 8) & 0xff));
	dataSend[2] = data[0];
	dataSend[3] = data[1];
	dataSend[4] = data[2];
	dataSend[5] = data[3];

	errorStatus = m_outDataCircularBuffer.writeBytes(dataSend); // Add messages to buffer
	if(errorStatus)
	{
		return  (actuatorError_rct)((int)ERROR_ROBOCORE_ACTUATOR_CIRCULAR_BUFFER + (int)errorStatus);
	}

	return actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
}
