/**
 * @file dynamixelSequencer.cpp
 * @brief
 * @version 1.0
 *
 * Version 1.01 2022-04-14
 * 	-Première version
 */

#include <main.h>
#include "dynamixelSequencer.h"

#include "TeamAT_B_Robocore/robocore_actuator.h"
#include "TeamAT_B_Robocore/roboCoreTypes.h"
#include <stm32l4xx_hal_gpio.h>
#include <sys/_stdint.h>
#include <TeamAT_B_Basic/teamATbasic_V1_1.h>
#include <TeamAt_H_Dynamixel/actuator_DynamixelXM.h>
#include <TeamAt_H_Dynamixel/dynamixel_xm540address.h>
#include <TeamAt_H_Dynamixel/dynamixelProtocolV2.h>
#include <uartApp.h>
#include <TeamAT_B_UartManager/uartManager_V1_0.h>


dynamixelSequencer::dynamixelSequencer (int refreshTime_ms)
{
	m_readPeriodTimeMs = refreshTime_ms;
}

dynamixelSequencer::~dynamixelSequencer ()
{

}

void dynamixelSequencer::setReadRefreshPeriode(int refreshTime_ms)
{
	m_readPeriodTimeMs = refreshTime_ms;
}

void dynamixelSequencer::start()
{
	m_SequencerState = comState_idle;
	m_dataState = dataState_read;
}

void dynamixelSequencer::stop()
{
	m_SequencerState = comState_disable;
}


int dynamixelSequencer::addActuator(actuator_DynamixelXM *actuator)
{

	if(m_NbActuators >= NB_ACTUATOR_MAX)
	{
		return ERROR_DYNAMIXELSEQUENCER_NO_SPACE;
	}

	m_actuatorList[m_NbActuators] = actuator;
	m_idList[m_NbActuators] = actuator->getId();
	m_NbActuators++;

	return ERROR_DYNAMIXELSEQUENCER_SUCCESS;
}

int dynamixelSequencer::handle()
{
		bool dataToSend = false;
		bool dataPending = false;
		bool dataToRead = false;

		switch(m_SequencerState)
		{
			case comState_waitingReply:

				//Si réponse reçueif()
				if(0 == g_commInProgress_expectedRxDataLen)
				{
					// wait tx done before going back to idle
					if (!serial3.txInProgress())
					{
						actuatorSelect = 0;
						m_SequencerState = comState_idle;
					}
					else
					{
						break;
					}
				}
				else if(serial3.available() >= g_commInProgress_expectedRxDataLen)
				{
					dataToRead = true;
					while(dataToRead){

						serial3.read(dynamixelRxBuffer, g_commInProgress_expectedRxDataLen);
						if (m_dataState == dataState_read){

							dataCommReceiveFastSync();

							for(int i = 0 ;i < m_NbActuators; i++)
							{
								if(m_ackPending[i] == 1)
								{
									m_actuatorList[i]->setSafetyFlag(SAFETY_COMM_MISSING_DATA_ERROR);
								}
							}

								//Received all data
								actuatorSelect = 0;
								m_SequencerState = comState_idle;
								m_dataState = dataState_writeControl; // if there is still data in buffer then it will be control data
								HAL_GPIO_WritePin(EXP_CS_1_GPIO_Port, EXP_CS_1_Pin, GPIO_PIN_RESET);

						}else if(m_dataState == dataState_writeControl){


							writeControlReceive();

							dataPending = false;
							for(int i = 0 ;i < m_NbActuators; i++)
							{
								if(m_ackPending[i])
								{
									dataPending = true;
								}
							}

							if(false == dataPending)
							{
								//Received all data
								m_dataState = dataState_read;
								m_SequencerState = comState_idle;
								HAL_GPIO_WritePin(EXP_CS_1_GPIO_Port, EXP_CS_1_Pin, GPIO_PIN_RESET);
							}
						}

						dataToRead = false;
					}

					if(m_SequencerState != comState_idle)
					{
						break; // do not continue automatically to next state, more data to receive.
					}

				}
				else
				{
					//Si timeout
					uint32_t tick = timeElapsed(atGetSysTick_ms(), m_sendTime);

					if(tick > m_replyTimeoutMs)
					{
						//Todo gestion logique/erreurs timeout.
						//volatile int dataAvailable = serial3.available(); //TODO pas sur que c'est utilisé, à valider

						for(int i = 0; i < m_NbActuators; i++)
						{
							if(m_ackPending[i])
							{
								m_actuatorList[i]->setSafetyFlag(SAFETY_COMM_TIMEOUT_ERROR);
							}
						}

						m_SequencerState = comState_idle;

					}
					else
					{
						break; // si pas de réponses, on reste en attente jusqu'au timeout
					}
				}

			case comState_idle:

				if ( (m_dataState!= dataState_writeControl) && dataReadRequired())
				{
					g_lastReadStart = atGetSysTick_ms();
					m_dataState = dataState_read;
					m_SequencerState = comState_sending; // for nex re-entry

					// Package control send for when data read is done
					for(int i = 0 ; i < m_NbActuators; i++)
					{
						m_actuatorList[i]->sendCommandRequest();
					}

				}
				else
				{
					for(int i = 0 ; i < m_NbActuators; i++)
					{
						if(m_actuatorList[i]->getOutBufferDataCount() > 0){
							dataToSend = true;
						}
					}

					if(dataToSend)
					{
						m_dataState = dataState_writeControl;
						m_SequencerState = comState_sending; // for nex re-entry
					}
					else
					{
						m_dataState = dataState_read;
						break;
					}
				}

			case comState_sending:


				switch(m_dataState)
				{
					case dataState_read:

						//dataRequestSend();
						dataRequestFastSend();
						break;

					case dataState_writeControl:

						bulkWriteSend();
						break;
				};

				m_sendTime = atGetSysTick_ms();
				//HAL_GPIO_WritePin(EXP_CS_1_GPIO_Port, EXP_CS_1_Pin, GPIO_PIN_SET);
				m_SequencerState = comState_waitingReply;

				break;

			case comState_disable:

				break;
		}

		return m_SequencerState;
}

 bool dynamixelSequencer::dataReadRequired()
 {
	uint32_t time = atGetSysTick_ms();
 	uint32_t tick = timeElapsed(time, g_lastReadStart);

 	return ( tick >= m_readPeriodTimeMs);
 }


