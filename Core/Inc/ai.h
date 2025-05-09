/*
 * ai.h
 *
 *  Created on: Apr 28, 2025
 *      Author: jakeb
 */

#ifndef INC_AI_H_
#define INC_AI_H_

#include <stdlib.h>
#include <time.h>
#include "gameplay.h"
#include "stm32f4xx_hal.h"

#define BOARD_ROWS 6
#define BOARD_COLS 7

// Initializes the random number generator for AI moves.
void init_ai_random(void);
// Determines the AI's next move randomly.
uint8_t ai_get_move(uint8_t board[BOARD_ROWS][BOARD_COLS]);

#endif /* INC_AI_H_ */
