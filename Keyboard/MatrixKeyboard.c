/*
 * MatrixKeyboard.c
 *
 *  Created on: 20 mar 2020
 *      Author: Hotwater
 */

#include "MatrixKeyboard.h"


void Keyboard_GPIO_Init(const keyboard_gpio_config_t *config)
{
	keyboardUserGPIOConfig.keyboardDebounceFunction = config->keyboardDebounceFunction;
	keyboardUserGPIOConfig.keyboardGPIOInitFunction = config->keyboardGPIOInitFunction;
	keyboardUserGPIOConfig.keyboardGPIOReadFunction = config->keyboardGPIOReadFunction;
	keyboardUserGPIOConfig.keyboardGPIOWriteFunction = config->keyboardGPIOWriteFunction;

	keyboardUserGPIOConfig.keyboardDebounceFunction();
}

void Keyboard_Column_Activate(const uint8_t column)
{
	keyboardUserGPIOConfig.keyboardGPIOWriteFunction(column, KEYBOARD_COLUMN_ACTIVE);
}

void Keyboard_Column_Deactivate(const uint8_t column)
{
	keyboardUserGPIOConfig.keyboardGPIOWriteFunction(column, KEYBOARD_COLUMN_NOT_ACTIVE);
}

void Keyboard_HID_Init(const keyboard_hid_config_t *config)
{
	keyboardUserHIDConfig.keyboardHIDInit = config->keyboardHIDInit;
	keyboardUserHIDConfig.keyboardHIDSend = config->keyboardHIDSend;

	keyboardUserHIDConfig.keyboardHIDInit();
}

void Keyboard_HID_Send()
{
	keyboardUserHIDConfig.keyboardHIDSend();
}

inline void Keyboard_Debounce(const uint16_t time)
{
	keyboardUserGPIOConfig.keyboardDebounceFunction();
}


bool Keyboard_Row_Read(const uint8_t row)
{
	keyboardUserGPIOConfig.keyboardGPIOReadFunction();
}

uint8_t Keyboard_Matrix_Read(pressed_keys_array_t *pressedArray)
{
	uint8_t pressedKeysCounter = 0;
	uint8_t	columnCounter = 0;
	uint8_t rowCounter = 0;
	uint8_t flushArrayIndex = 0;

	for(flushArrayIndex = 0; flushArrayIndex < KEYBOARD_NUMBER_OF_KEYS; flushArrayIndex++)
	{
		*pressedArray[flushArrayIndex] = KEY_ID_NOKEY;
	}

	for(columnCounter = 0; columnCounter < KEYBOARD_COLUMNS; columnCounter++)
	{
		Keyboard_Column_Activate(columnCounter);
		Keyboard_Debounce(KEYBOARD_DEBOUNCE_MS);
		for(rowCounter = 0; rowCounter < KEYBOARD_ROWS; rowCounter++)
		{
			if(Keyboard_Row_Read(rowCounter) == true)
			{
				*pressedArray[pressedKeysCounter] = keyboardIDMap[rowCounter][columnCounter];
				pressedKeyCounter++;
			}
		}
		Keyboard_Column_Deactivate(columnCounter);
	}

	return pressedKeysCounter;

}


