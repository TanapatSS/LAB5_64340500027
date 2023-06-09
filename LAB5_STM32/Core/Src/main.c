/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
int LED_speed = 500;
uint8_t RxBuffer[1];
uint8_t TxBuffer[1000];
int state = 0;
int press_button = 0;
int LEDset = 0;
int button = 1;
int Hz = 1;
int press = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void welcome();
void LEDControl();
void UARTInterruptConfig();
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  welcome();
  UARTInterruptConfig();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	static uint32_t timestamp = 0;
	if(HAL_GetTick()>=timestamp)
	{
		timestamp = HAL_GetTick()+LED_speed;
		if (LEDset == 0)
		{
			HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
		}
		else
		{
			HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,0);
		}
	}
	press_button = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	if(press_button == 0 && button == 1 && state == 2 && press == 0)
	{
		sprintf((char*)TxBuffer,"\r\n---------------------------------------------------\r\n"
								"0o  Button is pressed  o0\r\n"
								"---------------------------------------------------\r\n");
		HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*) TxBuffer),1000);
		press = 1;
	}
	else if(press_button == 1 && button == 1 && state == 2 && press == 1)
	{
		sprintf((char*)TxBuffer,"\r\n---------------------------------------------------\r\n"
								"0o  Button is unpressed  o0\r\n"
								"---------------------------------------------------\r\n");
		HAL_UART_Transmit(&huart2, TxBuffer, strlen((char*) TxBuffer),1000);
		press = 0;
	}
	button = press_button;
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_2;
  huart2.Init.Parity = UART_PARITY_EVEN;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void welcome()
{
	  uint8_t text[] ="\r\n---------------------------------------------------\r\n"
					  "*Welcome to LED speed control application*\r\n"
				   	  "-->press 0 to enter LED control\r\n"
					  "-->press 1 to enter button status\r\n"
					  "---------------------------------------------------\r\n";  //30
	  HAL_UART_Transmit(&huart2, text, 218, 1000);
}

void LEDControl()
{
	static uint32_t timestamp = 0;
	if(HAL_GetTick()>=timestamp)
	{
		timestamp = HAL_GetTick()+LED_speed;
		HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	}
}

void UARTInterruptConfig()
{
	HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{

		RxBuffer[1] = '\0';
		switch (state)
		{
		case 0:
			if (RxBuffer[0] == 48) //0
			{
				sprintf((char*)TxBuffer,"\r\nReceived : %s\r\n"
										    "---------------------------------------------------\r\n"
											"-->press a to speed up +1Hz\r\n"
											"-->press s to speed down -1Hz\r\n"
											"-->press d to On/Off LED\r\n"
											"-->press x to go back\r\n"
											"---------------------------------------------------\r\n",RxBuffer);
				state = 1;
				break;
			}
			if(RxBuffer[0] == 49) //1
			{
				sprintf((char*) TxBuffer,"\r\nReceived : %s\r\n"
										     "---------------------------------------------------\r\n"
											 "-->press a blue button to show status\r\n"
										     "-->press x to go back\r\n"
											 "---------------------------------------------------\r\n",RxBuffer);
				state = 2;
				break;
			}
			if(RxBuffer[0] != 48 || RxBuffer[0] != 49)
			{
				sprintf((char*) TxBuffer, "\r\nReceived : %s\r\n"
										  "---------------------------------------------------\r\n"
										  "xxx Wrong Button xxx\r\n"
										  "---------------------------------------------------\r\n",RxBuffer);
				break;
			}
		case 1:
			if (RxBuffer[0] == 120) //x
			{
				sprintf((char*)TxBuffer,"\r\nReceived : %s\r\n"
										"---------------------------------------------------\r\n"
										"*Welcome to LED speed control application*\r\n"
										"-->press 0 to enter LED control\r\n"
										"-->press 1 to enter button status\r\n"
										"---------------------------------------------------\r\n",RxBuffer);
				state = 0;
				break;
			}
			if(RxBuffer[0] == 97) //a
			{
				Hz += 1;
				if(Hz > 0)
				{
					LED_speed = 500/Hz; //1Hz = 500 ms, 2Hz = 250 ms
				}
				else
				{
					LED_speed = 0;
				}
				sprintf((char*)TxBuffer,"\r\nReceived : %s\r\n"
										"---------------------------------------------------\r\n"
										"LED speed = %d\r\n"
										"---------------------------------------------------\r\n", RxBuffer, LED_speed);
				break;
			}
			if(RxBuffer[0] == 115) //s
			{
				Hz -= 1;
				if(Hz > 0)
				{
					LED_speed = 500/Hz; //1Hz = 500 ms, 2Hz = 250 ms
				}
				else
				{
					LED_speed = 0;
				}
				sprintf((char*) TxBuffer,"\r\nReceived : %s\r\n"
										 "---------------------------------------------------\r\n"
										 "LED speed = %d\r\n"
										 "---------------------------------------------------\r\n", RxBuffer, LED_speed);
				break;
			}
			if(RxBuffer[0] == 100) //d
			{
				if(LEDset == 1)
				{
					sprintf((char*) TxBuffer, "\r\nReceived : %s\r\n"
											  "---------------------------------------------------\r\n"
											  "LED ON\r\n"
											  "---------------------------------------------------\r\n", RxBuffer);
					LEDset = 0;
				}
				else if(LEDset == 0)
				{
					sprintf((char*) TxBuffer, "\r\nReceived : %s\r\n"
											  "---------------------------------------------------\r\n"
											  "LED OFF\r\n"
											  "---------------------------------------------------\r\n", RxBuffer);
					LEDset = 1;
				}
				break;
			}
			else
			{
				sprintf((char*) TxBuffer, "\r\nReceived : %s\r\n"
										  "---------------------------------------------------\r\n"
										  "xxx Wrong Button xxx\r\n"
										  "---------------------------------------------------\r\n",RxBuffer);
				break;
			}
		case 2:
			if(RxBuffer[0] == 120) //x
			{
				sprintf((char*)TxBuffer,"\r\nReceived : %s\r\n"
										"---------------------------------------------------\r\n"
										"*Welcome to LED speed control application*\r\n"
										"-->press 0 to enter LED control\r\n"
										"-->press 1 to enter button status\r\n"
										"---------------------------------------------------\r\n",RxBuffer);
				state = 0;
				break;
			}
			else
			{
				sprintf((char*) TxBuffer, "\r\nReceived : %s\r\n"
										  "---------------------------------------------------\r\n"
										  "xxx Wrong Button xxx\r\n"
										  "---------------------------------------------------\r\n",RxBuffer);
				break;
			}
		}
		HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*) TxBuffer));
		HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
	}
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
