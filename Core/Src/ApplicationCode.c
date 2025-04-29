/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

extern void initialise_monitor_handles(void); 

static STMPE811_TouchData StaticTouchData;

void ApplicationInit(void) {
	initialise_monitor_handles(); // Allows printf functionality
	button_init();
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);
	InitializeLCDTouch();
	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;
}

void Connect4(void) {
	LCD_ShowMenu();
	
	while (1) {
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			uint8_t selected_mode;
			if (StaticTouchData.y > LCD_PIXEL_HEIGHT / 2) {
				selected_mode = GAME_MODE_ONE_PLAYER;
			} else {
				selected_mode = GAME_MODE_TWO_PLAYER;
			}
			
			init_game(selected_mode);
			
			// Game loop
			while (1) {
				while (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
					HAL_Delay(10);
				}
				
				HAL_Delay(100);
				
				if (game_state != GAME_ACTIVE) {
					show_final_screen(game_state, current_player, red_score, yellow_score, game_time_seconds);
					
					while (1) {
						while (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
							HAL_Delay(10);
						}
						
						while (returnTouchStateAndLocation(&StaticTouchData) != STMPE811_State_Pressed) {
							HAL_Delay(10);
						}
						
						uint8_t action = process_final_screen_touch(StaticTouchData.x, StaticTouchData.y);
						if (action == NEW_GAME) {
							reset_game(SAVE_STATS);
							init_game(selected_mode);
							break;
						} else if (action == RESTART_GAME) {
							reset_game(NO_SAVE_STATS);
							init_game(selected_mode);
							break;
						}
							
						while (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
							HAL_Delay(10);
						}
					}
					
					continue;
				}
				
				if (selected_mode == GAME_MODE_TWO_PLAYER || 
					(selected_mode == GAME_MODE_ONE_PLAYER && current_player == PLAYER_RED)) {
					
					if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
						process_touch_for_column_select(StaticTouchData.x);
						while (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
							HAL_Delay(10);
						}
						
						HAL_Delay(100);
					}
					
					if (button_get_state() == BUTTON_PRESSED) {
						place_piece_in_current_column();
						while (button_get_state() == BUTTON_PRESSED) {
							HAL_Delay(10);
						}
						
						HAL_Delay(300);
					}
				}
				HAL_Delay(10);
			}
			LCD_ShowMenu();
		}
		HAL_Delay(50);
	}
}

