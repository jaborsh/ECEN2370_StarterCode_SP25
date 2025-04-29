/*
 * button_driver.c
 *
 *  Created on: Apr 28, 2025
 *      Author: jakeb
 */

#include "button_driver.h"

void button_init(void) {
    GPIO_InitTypeDef button_config;
    
    USER_BUTTON_GPIO_CLK_ENABLE();
    button_config.Pin = USER_BUTTON_PIN;
    button_config.Mode = GPIO_MODE_INPUT;
    button_config.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USER_BUTTON_GPIO_PORT, &button_config);
}

uint8_t button_get_state(void) {
    GPIO_PinState state = HAL_GPIO_ReadPin(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
    return (state == GPIO_PIN_SET) ? BUTTON_PRESSED : BUTTON_RELEASED;
}

uint8_t button_wait_for_press(uint32_t timeout_ms) {
    uint32_t start_time = HAL_GetTick();
    
    while ((HAL_GetTick() - start_time) < timeout_ms) {
        if (button_get_state() == BUTTON_PRESSED) {
            HAL_Delay(20);
            if (button_get_state() == BUTTON_PRESSED) {
                return BUTTON_PRESSED;
            }
        }
        HAL_Delay(10);
    }
    
    return BUTTON_RELEASED;
}

uint8_t button_wait_for_release(uint32_t timeout_ms) {
    uint32_t start_time = HAL_GetTick();
    
    while ((HAL_GetTick() - start_time) < timeout_ms) {
        if (button_get_state() == BUTTON_RELEASED) {
            HAL_Delay(20);
            if (button_get_state() == BUTTON_RELEASED) {
                return BUTTON_RELEASED;
            }
        }
        HAL_Delay(10);
    }
    
    return BUTTON_PRESSED;
}


