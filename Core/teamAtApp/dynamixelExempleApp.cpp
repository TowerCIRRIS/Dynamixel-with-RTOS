/*
 * dynamixelExempleApp.cpp
 *
 *  Created on: 13 avr. 2022
 *      Author: teamat
 */

#include <TeamAt_H_Dynamixel/actuator_DynamixelXM.h> //"./hardware/actuator_DynamixelXM.h"
#include<vector>
#include "stdio.h"
#include "stdlib.h"

#include "at_plaformAbstraction_V1_0.h" // Pour appeler entrees/sorties

#include "dynamixelExempleApp.h"

#include "uartApp.h"

#include "./TeamAt_H_Dynamixel/dynamixelSequencer.h"

#define NB_ACTUATOR 1 // 3

typedef struct
{
	angleRad_rct position[NB_ACTUATOR];

} positionWaypoint_t;

// Étape 1: Declarer les actionneurs
actuator_DynamixelXM actuator1;
//actuator_DynamixelXM actuator2;
//actuator_DynamixelXM actuator3;

// Étape 2: Déclarer le séquenceur qui gère la communication
dynamixelSequencer dxSequencer(5); // 5 ms refresh position

// Optionel, propre à l'Exemple
// Pour faciliter l'accès aux moteurs dans une boucle on crée un tableau
// NB_ACTUATOR est défini dans trajectoireSimple.h
actuator_DynamixelXM *actuatorList[NB_ACTUATOR];

//char serialOutLine[300];// Port série; Buffer for string to send serial buffer

//// Variables générales
// Ajouter variables globales ici
#define DXL_SERIAL   serial3

void dynamixelExample_init()
{
	uartInit();

	actuator1.info.ProductID = 0x01;				// ID de l'Actionneur
	actuator1.info.gearRatio = 1.0;		// Si gearbox externe on peut changer
	actuator1.info.motorOrientation = 1.0;// Sens de rotation -1.0 ou + 1.0 ( Pratiquer si le moteur est installé physiquement à l'envers)
	actuator1.info.type = actuatorType_t::rotary;	// Type du moteur
	//Note: les autres paramètre ne sont pas encore implémentés
	dxSequencer.addActuator(&actuator1);
	actuatorList[0] = &actuator1;

//		actuator2.info.ProductID = 0x02;				// ID de l'Actionneur
//		actuator2.info.gearRatio = 1.0;					// Si gearbox externe on peut changer
//		actuator2.info.motorOrientation = -1.0; 		// Exemple de rotation inversé
//		actuator2.info.type = actuatorType_t::rotary;	// Type du moteur
//		//Note:les autres paramètre ne sont pas encore implémentés
//		dxSequencer.addActuator(&actuator2);
//		actuatorList[1] = &actuator2;

//		actuator3.info.ProductID = 0x03;				// ID de l'Actionneur
//		actuator3.info.gearRatio = 1.0;					// Si gearbox externe on peut changer
//		actuator3.info.motorOrientation = 1.0;			// Sens de rotation -1.0 ou + 1.0 ( Pratiquer si le moteur est installé physiquement à l'envers)
//		actuator3.info.type = actuatorType_t::rotary;	// Type du moteur
//		//Note: les autres paramètre ne sont pas encore implémentés
//		dxSequencer.addActuator(&actuator3);
//		actuatorList[2] = &actuator3;

}

// Les valeurs suivant servent à définir les valeurs de commandes
angleRad_rct angleCommand[NB_ACTUATOR];		// = { 0.0, 0.0, 0.0 };
angularVeocity_rct velocityCommand[NB_ACTUATOR];		// = { 0.0, 0.0, 0.0 };
float currentCommand[NB_ACTUATOR];		// = { 0.0, 0.0, 0.0 };
float pwmCommand[NB_ACTUATOR];		// = { 0.0, 0.0, 0.0 };

int actuatorCommCounter = 0; // Compteur pour ralentir la communication;

int serialPrintTimer = 0; // Compteur pour l'affichage générale sur le port série

// Variables pour monitoring des ereurs
actuatorError_rct errorStatus = actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
int errorCount = 0;

operatingMode_t controlMode = CONTROL_MODE_POSITION_ANGULAR;

// Création de notre trajectoire de test
std::vector<positionWaypoint_t> trajectoire;

positionWaypoint_t waypoint;

bool trajectoryEnable = false;
unsigned int trajectoryIndex = 0;

void dynamixelExample_beforeLoop()
{
	serial1.print("\t Démarrage. \n\r");
	serial1.startRxInterrupt(1);

	serial3.enableTxMode();

	dxSequencer.start();

	for (int i = 0; i < NB_ACTUATOR; i++)
	{
		// On s'assure que le contrôle des moteurs est désactivé au démarrage
		errorStatus = actuatorList[i]->disableControl();
		// On mets les actionneurs en mode de contrôle de position au démarrage
		errorStatus = actuatorList[i]->setOperatingMode(controlMode);
	}

	// Point 1
	waypoint.position[0] = 0.0;
//			waypoint.position[1] = 0.0;
//			waypoint.position[2] = 0.0;
	trajectoire.push_back(waypoint);

	// Point 2
	waypoint.position[0].setFromDegrees(120);
//			waypoint.position[1].setFromDegrees(290);
//			waypoint.position[2].setFromDegrees(-50);
	trajectoire.push_back(waypoint);

	// Point 3
	waypoint.position[0].setFromDegrees(420);
//			waypoint.position[1].setFromDegrees(-120);
//			waypoint.position[2].setFromDegrees(-300);
	trajectoire.push_back(waypoint);

	// Point 4
	waypoint.position[0].setFromDegrees(300);
//			waypoint.position[1].setFromDegrees(3-10);
//			waypoint.position[2].setFromDegrees(-600);
	trajectoire.push_back(waypoint);
}

