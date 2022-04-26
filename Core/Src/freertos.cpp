/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
	/////////////////////////////////////////////////////////////////////
	//! [lib1include]
		// Ajouté libririe générales ici
		#include "stm32l4xx_it.h" // pour le weak 50us
		#include "stdio.h"
		#include "stdbool.h"
	//! [lib1include]


    /////////////////////////////////////////////////////////////////////
	//! [lib3include]
    // Librairies de base / applications de TeamAT
		#include "microCounter.h"	// Pour le timer de boucle
		#include "at_plaformAbstraction_V1_0.h" // Pour appeler entrees/sorties
		#include "dynamixelExempleApp.h"
	//! [lib3include]

    /////////////////////////////////////////////////////////////////////
	//! [lib4include]
    // Librairies à ajouter (mettre .h et .c/.cpp dans l'arbre)
    //  *** USER

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
	/////////////////////////////////////////////////////////////////////////
	//! [variablesglobales]
	// !!!!!! VARIABLES GLOBALES !!!!!!
	// *** USER
	/////////////////////////////////////////////////////////////////////////

		//TODO Important: Les pint utilisé pour le uart doivent être configuré en mode push-pull

		// Variable pour les boucles de temps fixe
		volatile bool flagTask1ms = 0;
		int task1msTimer = 0;
		volatile bool flagTask10ms = 0;
		int task10msTimer = 0;
		float loopTiming = 10;
		int loopFreq = 100;

//		//// Variables générales



	//! [variablesglobales]
	//!
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for highPTask */
osThreadId_t highPTaskHandle;
const osThreadAttr_t highPTask_attributes = {
  .name = "highPTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for realtimeTask */
osThreadId_t realtimeTaskHandle;
const osThreadAttr_t realtimeTask_attributes = {
  .name = "realtimeTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
const osTimerAttr_t myTimer01_attributes = {
  .name = "myTimer01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void standardTasks(void *argument);
void highPriorityTask(void *argument);
void realtimePriorityTask(void *argument);
void Callback01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer01 */
  myTimer01Handle = osTimerNew(Callback01, osTimerPeriodic, NULL, &myTimer01_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(standardTasks, NULL, &defaultTask_attributes);

  /* creation of highPTask */
  highPTaskHandle = osThreadNew(highPriorityTask, NULL, &highPTask_attributes);

  /* creation of realtimeTask */
  realtimeTaskHandle = osThreadNew(realtimePriorityTask, NULL, &realtimeTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_standardTasks */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_standardTasks */
void standardTasks(void *argument)
{
  /* USER CODE BEGIN standardTasks */
  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }

	  //! [SECTION_INITIALISATION]

		// Variables
			/////////////////////////////////////////////////////////////////////
			// ! [init1mainvar]
			// Variables timing
	//			float tempsCode=0; 				// Temps de calcul du code sans print
	//			float tempsCodePrint=0; 			// Temps de calcul du code avec print
	//			float pourcentageCode=0;					// Pourcentage du temps de calcul du code sans print
	//			float pourcentageCodePrint=0;			// Pourcentage du temps de calcul du code avec print
	//			uint64_t timingCheck1;	// Temps en us depuis le départ (départ de la boucle)
	//			uint64_t timingCheck2;	// Temps en us depuis le départ (boucle après le code)
	//			uint64_t timingCheck3;	// Temps en us depuis le départ (boucle après le code et le print)
			// ! [init1mainvar]

			/////////////////////////////////////////////////////////////////////
			// ! [init2mainvar]
			// Variables generic
				int flagFirst __attribute__((unused)) = 0 ; 		// Pour indiquer la premiere fois qu'on entre dans la boucle
				int counter_a __attribute__((unused)) = 0 ;			// Compteur boucle principale





		    // ! [init2mainvar]

			/////////////////////////////////////////////////////////////////////
			// ! [init3mainvar]
			// Variables
			// *** USER
				dynamixelExample_init();

			// ! [init3mainvar]

				dynamixelExample_beforeLoop();

	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		dynamixelExample_mainLoop();

//	  //Tâches à exécuter aux 1 ms
		if (flagTask1ms)
		{
			flagTask1ms = false;

			/////////////////////////////////////////////////////////////////////
			// *** USER
			// Code utilisateur à effectuer à chaque 1 ms

			/////////////////////////////////////////////////////////////////////

		}

		//Tâches à exécuter aux 10 ms
		if (flagTask10ms)
		{
			//timingCheck1 = getTimeMicros();	// Get system tick time in us //HAL_GetTick() for ms and getTimeMicros() for us;
			flagTask10ms = 0;

			/////////////////////////////////////////////////////////////////////
			// *** USER

			dynamixelExample_task10ms();

			counter_a++;

			if (flagFirst == 0) // Premiere fois qu'on fait la boucle
			{

			}

			/////////////////////////////////////////////////////////////////////

			// Calcul du temps de code - ne pas vous occuper
			//		timingCheck2 = getTimeMicros();	// Get system tick time in us //HAL_GetTick() for ms and getTimeMicros() for us;
			//		tempsCode = (float)getMicrosDiff(timingCheck1, timingCheck2)*0.001; //en ms
			//		pourcentageCode = (tempsCode*(float)loopFreq*0.001)*100;

			/////////////////////////////////////////////////////////////////////
			// *** USER
			// Affichage série (pour debug, peut être commenté)
			//			int serialdebugonoff = 0;
			//			if (serialdebugonoff == 1)
			//			{
			//				sprintf(serialOutLine,"%d %f %d\r\n",counter_a,(float)-counter_a/100,(int)pourcentageCodePrint);
			//				HAL_UART_Transmit(&hlpuart1, (uint8_t*)serialOutLine, strlen(serialOutLine), HAL_MAX_DELAY);
			//			}

			/////////////////////////////////////////////////////////////////////
			//
			//		timingCheck3 = getTimeMicros();	// Get system tick time in us //HAL_GetTick() for ms and getTimeMicros() for us;
			//		tempsCodePrint = (float)getMicrosDiff(timingCheck1, timingCheck3)*0.001; //en us
			//		pourcentageCodePrint = (tempsCodePrint*(float)loopFreq*0.001)*100;

			flagFirst = 1;
			flagTask10ms = false;
		}
	}
  /* USER CODE END standardTasks */
}

/* USER CODE BEGIN Header_highPriorityTask */
/**
* @brief Function implementing the highPTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_highPriorityTask */
void highPriorityTask(void *argument)
{
  /* USER CODE BEGIN highPriorityTask */
  /* Infinite loop */
  for(;;)
  {
	  dynamixelExample_HighPriority_mainLoop();
	  osThreadYield();
    //osDelay(1);
  }
  /* USER CODE END highPriorityTask */
}

/* USER CODE BEGIN Header_realtimePriorityTask */
/**
* @brief Function implementing the realtimeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_realtimePriorityTask */
void realtimePriorityTask(void *argument)
{
  /* USER CODE BEGIN realtimePriorityTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END realtimePriorityTask */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */

  /* USER CODE END Callback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

uint32_t sysTick_teamAt = 0;
// Important: uniquement mettre les tâches critiques dans l'interruption, poru la majeure partie des tâches, vous pouvez définir un flag ici et le traiter dans le main
void interrupt50us()
{
	task1msTimer++;
	// 1 ms tasks.
	if (task1msTimer >= 20)
	{

		task1msTimer = 0;
		flagTask1ms = true;

		sysTick_teamAt++;
		task10msTimer++;
		if (task10msTimer >= 10)
		{
			task10msTimer = 0;
			flagTask10ms = true;
		}
	}

}
/* USER CODE END Application */

