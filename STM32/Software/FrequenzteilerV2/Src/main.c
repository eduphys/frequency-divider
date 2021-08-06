/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usbd_cdc_if.h"
#include "HMCWriter.h"
#include "SerialCommunication.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

int delay = 100;

//initial set
unsigned int divider = 32;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void send_message(char text[]);
void process_buffer();
unsigned int set_divider(unsigned int divider);
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

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /*
   * Set divider of the NB6N239S chip. Divider can be 1,2,4,8
   * See the following table to set
   * SELA1 SELA0  	QA Outputs
	 L		L		Divide by 1
	 L		H		Divide by 2
	 H		L		Divide by 4
	 H		H		Divide by 8

	 High (H) is GPIO_PIN_SET, Low (L) is GPIO_PIN_RESET
   */
  HAL_GPIO_WritePin(SELA1_GPIO_Port, SELA1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SELA0_GPIO_Port, SELA0_Pin, GPIO_PIN_RESET);


  // set GPO of HMC to 1
  writeHMCReg(0x05,0x01);

  // what does that?
  writeHMCReg(0x04,0x08);

  // divider settings
  // 0: /1
  //1: /2
  //2: /4
  //3: /8
  //4: /16
  //5: /32
  //6: N/A
  //7: N/A
  writeHMCReg(0x02,0x05);

  // sleep
  // maybe best to use Stop mode instead of Standby mode. See STM32F072x8/xB manual, page 15
  //HAL_PWR_EnterStopMode();

  send_message("...Initialization complete\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if (SerialComm_EndReached()) {
		//debug
		//send_message("Cmd received\n");

		process_buffer();

		// restart buffering
		SerialComm_StartBuffering();

	}



	// send "Hello World every 5000 milliseconds
//	if (5000 - counter*delay < delay) {
//		send_message("I'm alive\n");
//		counter = 0;
//	} else {
//		++counter;
//	}

	HAL_Delay(delay);
  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void send_message(char text[])
{
	uint16_t len = strlen((char *)text);
	CDC_Transmit_FS((uint8_t*)text, len);
	HAL_Delay(delay);
}


// where the magic happens
void process_buffer()
{
	// process the buffer. This function is called  in main loop
	// when the buffering stopped, i.e. endline received
	// then this function process the buffer and
	// implements the protocoll
	//
	//pointer to cmd buffer
	uint8_t * buffer;
	buffer = SerialComm_Cmd_Buffer();
	// length
	uint32_t len = 0;
	len = SerialComm_Cmd_Buffer_Len();

	char text[12];

	unsigned int new_divider;

	for (int i = 0; i < len; ++i) {

		switch (buffer[i]) {
		case 'd':

			sprintf(text, "%u\n", divider);
			send_message(text);

			return;

		case 'D':
//			send_message("Set divider cmd found\n");

			//read out buffer
			//first byte is 'D', so ignore
			sscanf(&buffer[1],"%u\n",&new_divider);

			//debug
//			CDC_Transmit_FS(buffer, len);
//			HAL_Delay(delay);


			if (!set_divider(new_divider)) {
				sprintf(text, "%u\n", new_divider);
				send_message(text);
			}
			return;
		}

	}


}


unsigned int set_divider(unsigned int new_divider)
{
//	uint8_t text[36];
//	sprintf(text, "new Divider %d\n", new_divider);
//	send_message(text);
//	return;

	if ((new_divider%2 != 0 && new_divider != 1) || new_divider > 32 || new_divider < 1)
	{
		char text[36];
		sprintf(text, "Divider %d invalid\n", new_divider);
		send_message(text);
		return 1;
	}


	//make log 2 of divider
	unsigned int HMC_divider_set = 0;
	unsigned int temp = new_divider;
	unsigned int divider2store = 1;

	// make shifts (log2 and power 2)
	while (temp >>= 1) {
		HMC_divider_set++;
		divider2store <<= 1;
	}

	// check log 2
	if (divider2store != new_divider) {
		char text[36];
		sprintf(text, "Divider %d invalid, not equal %d\n", new_divider, divider2store);
		send_message(text);
		return 1;
	}

	// set global divider information
	divider = new_divider;


	// divider settings
	// 0: /1
	//1: /2
	//2: /4
	//3: /8
	//4: /16
	//5: /32
	//6: N/A
	//7: N/A
	writeHMCReg(0x02,HMC_divider_set);

	return 0;

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	uint8_t buffer[] = "Error happened with HAL\r\n";
	CDC_Transmit_FS(buffer, sizeof(buffer));
	HAL_Delay(1000);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
