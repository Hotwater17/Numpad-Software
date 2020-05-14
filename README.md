# Numpad Software
Mechanical numpad software written in C for STM32F072 in CubeIDE
You are free to use the MartixKeyboard library.
For this you will need to create hardware specific user functions and point to them using keyboard_gpio_config_t keyGpioInit
	keyboard_hid_config_t type structures. Then call Keyboard_GPIO_Init and Keyboard_HID_Init with pointers to these structures as variables.
 In order to read the keyboard, call Keyboard_Read_Matrix with pressed_keys_array_t type array as an argument.
 Pressed keys IDs will be written to this array. 
