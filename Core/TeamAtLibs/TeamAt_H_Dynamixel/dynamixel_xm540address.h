// EEPROM PARAMETERS
#define 	XM540_MODEL_NUMBER_ADDR             0        //	R	1,120	-	-
#define 	XM540_MODEL_INFORMATION_ADDR        2        //	R	-	-	-
#define 	XM540_FIRMWARE_VERSION_ADDR         6        //	R	-	-	-
#define 	XM540_ID_ADDR                       7        //  RW	1	0 ~ 252	-
#define 	XM540_BAUD_RATE_ADDR                8        //  RW	1	0 ~ 7	-
#define 	XM540_RETURN_DELAY_TIME_ADDR        9        //	RW	250	0 ~ 254	2 [μsec]
#define 	XM540_DRIVE_MODE_ADDR               10       //  RW	0	0 ~ 5	-
	#define		XM540_DRIVE_MODE_NORMAL_REVERSE_BIT_POS		0
	#define		XM540_DRIVE_MODE_NORMAL						0
	#define 	XM540_DRIVE_MODE_REVERSE					1

	#define		XM540_DRIVE_MODE_MASTER_SLAVE_BIT_POS		1
	#define		XM540_DRIVE_MODE_MASTER						0
	#define 	XM540_DRIVE_MODE_SALVE						1

	#define		XM540_DRIVE_MODE_PROFILE_CONFIG_BIT_POS		2
	#define		XM540_DRIVE_MODE_PROFILE_VELOCITY_BASED		0
	#define		XM540_DRIVE_MODE_PROFILE_TIME_BASED			1

	#define		XM540_DRIVE_MODE_TORQUE_ON_BY_GOAL_BIT_POS	3
	#define		XM540_DRIVE_MODE_TORQUE_ON_BY_GOAL_DISABLE 	0
	#define		XM540_DRIVE_MODE_TORQUE_ON_BY_GOAL_ENABLE	1

#define 	XM540_OPERATING_MODE_ADDR           11       //	RW	3	0 ~ 16	-
	#define 	XM540_OPERATING_MODE_CURRENT				0
	#define		XM540_OPERATING_MODE_VELOCITY				1
	#define		XM540_OPERATING_MODE_POSITION				3
	#define		XM540_OPERATING_MODE_POSITION_MULTITURN		4
	#define		XM540_OPERATING_MODE_POSITION_CURRENT		5
	#define		XM540_OPERATING_MODE_PWM					16
#define 	XM540_SECONDARY_SHADOW_ID_ADDR      12       //	RW	255	0 ~ 252	-
#define 	XM540_PROTOCOL_TYPE_ADDR            13       //	RW	2	1 ~ 2	-
#define 	XM540_HOMING_OFFSET_ADDR            20       // 	RW	0	-1,044,479 ~ 1,044,479	1 [pulse]
#define 	XM540_MOVING_THRESHOLD_ADDR         24       //	RW	10	0 ~ 1,023	0.229 [rev/min]
#define 	XM540_TEMPERATURE_LIMIT_ADDR        31       //	RW	80	0 ~ 100	1 [°C]
#define 	XM540_MAX_VOLTAGE_LIMIT_ADDR        32       //	RW	160	95 ~ 160	0.1 [V]
#define 	XM540_MIN_VOLTAGE_LIMIT_ADDR        34       //	RW	95	95 ~ 160	0.1 [V]
#define 	XM540_PWM_LIMIT_ADDR                36       //	RW	885	0 ~ 885	0.113 [%]
#define 	XM540_CURRENT_LIMIT_ADDR            38       //	RW	2,047	0 ~ 2,047	2.69 [mA]
#define 	XM540_VELOCITY_LIMIT_ADDR           44       //	RW	128	0 ~ 1,023	0.229 [rev/min]
#define 	XM540_MAX_POSITION_LIMIT_ADDR       48       //	RW	4,095	0 ~ 4,095	1 [pulse]
#define 	XM540_MIN_POSITION_LIMIT_ADDR       52       //	RW	0	0 ~ 4,095	1 [pulse]
#define 	XM540_EXTERNAL_PORT_MODE1_ADDR      56       //	RW	3	0 ~ 3	-
#define 	XM540_EXTERNAL_PORT_MODE2_ADDR      57       //	RW	3	0 ~ 3	-
#define 	XM540_EXTERNAL_PORT_MODE3_ADDR      58       //	RW	3	0 ~ 3	-
#define 	XM540_STARTUP_CONFIGURATION_ADDR    60       //	RW	0	3	-
#define 	XM540_SHUTDOWN_ADDR                 63       //	RW	52	-	-

