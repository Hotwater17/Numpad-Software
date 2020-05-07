/*
 * MatrixKeyboard.h
 *
 *  Created on: 20 mar 2020
 *      Author: Hotwater
 */

#include <stdbool.h>

#ifndef __MATRIX_KEYBOARD_H
#define __MATRIX_KEYBOARD_H

#include "KeysUseID.h"
#include "KeyboardMap.h"

#define KEYBOARD_NUMBER_OF_KEYS	17
#define KEYBOARD_COLUMNS	4
#define KEYBOARD_ROWS		5

#define KEYBOARD_PRESSED_SIZE	KEYBOARD_NUMBER_OF_KEYS
#define KEYBOARD_DEBOUNCE_MS	30

#define KEYBOARD_COLUMN_ACTIVE		true
#define KEYBOARD_COLUMN_NOT_ACTIVE	false


typedef struct
{
	void (*keyboardHIDInit)();
	uint8_t (*keyboardHIDSend)(uint8_t *report);
}keyboard_hid_config_t;

typedef struct
{
	void (*keyboardGPIOInitFunction)();
	void (*keyboardGPIOWriteFunction)(const uint8_t column, const bool state);
	void (*keyboardDebounceFunction)(const uint16_t time);
	bool (*keyboardGPIOReadFunction)(const uint8_t row);
}keyboard_gpio_config_t;



keyboard_gpio_config_t	keyboardUserGPIOConfig;
keyboard_hid_config_t	keyboardUserHIDConfig;

typedef struct
{
	uint8_t actualArray[KEYBOARD_PRESSED_SIZE];
}pressed_keys_array_t ;

void Keyboard_GPIO_Init(const keyboard_gpio_config_t *config);
void Keyboard_Column_Activate(const uint8_t column);
void Keyboard_Column_Deactivate(const uint8_t column);
void Keyboard_HID_Init(const keyboard_hid_config_t *config);
void Keyboard_HID_Send(uint8_t *report);
void Keyboard_Debounce(const uint16_t time);
bool Keyboard_Row_Read(const uint8_t row);
uint8_t Keyboard_Matrix_Read(pressed_keys_array_t *pressedArray);

#endif