// void dynamixelSequencer::manageDynamixelErrors(uint8_t errorStatus)
// {
//	 //TODO
// }



// void dynamixelSequencer::dataRequestSend()
// {
//
//	 uint16_t dataLen =  XM540_Present_PWM_LEN + XM540_Present_Current_LEN +
//             XM540_Present_Velocity_LEN + XM540_Present_Position_LEN +
//             XM540_Velocity_Trajectory_LEN + XM540_Position_Trajectory_LEN +
//             XM540_Present_Input_Voltage_LEN + XM540_Present_Temperature_LEN;
//
//	int packetLen = packageSyncReadInstruction(dynamixelTxBuffer, m_idList,m_NbActuators,
//	                                           XM540_Present_PWM_ADDR,dataLen);
//
//	g_commInProgress_expectedRxDataLen = PKT_STATUS_OVERHEAD_LEN + dataLen; // voltage(2 bytes) + temperature(1 byte)
//	//g_commInProgress_xpectedActuatorId = m_actuatorList[actuatorSelect]->getId();
//
//	for(int i = 0 ; i < m_NbActuators ; i++)
//	{
//		m_ackPending[i] = true;
//	}
//
//	serial3.stopRxInterrupt();
//
//	serial3.flush();
//	serial3.write(dynamixelTxBuffer, packetLen);
//	if(g_commInProgress_expectedRxDataLen > 0)
//	{
//		serial3.startRxInterrupt(g_commInProgress_expectedRxDataLen);
//	}
//
// }

 void dynamixelSequencer::dataRequestFastSend()
 {

	 uint16_t dataLen =  XM540_Present_PWM_LEN + XM540_Present_Current_LEN +
             XM540_Present_Velocity_LEN + XM540_Present_Position_LEN +
             XM540_Velocity_Trajectory_LEN + XM540_Position_Trajectory_LEN +
             XM540_Present_Input_Voltage_LEN + XM540_Present_Temperature_LEN;

	int packetLen = packageFastSyncReadInstruction(dynamixelTxBuffer, m_idList,m_NbActuators,
	                                           XM540_Present_PWM_ADDR,dataLen);

	g_commInProgress_expectedRxDataLen = PKT_STATUS_OVERHEAD_LEN  + ((m_NbActuators-1)*3) + ((dataLen + PKT_ID_LEN) * m_NbActuators); // Overhead du premier actionneur + mini overhead  des autres actionneurs + data de tous les actionneurs
	//g_commInProgress_xpectedActuatorId = m_actuatorList[actuatorSelect]->getId();

	for(int i = 0 ; i < m_NbActuators ; i++)
	{
		m_ackPending[i] = true;
	}

	serial3.stopRxInterrupt();

	serial3.flush();
	serial3.write(dynamixelTxBuffer, packetLen);
	if(g_commInProgress_expectedRxDataLen > 0)
	{
		serial3.startRxInterrupt(g_commInProgress_expectedRxDataLen);
	}

 }

 void dynamixelSequencer::bulkWriteSend()
 {
	int writePtr;
	uint16_t dataLen;
	uint16_t writeStartAddress;
	uint8_t dataBuffer[8];

	writePtr = bulkWriteInstructionPackage_start(dynamixelTxBuffer);

	for(int i=0 ; i< m_NbActuators; i++)
	{

		if (m_actuatorList[i]->getDataToSend(writeStartAddress, dataLen, dataBuffer))
		{

			writePtr = bulkWriteInstructionPackage_AddData(dynamixelTxBuffer, writePtr, m_actuatorList[i]->getId(),
																writeStartAddress, dataLen, dataBuffer);

			m_ackPending[i] = 0; // TODO géré data qui exige une réponse
		}
		else
		{
			m_ackPending[i] = 0;
		}

	}

	writePtr =  bulkWriteInstructionPackage_complete(dynamixelTxBuffer, writePtr);

	g_commInProgress_expectedRxDataLen = 0; // Pas de reply pour les message bulk

	serial3.stopRxInterrupt();
	serial3.flush();
	serial3.write(dynamixelTxBuffer, writePtr);

	if(g_commInProgress_expectedRxDataLen > 0)
	{
		serial3.startRxInterrupt(g_commInProgress_expectedRxDataLen);
	}
 }