// RAM PARAMETERS
#define 	XM540_Torque_Enable_ADDR	        64         //RW	0	0 ~ 1	-
	#define 	XM540_Torque_Enable_ON		1
	#define 	XM540_Torque_Enable_OFF		0
#define 	XM540_LED_ADDR	                    65         //RW	0	0 ~ 1	-
	#define 	XM540_LED_ON		1
	#define 	XM540_LED_OFF		0
#define 	XM540_Status_Return_Level_ADDR		68         //RW	2	0 ~ 2	-
#define 	XM540_Registered_Instruction_ADDR	69         //R	0	0 ~ 1	-
#define 	XM540_Hardware_Error_Status_ADDR	70         //R	0	-	-
#define 	XM540_Velocity_I_Gain_ADDR		    76         //RW	1,920	0 ~ 16,383	-
#define 	XM540_Velocity_P_Gain_ADDR		    78         //RW	100	0 ~ 16,383	-
#define 	XM540_Position_D_Gain_ADDR		    80         //RW	0	0 ~ 16,383	-
#define 	XM540_Position_I_Gain_ADDR		    82         //RW	0	0 ~ 16,383	-
#define 	XM540_Position_P_Gain_ADDR		    84         //RW	800	0 ~ 16,383	-
#define 	XM540_Feedforward_2nd_Gain_ADDR		88         //RW	0	0 ~ 16,383	-
#define 	XM540_Feedforward_1st_Gain_ADDR		90         //RW	0	0 ~ 16,383	-
#define 	XM540_Bus_Watchdog_ADDR		        98         //RW	0	1 ~ 127	20 [msec]
#define 	XM540_Goal_PWM_ADDR		            100        //RW	-	-PWM Limit(36) ~PWM Limit(36)	0.113 [%]
#define 	XM540_Goal_Current_ADDR		        102        //RW	-	-Current Limit(38) ~Current Limit(38)	2.69 [mA]
#define 	XM540_Goal_Velocity_ADDR		    104        //RW	-	-Velocity Limit(44) ~Velocity Limit(44)	0.229 [rev/min]
#define 	XM540_Profile_Acceleration_ADDR		108        //RW	0	0 ~ 32,7670 ~ 32,737	214.577 [rev/min2] 1 [ms]
#define 	XM540_Profile_Velocity_ADDR		    112        //RW	0	0 ~ 32,767	0.229 [rev/min]
#define 	XM540_Goal_Position_ADDR		    116        //RW	-	Min Position Limit(52) ~Max Position Limit(48)	1 [pulse]
#define 	XM540_Realtime_Tick_ADDR		    120        //R	-	0 ~ 32,767	1 [msec]
#define 	XM540_Moving_ADDR		            122        //R	0	0 ~ 1	-
#define 	XM540_Moving_Status_ADDR		    123        //R	0	-	-
#define 	XM540_Present_PWM_ADDR		        124        //R	-	-	-
#define 	XM540_Present_Current_ADDR		    126        //R	-	-	2.69 [mA]
#define 	XM540_Present_Velocity_ADDR		    128        //R	-	-	0.229 [rev/min]
#define 	XM540_Present_Position_ADDR		    132        //R	-	-	1 [pulse]
#define 	XM540_Velocity_Trajectory_ADDR		136        //R	-	-	0.229 [rev/min]
#define 	XM540_Position_Trajectory_ADDR		140        //R	-	-	1 [pulse]
#define 	XM540_Present_Input_Voltage_ADDR	144        //R	-	-	0.1 [V]
#define 	XM540_Present_Temperature_ADDR		146        //R	-	-	1 [°C]
#define 	XM540_Backup_Ready_ADDR		        147        //R	-	0 ~ 1	-
#define 	XM540_External_Port_Data_1_ADDR		152        //RW	-	-	-
#define 	XM540_External_Port_Data_2_ADDR		154        //RW	-	-	-
#define 	XM540_External_Port_Data_3_ADDR		156        //RW	-	-	-
#define 	XM540_Indirect_Address_1_ADDR		168        //RW	224	64 ~ 661	-
#define 	XM540_Indirect_Address_2_ADDR		170        //RW	225	64 ~ 661	-
#define 	XM540_Indirect_Address_3_ADDR		172        //RW	226	64 ~ 661	-   …	…	…	…	…	-	-
#define 	XM540_Indirect_Address_26_ADDR	    218        //RW	249	64 ~ 661	-
#define 	XM540_Indirect_Address_27_ADDR	    220        //RW	250	64 ~ 661	-
#define 	XM540_Indirect_Address_28_ADDR	    222        //RW	251	64 ~ 661	-
#define 	XM540_Indirect_Data_1_ADDR	        224        //RW	0	0 ~ 255	-
#define 	XM540_Indirect_Data_2_ADDR	        225        //RW	0	0 ~ 255	-
#define 	XM540_Indirect_Data_3_ADDR	        226        //RW	0	0 ~ 255	- …	…	…	…	…	-	-
#define 	XM540_Indirect_Data_26_ADDR	        249        //RW	0	0 ~ 255	-
#define 	XM540_Indirect_Data_27_ADDR	        250        //RW	0	0 ~ 255	-
#define 	XM540_Indirect_Data_28_ADDR	        251        //RW	0	0 ~ 255	-
#define 	XM540_Indirect_Address_29_ADDR	    578        //RW	634	64 ~ 661	-
#define 	XM540_Indirect_Address_30_ADDR	    580        //RW	635	64 ~ 661	-
#define 	XM540_Indirect_Address_31_ADDR	    582        //RW	636	64 ~ 661	- …	…	…	…	…	-	-
#define 	XM540_Indirect_Address_54_ADDR	    628        //RW	659	64 ~ 661	-
#define 	XM540_Indirect_Address_55_ADDR	    630        //RW	660	64 ~ 661	-
#define 	XM540_Indirect_Address_56_ADDR	    632        //RW	661	64 ~ 661	-
#define 	XM540_Indirect_Data_29_ADDR	        634        //RW	0	0 ~ 255	-
#define 	XM540_Indirect_Data_30_ADDR	        635        //RW	0	0 ~ 255	-
#define 	XM540_Indirect_Data_31_ADDR	        636        //RW	0	0 ~ 255	-   …	…	…	…	…	-	-
#define 	XM540_Indirect_Data_54_ADDR	        659        //  RW	0	0 ~ 255	-
#define 	XM540_Indirect_Data_55_ADDR	        660        //  RW	0	0 ~ 255	-
#define 	XM540_Indirect_Data_56_ADDR	        661        //  RW	0	0 ~ 255	-