void dynamixelExample_mainLoop()
{

	dynamixelExample_gestionCommandesPortSerie();

	//dxSequencer.handle();

}

void dynamixelExample_HighPriority_mainLoop()
{

	//dynamixelExample_gestionCommandesPortSerie();

	dxSequencer.handle();

}

void dynamixelExample_task10ms()
{
	dynamixelExample_affichageInfo();

	dynamixelExample_jouerTrajectoire();

}

void dynamixelExample_gestionCommandesPortSerie()
{
	char tempTxBufer[64] __attribute__((unused));


	if (serial1.available())// Si charactère recu du port série
	{
		int rxLen __attribute__((unused)) = serial1.read(tempTxBufer, 64);

		// Activer / désactive le contrôle moteur en appuyand sur "Entrer"
		if (tempTxBufer[0] == 13) // '13' = Entrer
		{
			if (trajectoryEnable)
			{
				trajectoryEnable = false;
				for (int i = 0; i < NB_ACTUATOR; i++)
				{
					errorStatus = actuatorList[i]->disableControl();
					errorStatus = actuatorList[i]->setLed(false);
				}

				if (errorStatus)
				{
					serial1.print("\n\r --- ERROR ---\n\r");
					errorCount++;
					errorStatus = actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
				}
				serial1.print("--- CONTROL STOPPED ---\n\r");
			}
			else
			{
				trajectoryEnable = true;
				trajectoryIndex = 0;
				for (int i = 0; i < NB_ACTUATOR; i++)
				{
					errorStatus = actuatorList[i]->enableControl();
					errorStatus = actuatorList[i]->setLed(true);

					if (errorStatus)
					{
						serial1.print("\n\r --- ERROR ---\n\r");
						errorCount++;
						errorStatus =
						actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
					}
				}

				serial1.print("--- CONTROL STARTED ---%d %d\n\r",12,13);
			}
		}

		// Passe au mode de contrôle en position
		if (tempTxBufer[0] == '1')
		{
			controlMode = CONTROL_MODE_POSITION_ANGULAR;
			for (int i = 0; i < NB_ACTUATOR; i++)
			{
				errorStatus = actuatorList[i]->setOperatingMode(controlMode);
				if (errorStatus)
				{
					serial1.print("\n\r --- ERROR ---\n\r");
					errorCount++;
					errorStatus =
							actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
				}

				angleCommand[i] = actuatorList[i]->getAngle();
			}

			serial1.write("--- SWITCHED TO POSITION MODE ---\n\r");
		}

		// Test set zero position
		if (tempTxBufer[0] == 'z')
		{
			serial1.write("--- Set Zero Position - ---\n\r");

			for (int i = 0; i < NB_ACTUATOR; i++)
			{
				errorStatus = actuatorList[i]->setZeroPosition();
				if (errorStatus)
				{
					serial1.write("\n\r --- ERROR ---\n\r");
					errorCount++;
					errorStatus =
							actuatorError_rct::ERROR_ROBOCORE_ACTUATOR_NONE;
				}

				angleCommand[i].setFromDegrees(0);
			}

		}

		serial1.startRxInterrupt(1);
	}
}

void dynamixelExample_affichageInfo()
{
	serialPrintTimer++;
	if (serialPrintTimer >= 10) // On affiche l'info une fois sur 10, donc aux 100 ms maximum
	{
		serialPrintTimer = 0;

		// Détection d'erreurs
		for (int i = 0; i < NB_ACTUATOR; i++)
		{
			if (actuatorList[i]->testSafetyFlag(SAFETY_COMM_TIMEOUT_ERROR))
			{
				serial1.print("Actuator: SAFETY_COMM_TIMEOUT_ERROR flag catch\n\r");
				actuatorList[i]->clearSafetyFlag(SAFETY_COMM_TIMEOUT_ERROR);
			}
		}

	}
}

void dynamixelExample_jouerTrajectoire()
{
	if (trajectoryEnable)
	{
		bool reached = true;
		for (int i = 0; i < NB_ACTUATOR; i++)
		{
			// Si le point est à +- .2 degrés du point désiré
			if (!(abs(
					trajectoire[trajectoryIndex].position[i].degConvert()
							- actuatorList[i]->getAngle().degConvert()) < 0.2))
			{
				reached = false;
			}
		}

		if (reached)
		{
			serial1.print("Point %u reached at %ums\n\r",trajectoryIndex, atGetSysTick_ms());

			trajectoryIndex++;

			if (trajectoryIndex >= trajectoire.size())
			{
				// Fin de la trajectoire
				trajectoryIndex = 0;
				trajectoryEnable = false;

				serial1.print("--- Trajectory completed ---\n\r");
			}
		}

		for (int i = 0; i < NB_ACTUATOR; i++)
		{
			if (controlMode == CONTROL_MODE_POSITION_ANGULAR)
			{
				actuatorList[i]->setAngleCommand(
						trajectoire[trajectoryIndex].position[i]);
			}

			//			  if(controlMode == CONTROL_MODE_CURRENT)
			//			  {
			//				  actuatorList[i]->setCurrentCommand(currentCommand[i]);
			//			  }
			//
			//			  if(controlMode == CONTROL_MODE_VELOCITY_ANGULAR)
			//			  {
			//				  actuatorList[i]->setAngularVelocityCommand(velocityCommand[i]);
			//			  }
			//
			//			  if(controlMode == CONTROL_MODE_PWM)
			//			  {
			//				  actuatorList[i]->setPwmCommand(pwmCommand[i]);
			//			  }

		}
	}
}