// uint8_t dynamixelSequencer::dataCommReceive(uint8_t actuatorSelect)
// {
//
//	if(validStatusMessage(dynamixelRxBuffer))
//	{
//		for(int i = 0 ; i < m_NbActuators; i++)
//		{
//			if(m_actuatorList[i]->getId() == dynamixelRxBuffer[PKT_ID])
//			{
//
//				HAL_GPIO_WritePin(OUT_DEBUG_2_PORT, OUT_DEBUG_2_PIN, GPIO_PIN_SET);
//				int16_t pwm = DXL_MAKEWORD(dynamixelRxBuffer[PKT_ERROR+1],dynamixelRxBuffer[PKT_ERROR+2]);
//				float fPwm = (float)pwm * 0.113;
//				int16_t intCurrent = DXL_MAKEWORD(dynamixelRxBuffer[PKT_ERROR+3],dynamixelRxBuffer[PKT_ERROR+4]);
//				//uint16_t int16Current
//				float currentmA = (float)(intCurrent) * 2.69;
//				int32_t intVelocity = *(int32_t*)&dynamixelRxBuffer[PKT_ERROR+5];
//				float velociy = (float)intVelocity * 0.229;
//				angularVeocity_rct angVelocity;
//				angVelocity.setFromDegrees(velociy);
//
//				int32_t intPosition = *(int32_t*)&dynamixelRxBuffer[PKT_ERROR+9];
//				float position = (float)intPosition*0.088;
//				angleRad_rct anglePos;
//				anglePos.setFromDegrees(position);
//
//				float voltage  = (float)(DXL_MAKEWORD(dynamixelRxBuffer[PKT_ERROR+21],dynamixelRxBuffer[PKT_ERROR+2]))/10;
//				uint8_t temperature = dynamixelRxBuffer[PKT_ERROR+23];
//
//				m_actuatorList[i]->setRxCurrent(currentmA);
//				m_actuatorList[i]->setRxAngle(position);
//				m_actuatorList[i]->setRxAngularVelocity(angVelocity);
//				m_actuatorList[i]->setRxAngle(anglePos);
//				m_actuatorList[i]->setRxTemperature((float)temperature);
//				m_actuatorList[i]->setRxPwm(fPwm);
//				m_actuatorList[i]->setRxVoltage(voltage);
//				m_ackPending[i] = 0;
//
//				HAL_GPIO_WritePin(OUT_DEBUG_2_PORT, OUT_DEBUG_2_PIN, GPIO_PIN_RESET);
//				//Set data in actuators
//				return i;
//			}
//		}
//	}
//	else
//	{
//		m_actuatorList[actuatorSelect]->setSafetyFlag(SAFETY_COMM_CORRUPTED_ERROR);
//	}
//
//	return 0;
// }


 uint8_t dynamixelSequencer::dataCommReceiveFastSync()
 {

	if(validStatusMessage(dynamixelRxBuffer))
	{
		uint32_t index = PKT_ERROR + 1; // ID of the frist actuator that replies
		//bool dataToExtract = true;

		int i;
		for(i = 0 ; i < m_NbActuators; i++)
		{
			int actuatorSelect;
			for (actuatorSelect = 0; actuatorSelect < m_NbActuators; actuatorSelect++) {

				if(m_actuatorList[actuatorSelect]->getId() == dynamixelRxBuffer[index])
				{
					break;
				}
			}

			if(actuatorSelect < m_NbActuators) // Match trouvé?
			{

				int16_t pwm = DXL_MAKEWORD(dynamixelRxBuffer[index+1],dynamixelRxBuffer[index+2]);

				int16_t intCurrent = DXL_MAKEWORD(dynamixelRxBuffer[index+3],dynamixelRxBuffer[index+4]);
				int32_t intVelocity = *(int32_t*)&dynamixelRxBuffer[index+5]; // byte 5,6,7,8
				int32_t intPosition = *(int32_t*)&dynamixelRxBuffer[index+9];// byte 9,10,11,12
				float voltage = (float)(DXL_MAKEWORD(dynamixelRxBuffer[index+21],dynamixelRxBuffer[index+22]))/10;
				uint8_t temperature = dynamixelRxBuffer[index+23];

				m_actuatorList[i]->setRxVoltage(voltage);

				float fPwm = (float)pwm * 0.113;
				m_actuatorList[actuatorSelect]->setRxPwm(fPwm);
				float currentmA = (float)(intCurrent) * 2.69;
				m_actuatorList[actuatorSelect]->setRxCurrent(currentmA);

				float velociy = (float)intVelocity * 0.229;
				angularVeocity_rct angVelocity;
				angVelocity.setFromDegrees(velociy);
				m_actuatorList[actuatorSelect]->setRxAngularVelocity(angVelocity);

				float position = (float)intPosition*0.088;
				angleRad_rct anglePos;
				anglePos.setFromDegrees(position);
				m_actuatorList[actuatorSelect]->setRxAngle(anglePos);

				m_actuatorList[actuatorSelect]->setRxTemperature((float)temperature);

				m_ackPending[actuatorSelect] = 0;

				index = index + 24 + PKT_CRC_LEN + PKT_ERROR_LEN; // Passer les données, le CRC et l'erreur du joint suivant ( si présent)

				if(index >= g_commInProgress_expectedRxDataLen) // buffer entièrement parcouru
				{
					break; // force loop exit
				}

			}
			else
			{
				// Actionneur pas dans la liste ( peu probable)
			}
		}

		if(i >= m_NbActuators)
		{
			// pas tous les moteurs on reçu leur data
		}

	}
	else
	{
		return ERROR_DYNAMIXELSEQUENCER_SUCCESS;
	}

	return ERROR_DYNAMIXELSEQUENCER_NO_VALID_MESSAGE;
 }

 uint8_t dynamixelSequencer::writeControlReceive()
 {

	 // Recevie ack from write message
	if(validStatusMessage(dynamixelRxBuffer))
	{
		for(int i = 0 ; i < m_NbActuators; i++)
		{
			if(m_ackPending[i] == 1)
			{
				if(m_actuatorList[i]->getId() == dynamixelRxBuffer[PKT_ID])
				{

					m_ackPending[i] = 0;

					//uint8_t errorStatus = dynamixelRxBuffer[PKT_ERROR];

					//TODO manage status / ack management
					//manageDynamixelErrors(errorStatus);
					//Set data in actuators
					return actuatorSelect;
				}
			}
		}
	}
	else
	{
		//TODO manage errors
	}

	return 0;
 }