// EEPROM PARAMETERS
#define    	XM540_MODEL_NUMBER_LEN              2       //	R	1,120	-	-
#define 	XM540_MODEL_INFORMATION_LEN         4       //	R	-	-	-
#define 	XM540_FIRMWARE_VERSION_LEN          1       //	R	-	-	-
#define 	XM540_ID_LEN                        1       //  RW	1	0 ~ 252	-
#define 	XM540_BAUD_RATE_LEN                 1       //  RW	1	0 ~ 7	-
#define 	XM540_RETURN_DELAY_TIME_LEN         1       //	RW	250	0 ~ 254	2 [μsec]
#define 	XM540_DRIVE_MODE_LEN                1       //  RW	0	0 ~ 5	-
#define 	XM540_OPERATING_MODE_LEN            1       //	RW	3	0 ~ 16	-
#define 	XM540_SECONDARY_SHADOW_ID_LEN       1       //	RW	255	0 ~ 252	-
#define 	XM540_PROTOCOL_TYPE_LEN             1       //	RW	2	1 ~ 2	-
#define 	XM540_HOMING_OFFSET_LEN             4       // 	RW	0	-1,044,479 ~ 1,044,479	1 [pulse]
#define 	XM540_MOVING_THRESHOLD_LEN          4       //	RW	10	0 ~ 1,023	0.229 [rev/min]
#define 	XM540_TEMPERATURE_LIMIT_LEN         1       //	RW	80	0 ~ 100	1 [°C]
#define 	XM540_MAX_VOLTAGE_LIMIT_LEN         2       //	RW	160	95 ~ 160	0.1 [V]
#define 	XM540_MIN_VOLTAGE_LIMIT_LEN         2       //	RW	95	95 ~ 160	0.1 [V]
#define 	XM540_PWM_LIMIT_LEN                 2       //	RW	885	0 ~ 885	0.113 [%]
#define 	XM540_CURRENT_LIMIT_LEN             2       //	RW	2,047	0 ~ 2,047	2.69 [mA]
#define 	XM540_VELOCITY_LIMIT_LEN            4       //	RW	128	0 ~ 1,023	0.229 [rev/min]
#define 	XM540_MAX_POSITION_LIMIT_LEN        4       //	RW	4,095	0 ~ 4,095	1 [pulse]
#define 	XM540_MIN_POSITION_LIMIT_LEN        4       //	RW	0	0 ~ 4,095	1 [pulse]
#define 	XM540_EXTERNAL_PORT_MODE1_LEN       1       //	RW	3	0 ~ 3	-
#define 	XM540_EXTERNAL_PORT_MODE2_LEN       1       //	RW	3	0 ~ 3	-
#define 	XM540_EXTERNAL_PORT_MODE3_LEN       1       //	RW	3	0 ~ 3	-
#define 	XM540_STARTUP_CONFIGURATION_LEN     1       //	RW	0	3	-
#define 	XM540_SHUTDOWN_LEN                  1       //	RW	52	-	-

