/*
 * final.c
 *
 *  Created on: Apr 23, 2025
 *      Author: jakeb
 */

#include "final.h"

void show_final_screen(uint8_t game_state, uint8_t current_player, 
                       uint32_t red_score, uint32_t yellow_score,
                       uint32_t game_time_seconds) {
    LCD_Clear(0, LCD_COLOR_WHITE);
    
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);
    
    if (game_state == GAME_WIN) {
        if (current_player == 1) {
            LCD_DisplayString(50, 40, "RED WINS!");
        } else {
            LCD_DisplayString(30, 40, "YELLOW WINS!");
        }
    } else if (game_state == GAME_DRAW) {
        LCD_DisplayString(70, 40, "DRAW!");
    }
    
    char score_text[30];
    sprintf(score_text, "RED: %lu", red_score);
    LCD_DisplayString(20, 90, score_text);
    
    sprintf(score_text, "YELLOW: %lu", yellow_score);
    LCD_DisplayString(20, 120, score_text);
    
    sprintf(score_text, "Time: %lus", game_time_seconds);
    LCD_DisplayString(20, 160, score_text);
    
    LCD_Draw_FillRectangle(30, 200, 180, 40, LCD_COLOR_GREEN);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DisplayString(45, 210, "NEW GAME");
    
    LCD_Draw_FillRectangle(30, 250, 180, 40, LCD_COLOR_RED);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayString(45, 260, "RESTART");
}

uint8_t process_final_screen_touch(uint16_t x, uint16_t y) {
    // Dividing by 5 to have a rough approximation of the button location
    if (y >= LCD_PIXEL_HEIGHT / 5) {
        return NEW_GAME;
    } else {
        return RESTART_GAME;
    }
}


