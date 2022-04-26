/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "can.h"
#include "dac.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "usart.h"
#include "rtc.h"
#include "sai.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usb_otg.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */



/////////////////////////////////////////////////////////////////////////
// !!!!!! LIBRAIRIES !!!!!!
/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	//! [lib1include]
		// Ajouté libririe générales ici
		#include "stm32l4xx_it.h" // pour le weak 50us
		#include "stdio.h"
	//! [lib1include]

    /////////////////////////////////////////////////////////////////////
	//! [lib3include]
    // Librairies de base / applications de TeamAT
//		#include "microCounter.h"	// Pour le timer de boucle
//		#include "at_plaformAbstraction_V1_0.h" // Pour appeler entrees/sorties
//		#include "dynamixelExempleApp.h"
	//! [lib3include]

    /////////////////////////////////////////////////////////////////////
	//! [lib4include]
    // Librairies à ajouter (mettre .h et .c/.cpp dans l'arbre)
    //  *** USER


	//! [lib4include]




/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	/////////////////////////////////////////////////////////////////////////
	//! [variablesglobales]
	// !!!!!! VARIABLES GLOBALES !!!!!!
	// *** USER
	/////////////////////////////////////////////////////////////////////////

		//TODO Important: Les pint utilisé pour le uart doivent être configuré en mode push-pull

		// Variable pour les boucles de temps fixe
//		volatile bool flagTask1ms = 0;
//		int task1msTimer = 0;
//		volatile bool flagTask10ms = 0;
//		int task10msTimer = 0;
//		float loopTiming = 10;
//		int loopFreq = 100;

//		//// Variables générales



	//! [variablesglobales]

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* Configure the peripherals common clocks */
void PeriphCommonClock_Config(void);
extern void MX_FREERTOS_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */



/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */



/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system cl1ock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
    PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_LPUART1_UART_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    MX_RTC_Init();
    MX_SAI1_Init();
    MX_SDMMC1_SD_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM5_Init();
    MX_TIM15_Init();
    //MX_USB_OTG_FS_PCD_Init();
    MX_I2C2_Init();
    MX_USART2_UART_Init();
    MX_SPI3_Init();
    MX_TIM1_Init();
    MX_TIM16_Init();
    MX_DAC1_Init();
    MX_CAN1_Init();
    MX_FATFS_Init();
    MX_TIM7_Init();
    MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */


//  //! [SECTION_INITIALISATION]
//
//	// Variables
//		/////////////////////////////////////////////////////////////////////
//		// ! [init1mainvar]
//		// Variables timing
////			float tempsCode=0; 				// Temps de calcul du code sans print
////			float tempsCodePrint=0; 			// Temps de calcul du code avec print
////			float pourcentageCode=0;					// Pourcentage du temps de calcul du code sans print
////			float pourcentageCodePrint=0;			// Pourcentage du temps de calcul du code avec print
////			uint64_t timingCheck1;	// Temps en us depuis le départ (départ de la boucle)
////			uint64_t timingCheck2;	// Temps en us depuis le départ (boucle après le code)
////			uint64_t timingCheck3;	// Temps en us depuis le départ (boucle après le code et le print)
//		// ! [init1mainvar]
//
//		/////////////////////////////////////////////////////////////////////
//		// ! [init2mainvar]
//		// Variables generic
//			int flagFirst __attribute__((unused)) = 0 ; 		// Pour indiquer la premiere fois qu'on entre dans la boucle
//			int counter_a __attribute__((unused)) = 0 ;			// Compteur boucle principale
//
//
//
//
//
//	    // ! [init2mainvar]
//
//		/////////////////////////////////////////////////////////////////////
//		// ! [init3mainvar]
//		// Variables
//		// *** USER
//			dynamixelExample_init();
//
//		// ! [init3mainvar]


	// Démarage du timer pour les boucles de temps fixe
	if (HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)
	{
		Error_Handler();
	}

  //! [SECTION_INITIALISATION]

	// ! [preLoopSetup]
	//dynamixelExample_beforeLoop();



	// ! [preLoopSetup]

  /* USER CODE END 2 */
	  /* Init scheduler */
	  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
	  MX_FREERTOS_Init();
	  /* Start scheduler */
	  osKernelStart();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



//	while (1)
//	{
//    /* USER CODE END WHILE */
//
//	/* USER CODE BEGIN 3 */
//
//		dynamixelExample_mainLoop();
//
////	  //Tâches à exécuter aux 1 ms
//	  if(flagTask1ms)
//	  {
//		  flagTask1ms = false;
//
//		  /////////////////////////////////////////////////////////////////////
//		  // *** USER
//		  // Code utilisateur à effectuer à chaque 1 ms
//
//
//
//		  /////////////////////////////////////////////////////////////////////
//
//	  }
//
//	  //Tâches à exécuter aux 10 ms
//	  if(flagTask10ms)
//	  {
//		//timingCheck1 = getTimeMicros();	// Get system tick time in us //HAL_GetTick() for ms and getTimeMicros() for us;
//		flagTask10ms=0;
//
//		/////////////////////////////////////////////////////////////////////
//		// *** USER
//
//		dynamixelExample_task10ms();
//
//		counter_a ++;
//
//		if (flagFirst == 0) // Premiere fois qu'on fait la boucle
//		{
//
//		}
//
//		/////////////////////////////////////////////////////////////////////
//
//
//		// Calcul du temps de code - ne pas vous occuper
////		timingCheck2 = getTimeMicros();	// Get system tick time in us //HAL_GetTick() for ms and getTimeMicros() for us;
////		tempsCode = (float)getMicrosDiff(timingCheck1, timingCheck2)*0.001; //en ms
////		pourcentageCode = (tempsCode*(float)loopFreq*0.001)*100;
//
//		/////////////////////////////////////////////////////////////////////
//		// *** USER
//		// Affichage série (pour debug, peut être commenté)
////			int serialdebugonoff = 0;
////			if (serialdebugonoff == 1)
////			{
////				sprintf(serialOutLine,"%d %f %d\r\n",counter_a,(float)-counter_a/100,(int)pourcentageCodePrint);
////				HAL_UART_Transmit(&hlpuart1, (uint8_t*)serialOutLine, strlen(serialOutLine), HAL_MAX_DELAY);
////			}
//
//		/////////////////////////////////////////////////////////////////////
////
////		timingCheck3 = getTimeMicros();	// Get system tick time in us //HAL_GetTick() for ms and getTimeMicros() for us;
////		tempsCodePrint = (float)getMicrosDiff(timingCheck1, timingCheck3)*0.001; //en us
////		pourcentageCodePrint = (tempsCodePrint*(float)loopFreq*0.001)*100;
//
//
//		flagFirst = 1;
//		flagTask10ms = false;
//}
//
//
//}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */






//// Important: uniquement mettre les tâches critiques dans l'interruption, poru la majeure partie des tâches, vous pouvez définir un flag ici et le traiter dans le main
//void interrupt50us()
//{
//	task1msTimer++;
//	// 1 ms tasks.
//	if (task1msTimer >= 20)
//	{
//		task1msTimer = 0;
//		flagTask1ms = true;
//
//		task10msTimer++;
//		if (task10msTimer >= 10)
//		{
//			task10msTimer = 0;
//			flagTask10ms = true;
//		}
//	}
//
//}

/* USER CODE END 4 */



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