// RAM PARAMETERS
#define 		XM540_Torque_Enable_LEN	            1     //RW	0	0 ~ 1	-
#define 		XM540_LED_LEN	                    1     //RW	0	0 ~ 1	-
#define 		XM540_Status_Return_Level_LEN	    1     //RW	2	0 ~ 2	-
#define 		XM540_Registered_Instruction_LEN	1     //R	0	0 ~ 1	-
#define 		XM540_Hardware_Error_Status_LEN		1     //R	0	-	-
#define 		XM540_Velocity_I_Gain_LEN		    2     //RW	1,920	0 ~ 16,383	-
#define 		XM540_Velocity_P_Gain_LEN		    2     //RW	100	0 ~ 16,383	-
#define 		XM540_Position_D_Gain_LEN		    2     //RW	0	0 ~ 16,383	-
#define 		XM540_Position_I_Gain_LEN		    2     //RW	0	0 ~ 16,383	-
#define 		XM540_Position_P_Gain_LEN		    2     //RW	800	0 ~ 16,383	-
#define 		XM540_Feedforward_2nd_Gain_LEN		2     //RW	0	0 ~ 16,383	-
#define 		XM540_Feedforward_1st_Gain_LEN		2     //RW	0	0 ~ 16,383	-
#define 		XM540_Bus_Watchdog_LEN		        1     //RW	0	1 ~ 127	20 [msec]
#define 		XM540_Goal_PWM_LEN		            2     //RW	-	-PWM Limit(36) ~PWM Limit(36)	0.113 [%]
#define 		XM540_Goal_Current_LEN		        2     //RW	-	-Current Limit(38) ~Current Limit(38)	2.69 [mA]
#define 		XM540_Goal_Velocity_LEN		        4     //RW	-	-Velocity Limit(44) ~Velocity Limit(44)	0.229 [rev/min]
#define 		XM540_Profile_Acceleration_LEN		4     //RW	0	0 ~ 32,7670 ~ 32,737	214.577 [rev/min2] 1 [ms]
#define 		XM540_Profile_Velocity_LEN		    4     //RW	0	0 ~ 32,767	0.229 [rev/min]
#define 		XM540_Goal_Position_LEN		        4     //RW	-	Min Position Limit(52) ~Max Position Limit(48)	1 [pulse]
#define 		XM540_Realtime_Tick_LEN		        2     //R	-	0 ~ 32,767	1 [msec]
#define 		XM540_Moving_LEN		            1     //R	0	0 ~ 1	-
#define 		XM540_Moving_Status_LEN	            1     //R	0	-	-
#define 		XM540_Present_PWM_LEN		        2     //R	-	-	-
#define 		XM540_Present_Current_LEN		    2     //R	-	-	2.69 [mA]
#define 		XM540_Present_Velocity_LEN		    4     //R	-	-	0.229 [rev/min]
#define 		XM540_Present_Position_LEN		    4     //R	-	-	1 [pulse]
#define 		XM540_Velocity_Trajectory_LEN	    4     //R	-	-	0.229 [rev/min]
#define 		XM540_Position_Trajectory_LEN	    4     //R	-	-	1 [pulse]
#define 		XM540_Present_Input_Voltage_LEN		2     //R	-	-	0.1 [V]
#define 		XM540_Present_Temperature_LEN	    1     //R	-	-	1 [°C]
#define 		XM540_Backup_Ready_LEN		        1     //R	-	0 ~ 1	-
#define 		XM540_External_Port_Data_1_LEN		2     //RW	-	-	-
#define 		XM540_External_Port_Data_2_LEN		2     //RW	-	-	-
#define 		XM540_External_Port_Data_3_LEN		2     //RW	-	-	-
#define 		XM540_Indirect_Address_1_LEN	    2     //RW	224	64 ~ 661	-
#define 		XM540_Indirect_Address_2_LEN	    2     //RW	225	64 ~ 661	-
#define 		XM540_Indirect_Address_3_LEN	    2     //RW	226	64 ~ 661	-   …	…	…	…	…	-	-
#define 		XM540_Indirect_Address_26_LEN	    2     //RW	249	64 ~ 661	-
#define 		XM540_Indirect_Address_27_LEN	    2     //RW	250	64 ~ 661	-
#define 		XM540_Indirect_Address_28_LEN	    2     //RW	251	64 ~ 661	-
#define 		XM540_Indirect_Data_1_LEN	        1     //RW	0	0 ~ 255	-
#define 		XM540_Indirect_Data_2_LEN	        1     //RW	0	0 ~ 255	-
#define 		XM540_Indirect_Data_3_LEN	        1     //RW	0	0 ~ 255	- …	…	…	…	…	-	-
#define 		XM540_Indirect_Data_26_LEN	        1     //RW	0	0 ~ 255	-
#define 		XM540_Indirect_Data_27_LEN	        1     //RW	0	0 ~ 255	-
#define 		XM540_Indirect_Data_28_LEN	        1     //RW	0	0 ~ 255	-
#define 		XM540_Indirect_Address_29_LEN	    2     //RW	634	64 ~ 661	-
#define 		XM540_Indirect_Address_30_LEN	    2     //RW	635	64 ~ 661	-
#define 		XM540_Indirect_Address_31_LEN	    2     //RW	636	64 ~ 661	- …	…	…	…	…	-	-
#define 		XM540_Indirect_Address_54_LEN	    2     //RW	659	64 ~ 661	-
#define 		XM540_Indirect_Address_55_LEN	    2     //RW	660	64 ~ 661	-
#define 		XM540_Indirect_Address_56_LEN	    2     //RW	661	64 ~ 661	-
#define 		XM540_Indirect_Data_29_LEN	        1     //RW	0	0 ~ 255	-
#define 		XM540_Indirect_Data_30_LEN	        1     //RW	0	0 ~ 255	-
#define 		XM540_Indirect_Data_31_LEN	        1     //RW	0	0 ~ 255	-   …	…	…	…	…	-	-
#define 		XM540_Indirect_Data_54_LEN	        1     //  RW	0	0 ~ 255	-
#define 		XM540_Indirect_Data_55_LEN	        1     //  RW	0	0 ~ 255	-
#define 		XM540_Indirect_Data_56_LEN	        1     //  RW	0	0 ~ 255	-
