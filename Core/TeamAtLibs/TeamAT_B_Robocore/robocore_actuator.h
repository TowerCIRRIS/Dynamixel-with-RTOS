/**
 * @file robocore_actuator.h
 * @brief Classe de d'actionneur de base pour robotCore
 * @version 1.2
 *
 * Version 1.2 2022-03-15
 * 	-Renommer variable pour inclure les unités
 * 	-Renommer variables membres
 * 	-ajout du "control mode"
 *
 * Version 1.1.0 2022-01-10
 * 	-Ajout orientation configurable
 * 	- Ajout contrôle en courant DRF_CURRENT_COMMAND,
 *
 */


#ifndef ROBOCORE_ACTUATOR_H
    #define ROBOCORE_ACTUATOR_H


    #include "roboCoreTypes.h"
    #include "robocore_Peripheral.h"

	typedef enum
	{
		ERROR_ROBOCORE_ACTUATOR_NONE,
		ERROR_ROBOCORE_ACTUATOR_UNDEFINED,
		ERROR_ROBOCORE_ACTUATOR_FUNCTION_NOT_AVAILABLE,
		ERROR_ROBOCORE_ACTUATOR_NOT_ENABLED,
		ERROR_ROBOCORE_ACTUATOR_CONTROL_MODE,
		ERROR_ROBOCORE_ACTUATOR_BUSY,
		ERROR_ROBOCORE_ACTUATOR_INVALID_PARAMETER,

		ERROR_ROBOCORE_ACTUATOR_CIRCULAR_BUFFER = 1000,
		ERROR_ROBOCORE_ACTUATOR_CIRCULAR_BUFFER_EMPTY = 1001,
		ERROR_ROBOCORE_ACTUATOR_CIRCULAR_BUFFER_MODE_ERROR = 1252,
		ERROR_ROBOCORE_ACTUATOR_CIRCULAR_BUFFER_NOT_ENOUGH_SPACE = 1253,
		ERROR_ROBOCORE_ACTUATOR_CIRCULAR_BUFFER_FULL = 1254,
		ERROR_ROBOCORE_ACTUATOR_CIRCULAR_BUFFER_WRITE_ERROR = 1255,



	}actuatorError_rct;

	// Data read flag enum
	typedef enum
	{
		DRF_ANGULAR_VELOCITY_RX,
		DRF_ANGULAR_VELOCITY_CALC,
		DRF_ANGULAR_VELOCITY_COMMAND,
		DRF_ANGLE_RX,
		DRF_ANGLE_COMMAND,
		DRF_LINEAR_VELOCITY_RX,
		DRF_LINEAR_VELOCITY_CALC,
		DRF_LINEAR_VELOCITY_COMMAND,
		DRF_LINEAR_POSITION_RX,
		DRF_LINEAR_POSITION_COMMAND,
		DRF_CURRENT_RX,
		DRF_CURRENT_COMMAND,
		DRF_TORQUE_RX,
		DRF_TORQUE_COMMAND,
		DRF_TEMPERATURE_RX,
		DRF_PWM_COMMAND,
		DRF_PWM_RX,
		DRF_VOLTAGE_RX,
		DRF_ITEM_COUNT

	}dataReadyEnum_t;


	typedef enum
	{
		SAFETY_TEMPERATURE_WARNING,
		SAFETY_TEMPERATURE_ERROR,
		SAFETY_POSITION_ERROR,
		SAFETY_VELOCITY_ERROR,
		SAFETY_COMM_MISSING_DATA_ERROR,
		SAFETY_COMM_TIMEOUT_ERROR,
		SAFETY_COMM_CORRUPTED_ERROR,
		SAFETY_ITEM_COUNT


	}safetyEnum_t;


	typedef enum
	{
		CONTROL_MODE_UNDEFINED,
		CONTROL_MODE_POSITION_ANGULAR,
		CONTROL_MODE_VELOCITY_ANGULAR,
		CONTROL_MODE_TORQUE,
		CONTROL_MODE_POSITION_TORQUE_ANGULAR,
		CONTROL_MODE_POSITION_LINEAR,
		CONTROL_MODE_VELOCITY_LINEAR,
		CONTROL_MODE_PWM,
		CONTROL_MODE_CURRENT

	}operatingMode_t;



#define MOTOR_ORIENTATION_STANDARD (1.0)
#define MOTOR_ORIENTATION_INVERTED (-1.0)

