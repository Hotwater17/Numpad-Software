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
#include "MatrixKeyboard.c"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define REPORT_SIZE	8
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE END PFP */

/* Private user code ---------------- -----------------------------------------*/
/* USER CODE BEGIN 0 */

void User_Key_Init()
{
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	/*
	 * Rows
	 */
	GPIO_InitTypeDef keyGPIOStruct;
	keyGPIOStruct.Mode = GPIO_MODE_INPUT;
	keyGPIOStruct.Pull = GPIO_PULLDOWN;
	keyGPIOStruct.Pin = ROW4_Pin | ROW3_Pin | ROW2_Pin | ROW1_Pin | ROW0_Pin;
	HAL_GPIO_Init(ROWS_GPIO_Port, &keyGPIOStruct);

	/*
	 * Columns
	 */
	keyGPIOStruct.Mode = GPIO_MODE_OUTPUT_PP;
	keyGPIOStruct.Pull = GPIO_NOPULL;
	keyGPIOStruct.Pin = COL0_Pin | COL1_Pin | COL2_Pin | COL3_Pin;
	HAL_GPIO_Init(COLUMNS_GPIO_Port, &keyGPIOStruct);

}

void User_Key_Debounce(uint16_t time)
{
	HAL_Delay(time);
}

void User_Key_Write(uint8_t column, bool state)
{
	switch(column)
	{
		case 0:
		{
			HAL_GPIO_WritePin(COLUMNS_GPIO_Port, COL0_Pin, state);
			break;
		}
		case 1:
		{
			HAL_GPIO_WritePin(COLUMNS_GPIO_Port, COL1_Pin, state);
			break;
		}
		case 2:
		{
			HAL_GPIO_WritePin(COLUMNS_GPIO_Port, COL2_Pin, state);
			break;
		}
		case 3:
		{
			HAL_GPIO_WritePin(COLUMNS_GPIO_Port, COL3_Pin, state);
			break;
		}
		default:
		{
			break;
		}
	}
}



bool User_Key_Read(uint8_t row)
{
	bool state = false;

	switch(row)
	{
		case 0:
		{
			state = HAL_GPIO_ReadPin(ROWS_GPIO_Port, ROW0_Pin);
			break;
		}
		case 1:
		{
			state = HAL_GPIO_ReadPin(ROWS_GPIO_Port, ROW1_Pin);
			break;
		}
		case 2:
		{
			state = HAL_GPIO_ReadPin(ROWS_GPIO_Port, ROW2_Pin);
			break;
		}
		case 3:
		{
			state = HAL_GPIO_ReadPin(ROWS_GPIO_Port, ROW3_Pin);
			break;
		}
		case 4:
		{
			state = HAL_GPIO_ReadPin(ROWS_GPIO_Port, ROW4_Pin);
			break;
		}
		default:
		{
			break;
		}
	}

	return state;

}

void User_Key_HID_Init()
{
	MX_USB_DEVICE_Init();
}

uint8_t User_Key_HID_Send(uint8_t *report)
{
	USBD_HID_SendReport(&hUsbDeviceFS, report, REPORT_SIZE);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	keyboard_gpio_config_t keyGpioInit;
	keyboard_hid_config_t hidInit;
	pressed_keys_array_t keysArray;
	uint8_t pressedKeysNumber = 0;
	uint8_t temporaryReport[REPORT_SIZE] = {0};
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


  /* USER CODE BEGIN 2 */
  keyGpioInit.keyboardGPIOInitFunction = User_Key_Init;
  keyGpioInit.keyboardDebounceFunction = User_Key_Debounce;
  keyGpioInit.keyboardGPIOReadFunction = User_Key_Read;
  keyGpioInit.keyboardGPIOWriteFunction = User_Key_Write;

  hidInit.keyboardHIDInit = User_Key_HID_Init;
  hidInit.keyboardHIDSend = User_Key_HID_Send;

  Keyboard_GPIO_Init(&keyGpioInit);
  Keyboard_HID_Init(&hidInit);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
	  pressedKeysNumber = Keyboard_Matrix_Read(&keysArray);
	  for(uint8_t cnt = 0; cnt < REPORT_SIZE; cnt++)
	  {
		  temporaryReport[cnt+2] = keysArray.actualArray[cnt];
	  }
	  Keyboard_HID_Send(temporaryReport);

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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, COL0_Pin|COL1_Pin|COL2_Pin|COL3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : COL0_Pin COL1_Pin COL2_Pin COL3_Pin */
  GPIO_InitStruct.Pin = COL0_Pin|COL1_Pin|COL2_Pin|COL3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ROW4_Pin ROW3_Pin ROW2_Pin ROW1_Pin 
                           ROW0_Pin */
  GPIO_InitStruct.Pin = ROW4_Pin|ROW3_Pin|ROW2_Pin|ROW1_Pin 
                          |ROW0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
