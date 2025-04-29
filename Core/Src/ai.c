/*
 * ai.c
 *
 *  Created on: Apr 28, 2025
 *      Author: jakeb
 */

#include "ai.h"


static uint32_t ai_seed;

void init_ai_random(void) {
    ai_seed = HAL_GetTick();
    srand(ai_seed);
}

uint8_t ai_get_move(uint8_t board[BOARD_ROWS][BOARD_COLS], uint8_t difficulty) {
    uint8_t available_cols[BOARD_COLS];
    uint8_t num_available = 0;
    
    for (uint8_t col = 0; col < BOARD_COLS; col++) {
        if (board[0][col] == PLAYER_NONE) {
            available_cols[num_available++] = col;
        }
    }
    
    if (num_available == 0) {
        return BOARD_COLS;
    }
    
    uint8_t random_index = rand() % num_available;
    return available_cols[random_index];
}