typedef struct
{
    
    float gearRatio;
    float motorOrientation = MOTOR_ORIENTATION_STANDARD;
    actuatorType_t type;
    
    // Absolute physical limit
    float minPosition;
    float maxPosition;
    float maxTorquePeak;
    float maxTorqueContinuious;
    float maxCurrentmA;
    float maxSpeed;

    //Other info that might be usefull 
    float operatinVoltage;
    float weight;
    uint32_t ProductID; // ID defined by us to be able to identify the product, versions, etc.
    
}actuatorInfo_t;


    class robocore_actuator: public robocore_Peripheral{

        public:

            actuatorInfo_t info;

            //------------------------------------------------------------------------// 
            // Actuator Data access  functions
            //------------------------------------------------------------------------// 
                angularVeocity_rct getRxAngularVelocity();     // Received velocity from actuator
                bool newRxAngularVelocity(){return testDataReadyFlag(DRF_ANGULAR_VELOCITY_RX);}

                angularVeocity_rct  getCalcAngularVelocity();   // Calculated velocity
                bool newCalcAngularVelocity(){return testDataReadyFlag(DRF_ANGULAR_VELOCITY_CALC);}

                angleRad_rct getAngle();    // Only works for rotary actuators
                bool newAngle(){return testDataReadyFlag(DRF_ANGLE_RX);}
                
                float getRxLinearVelocity();  // Only for linear actuator
                bool newRxLinearVelocity(){return testDataReadyFlag(DRF_LINEAR_VELOCITY_RX);}

                float getCalcLinearVelocity();  // Only for linear actuator
                bool newCalcLinearVelocity(){return testDataReadyFlag(DRF_LINEAR_VELOCITY_CALC);}

                float getLinearPosition();  // Only for linear actuator
                bool newLinearPosition(){return testDataReadyFlag(DRF_LINEAR_POSITION_RX);}

                float getCurrent();
                bool newCurrent(){return testDataReadyFlag(DRF_CURRENT_RX);}

                float getVoltage();
                bool newVoltage(){return testDataReadyFlag(DRF_VOLTAGE_RX);}

                float getTorque();
                bool newTorque(){return testDataReadyFlag(DRF_TORQUE_RX);}

                float getTemperature();
                bool newTemperature(){return testDataReadyFlag(DRF_TEMPERATURE_RX);}

                float getPwm();
                bool newPwm(){return testDataReadyFlag(DRF_PWM_RX);}

                bool testDataReadyFlag(dataReadyEnum_t flag){

                		return((0x0001 << (int)flag) & m_dataReadFlags);
                }

                void setDataReadyFlag(dataReadyEnum_t flag)
                {
                	m_dataReadFlags = m_dataReadFlags | (0x0001 << (int)flag);
                }

               void clearDataReadyFlag(dataReadyEnum_t flag)
			   {

            	   m_dataReadFlags = m_dataReadFlags & ~(0x0001 << (int)flag);
			   }


            //------------------------------------------------------------------------// 
            // Actuator managements functions
            //------------------------------------------------------------------------// 

			virtual actuatorError_rct enableControl(){return ERROR_ROBOCORE_ACTUATOR_FUNCTION_NOT_AVAILABLE;}
			virtual actuatorError_rct disableControl(){return ERROR_ROBOCORE_ACTUATOR_FUNCTION_NOT_AVAILABLE;}
			bool getControlEnableStatus(){return m_ConstrolEnableStatus;}

			virtual actuatorError_rct setOperatingMode(operatingMode_t controlMode){m_controlMode = controlMode; return ERROR_ROBOCORE_ACTUATOR_NONE;}
			operatingMode_t getControlMode(){return m_controlMode;}

			virtual actuatorError_rct setZeroPosition(){return ERROR_ROBOCORE_ACTUATOR_FUNCTION_NOT_AVAILABLE;}

			/**
			 * @fn actuatorError_rct setId(uint32_t)
			 * @brief Cette fonction modifie la valeur du id dans la classe de l'actionneur
			 *
			 * \note Attention, cette fonction ne modifie pas le ID de l'actionneur physique.
			 *
			 * @param productId
			 */
			void setId(uint32_t productId){info.ProductID = productId;}


            //------------------------------------------------------------------------// 
            // Actuator Data Set functions
            //------------------------------------------------------------------------//

			// Data received via communication with the physical actuator
			// Only works for rotary actuators
			/**
			 * @fn void setRxAngle(angleRad_rct)
			 * @brief Set the received angle value in angle ( angle supports both deg/s or rad/s
			 *
			 * @param data
			 */
			virtual  void setRxAngle(angleRad_rct data){
					m_angleRx = data;
					setDataReadyFlag(DRF_ANGLE_RX);
			}

			// Set Received velocity from actuator
			/**
			 * @fn void setRxAngularVelocity(angularVeocity_rct)
			 * @brief set the received velocity in angle/sec (angle supports both deg/s or rad/s)
			 *
			 * @param data
			 */
			virtual  void setRxAngularVelocity(angularVeocity_rct data){
					m_angularVelocityRx = data;
					setDataReadyFlag(DRF_ANGULAR_VELOCITY_RX);
			}

			// Only for linear actuator
			/**
			 * @fn void setRxLinearPosition(float)
			 * @brief Set the received position value
			 *
			 * @param position_mm the position in milli meters
			 */
			virtual  void setRxLinearPosition(float position_mm){
					m_linearPositionRx = position_mm;
					setDataReadyFlag(DRF_LINEAR_POSITION_RX);
			}

			// Only for linear actuator
			virtual  void setRxLinearVelocity(float data){
				m_linearVelocityRx = data;
				setDataReadyFlag(DRF_LINEAR_VELOCITY_RX);
			}


			/**
			 * @fn void setRxCurrent(float)
			 * @brief se the received current value
			 *
			 * @param current_mA current in milliAmps
			 */
			virtual  void setRxCurrent(float current_mA){
				m_currentRx = current_mA;
				setDataReadyFlag(DRF_CURRENT_RX);
			}

			virtual  void setRxVoltage(float voltage){
				m_voltageRx = voltage;
				setDataReadyFlag(DRF_VOLTAGE_RX);
			}


			virtual  void setRxTemperature(float tempDegrees){
				m_temperatureRx = tempDegrees;
				setDataReadyFlag(DRF_TEMPERATURE_RX);
			}

			virtual  void setRxPwm(float pwm){
				m_pwmRx = pwm;
				setDataReadyFlag(DRF_PWM_RX);
			}


			// Calculated values
			virtual  void setCalcAngularVelocity(angularVeocity_rct data){}   // Set Calculated velocity



			// Actuator Commands
			virtual actuatorError_rct setAngleCommand(angleRad_rct data);   // Only works for rotary actuators
			virtual actuatorError_rct setAngularVelocityCommand(angularVeocity_rct data);      // Set Received velocity from actuator
			virtual actuatorError_rct setTorqueCommand(float data);
			virtual actuatorError_rct setCurrentCommand(float current_mA);
			virtual actuatorError_rct setLinearVelocityCommand(float data);  // Only for linear actuator
			virtual actuatorError_rct setLinearPositionCommand(float data);  // Only for linear actuator


			virtual actuatorError_rct setKpGain(float data){return ERROR_ROBOCORE_ACTUATOR_FUNCTION_NOT_AVAILABLE;}
			virtual actuatorError_rct setKiGain(float data){return ERROR_ROBOCORE_ACTUATOR_FUNCTION_NOT_AVAILABLE;}
			virtual actuatorError_rct setKdGain(float data){return ERROR_ROBOCORE_ACTUATOR_FUNCTION_NOT_AVAILABLE;}

			virtual actuatorError_rct setPwmCommand(float pwm);

            //------------------------------------------------------------------------// 
            // Safeties functions
            //------------------------------------------------------------------------//

			bool testSafetyFlag(safetyEnum_t flag)
			{
				return((0x0001 << (int)flag) & m_safetyFlags);
			}

			void setSafetyFlag(safetyEnum_t flag)
			{
			   m_safetyFlags = m_safetyFlags | (0x00000001 << (uint64_t)flag);
			}

			void clearSafetyFlag(safetyEnum_t flag)
			{
				m_safetyFlags = m_safetyFlags & ~(0x00000001 << (uint64_t)flag);
			}


        protected:

            uint32_t m_dataReadFlags;
            uint64_t m_safetyFlags;
            bool m_ConstrolEnableStatus = false;
            operatingMode_t m_controlMode;

          	angularVeocity_rct m_angularVelocityRx = 0.0;
          	angleRad_rct m_angleRx = 0.0;
          	float m_currentRx = 0.0;
          	float m_voltageRx = 0.0;
          	float m_torqueRx = 0.0;
        	float m_linearPositionRx = 0.0;
        	float m_linearVelocityRx = 0.0;
        	float  m_temperatureRx = 0;

        	float m_pwmRx = 0;

    		angleRad_rct m_angleCommand;
    		angularVeocity_rct m_angularVelocityCommand;
    		float m_torqueCommand = 0.0;
    		float m_currentCommand_mA = 0.0;
         	float m_linearPositionCommand = 0.0;
            float m_linearVelocityCommand = 0.0;
            float m_pwmCommand = 0.0;

    };

#endif
