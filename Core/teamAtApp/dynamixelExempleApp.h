/*
 * dynamixelExempleApp.h
 *
 *  Created on: 13 avr. 2022
 *      Author: teamat
 */

#ifndef TEAMATAPP_DYNAMIXELEXEMPLEAPP_H_
#define TEAMATAPP_DYNAMIXELEXEMPLEAPP_H_



//Fonctions à appeler dans le main
void dynamixelExample_init();
void dynamixelExample_beforeLoop();
void dynamixelExample_mainLoop();
void dynamixelExample_HighPriority_mainLoop();
void dynamixelExample_task10ms();

// Fonctions internes
void dynamixelExample_gestionCommandesPortSerie();
void dynamixelExample_affichageInfo();
void dynamixelExample_jouerTrajectoire();


#endif /* TEAMATAPP_DYNAMIXELEXEMPLEAPP_H_ */
