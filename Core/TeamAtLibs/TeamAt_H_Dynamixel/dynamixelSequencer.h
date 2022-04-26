/**
 * @file dynamixelSequencer.h
 * @brief
 * @version 1.0
 *
 * Version 1.01 2022-04-14
 * 	-Première version
 */

#ifndef TEAMATLIBS_DYNAMIXELSEQUENCER_H_
#define TEAMATLIBS_DYNAMIXELSEQUENCER_H_

#include <TeamAt_H_Dynamixel/dynamixel.h>
#include "at_plaformAbstraction_V1_0.h"

#define ERROR_DYNAMIXELSEQUENCER_NO_SPACE	-1
#define ERROR_DYNAMIXELSEQUENCER_SUCCESS	0

#define ERROR_UNEXPECTED_PACKET_RECEIVED	1
#define ERROR_DYNAMIXELSEQUENCER_NO_VALID_MESSAGE				2



#define	comState_idle 			0
#define comState_sending 		1
#define comState_waitingReply 	2
#define	comState_disable 		3


#define	dataState_read 			0
#define	dataState_writeControl 	1
#define	dataState_count 		3



class dynamixelSequencer
{
	public:
		dynamixelSequencer (int refreshTime_ms = 10);
		virtual ~dynamixelSequencer ();

		/**
		 * @fn int handle()
		 * @brief Main fuction, call in main loop.
		 *
		 * @return the current state
		 */
		int handle();

		/**
		 * @fn int addActuator(actuator_DynamixelXM*)
		 * @brief add an actuator to the sequence
		 *
		 * @param actuator pointer to the actuator_DynamixelXM
		 * @return status
		 */
		int addActuator(actuator_DynamixelXM *actuator);

		/**
		 * @fn void setReadRefreshPeriode(int)
		 * @brief Change the refresh periode of the sequencer
		 *
		 * @param refreshTime_ms the time in ms
		 */
		void setReadRefreshPeriode(int refreshTime_ms);

		/**
		 * @fn void start()
		 * @brief  Start the sequencer
		 *
		 */
		void start();

		/**
		 * @fn void stop()
		 * @brief Stop the sequencer
		 *
		 */
		void stop();

	private:

		/**
		 * @fn void dataRequestSend()
		 * @brief Send a read command for choosen data:
		 * 				Present_PWM
		 * 				Present_Current
		 * 				Present_Velocity
		 * 				Present_Position
		 * 				Velocity_Trajectory // No used
		 * 				Position_Trajectory // No used
		 * 				Present_Input_Voltage
		 * 				Present_Temperature
		 *
		 * 	@note This function was implemented before  dataRequestFastSend() and there is issues where sometimes the
		 * 	      actuator sends more bytes. Use   dataRequestFastSend() instead
		 */
		//void dataRequestSend();

		/**
		 * @fn void dataRequestFastSend()
		 * @brief Send a read command for choosen data:
		 * 				Present_PWM
		 * 				Present_Current
		 * 				Present_Velocity
		 * 				Present_Position
		 * 				Velocity_Trajectory // No used
		 * 				Position_Trajectory // No used
		 * 				Present_Input_Voltage
		 * 				Present_Temperature
		 *
		 */
		void dataRequestFastSend();

/**
 * @fn uint8_t dataCommReceiveFastSync()
 * @brief
 *
 * 	@note This function was implemented before  dataRequestFastSend() and there is issues where sometimes the
 * 	      actuator sends more bytes. Use   dataRequestFastSend() instead
 * @return
 */
//		uint8_t dataCommReceive(uint8_t actuatorSelect);


		/**
		 * @fn uint8_t dataCommReceiveFastSync()
		 * @brief Take received data and dispatch it to actuator
		 *
		 * @return
		 */
		uint8_t dataCommReceiveFastSync();

		void bulkWriteSend();
		uint8_t writeControlReceive();

		bool dataReadRequired();

		void manageDynamixelErrors(uint8_t errorStatus);

		uint8_t dynamixelTxBuffer[TXPACKET_MAX_LEN];
		uint8_t dynamixelRxBuffer[RXPACKET_MAX_LEN];

		uint32_t g_commInProgress_expectedRxDataLen = 0;
		uint8_t g_commInProgress_xpectedActuatorId = 0;

		uint32_t m_readPeriodTimeMs = 10;

		uint32_t m_replyTimeoutMs = 50;
		uint32_t m_sendTime;


		uint32_t g_lastReadStart= 0;
		uint32_t g_lastLpDataRefresh= 0;

		int m_NbActuators =0;
		int m_SequencerState = comState_disable;
		int m_dataState = dataState_read;

		#define NB_ACTUATOR_MAX	10
		uint8_t actuatorSelect = 0;

		actuator_DynamixelXM* m_actuatorList[NB_ACTUATOR_MAX];
		uint8_t m_idList[NB_ACTUATOR_MAX];
		uint8_t m_ackPending[NB_ACTUATOR_MAX];

};

#endif /* TEAMATLIBS_DYNAMIXELSEQUENCER_H_ */
