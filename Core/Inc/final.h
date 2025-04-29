/*
 * final.h
 *
 *  Created on: Apr 23, 2025
 *      Author: jakeb
 */

#ifndef INC_FINAL_H_
#define INC_FINAL_H_

#include <stdio.h>
#include "gameplay.h"
#include "LCD_Driver.h"


#define GAME_ACTIVE 0
#define GAME_WIN 1
#define GAME_DRAW 2

// Displays the final game screen with game results.
void show_final_screen(uint8_t game_state, uint8_t current_player, 
                       uint32_t red_score, uint32_t yellow_score,
                       uint32_t game_time_seconds);
// Processes touch input on the final screen.
uint8_t process_final_screen_touch(uint16_t x, uint16_t y);

#endif /* INC_FINAL_H_ */
