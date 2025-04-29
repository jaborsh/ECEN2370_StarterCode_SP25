/*
 * button_driver.h
 *
 *  Created on: Apr 28, 2025
 *      Author: jakeb
 */

#ifndef INC_BUTTON_DRIVER_H_
#define INC_BUTTON_DRIVER_H_

#include "stm32f4xx_hal.h"

#define USER_BUTTON_PIN GPIO_PIN_0
#define USER_BUTTON_GPIO_PORT GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BUTTON_RELEASED 0
#define BUTTON_PRESSED 1

// Initialize the user button GPIO pin.
void button_init(void);
// Get the current state of the user button.
uint8_t button_get_state(void);
// Wait for the user button to be pressed.
uint8_t button_wait_for_press(uint32_t timeout_ms);
// Wait for the user button to be released.
uint8_t button_wait_for_release(uint32_t timeout_ms);

#endif /* INC_BUTTON_DRIVER_H_ */
