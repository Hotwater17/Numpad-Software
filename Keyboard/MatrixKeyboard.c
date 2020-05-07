/*
 * MatrixKeyboard.c
 *
 *  Created on: 20 mar 2020
 *      Author: Hotwater
 */

#include "MatrixKeyboard.h"



uint8_t keyboardIDMap[KEYBOARD_ROWS][KEYBOARD_COLUMNS] =
{
		//COL 0					COL 1				COL 2				COL 3
		{KEY_ID_PAD_NUMLOCK,	KEY_ID_PAD_SLASH,	KEY_ID_PAD_STAR,	KEY_ID_PAD_MINUS},	//ROW 0
		{KEY_ID_PAD_7,			KEY_ID_PAD_8,		KEY_ID_PAD_9,		KEY_ID_PAD_PLUS},	//ROW 1
		{KEY_ID_PAD_4,			KEY_ID_PAD_5,		KEY_ID_PAD_6,		KEY_ID_NOKEY},		//ROW 2
		{KEY_ID_PAD_1,			KEY_ID_PAD_2,		KEY_ID_PAD_3,		KEY_ID_PAD_ENTER},	//ROW 3
		{KEY_ID_PAD_0,			KEY_ID_PAD_DOT,		KEY_ID_NOKEY,		KEY_ID_NOKEY}		//ROW 4
};

void Keyboard_GPIO_Init(const keyboard_gpio_config_t *config)
{
	keyboardUserGPIOConfig.keyboardDebounceFunction = config->keyboardDebounceFunction;
	keyboardUserGPIOConfig.keyboardGPIOInitFunction = config->keyboardGPIOInitFunction;
	keyboardUserGPIOConfig.keyboardGPIOReadFunction = config->keyboardGPIOReadFunction;
	keyboardUserGPIOConfig.keyboardGPIOWriteFunction = config->keyboardGPIOWriteFunction;

	keyboardUserGPIOConfig.keyboardGPIOInitFunction();
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

void Keyboard_HID_Send(uint8_t *report)
{
	keyboardUserHIDConfig.keyboardHIDSend(report);
}

void Keyboard_Debounce(const uint16_t time)
{
	keyboardUserGPIOConfig.keyboardDebounceFunction(time);
}


bool Keyboard_Row_Read(const uint8_t row)
{
	return keyboardUserGPIOConfig.keyboardGPIOReadFunction(row);
}

uint8_t Keyboard_Matrix_Read(pressed_keys_array_t *pressedArray)
{
	uint8_t pressedKeysCounter = 0;
	uint8_t	columnCounter = 0;
	uint8_t rowCounter = 0;
	uint8_t flushArrayIndex = 0;

	for(flushArrayIndex = 0; flushArrayIndex < KEYBOARD_NUMBER_OF_KEYS; flushArrayIndex++)
	{
		pressedArray->actualArray[flushArrayIndex] = KEY_ID_NOKEY;
	}

	for(columnCounter = 0; columnCounter < KEYBOARD_COLUMNS; columnCounter++)
	{
		Keyboard_Column_Activate(columnCounter);
		//Keyboard_Debounce(KEYBOARD_DEBOUNCE_MS);
		for(rowCounter = 0; rowCounter < KEYBOARD_ROWS; rowCounter++)
		{
			if(Keyboard_Row_Read(rowCounter) == true)
			{
				pressedArray->actualArray[pressedKeysCounter] = keyboardIDMap[rowCounter][columnCounter];
				pressedKeysCounter++;
			}
		}
		Keyboard_Column_Deactivate(columnCounter);
	}

	return pressedKeysCounter;

}


