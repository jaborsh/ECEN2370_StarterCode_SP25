/*
 * gameplay.h
 *
 *  Created on: Apr 23, 2025
 *      Author: jakeb
 */

#ifndef INC_GAMEPLAY_H_
#define INC_GAMEPLAY_H_

#include <stdio.h>
#include "ai.h"
#include "final.h"
#include "LCD_Driver.h"

#define BOARD_ROWS 6
#define BOARD_COLS 7
#define BOARD_COLOR LCD_COLOR_BLUE
#define EMPTY_COLOR LCD_COLOR_WHITE
#define PLAYER_NONE 0
#define PLAYER_RED 1
#define PLAYER_YELLOW 2
#define WIN_COUNT 4
#define GAME_MODE_ONE_PLAYER 1
#define GAME_MODE_TWO_PLAYER 2
#define NEW_GAME 1
#define RESTART_GAME 2
#define SAVE_STATS 1
#define NO_SAVE_STATS 0
#define GAME_WON 1
#define GAME_CONTINUE 0

extern uint8_t game_board[BOARD_ROWS][BOARD_COLS];
extern uint8_t current_player;
extern uint8_t current_column;
extern uint8_t game_state;
extern uint8_t game_mode;
extern uint32_t red_score;
extern uint32_t yellow_score;
extern uint32_t game_time_seconds;
extern uint32_t game_start_time;

// Initializes a new game.
void init_game(uint8_t mode);
// Draws the game board and all pieces on the display.
void draw_board(void);
// Draws the indicator at the top of the screen displaying actively selected column.
void draw_player_indicator(uint8_t column, uint16_t color);
// Processes touch to select or place a piece in a column
uint8_t process_touch_for_column(uint16_t touch_x);
// Drops a piece in the specified column.
int8_t drop_piece(uint8_t column, uint8_t player);
// Draws the dropped piece in the specific board position it was dropped.
void draw_piece(uint8_t row, uint8_t column, uint8_t player);
// Gets the associated color with the current player.
uint16_t get_player_color(uint8_t player);
// Changes to the next player.
void switch_player(void);
// Check if the last move ended the game.
uint8_t check_win(uint8_t row, uint8_t col, uint8_t player);
// Check if the game ends in a draw.
uint8_t check_draw(void);
// Update game duration.
void update_game_time(void);
// Restart the last round of gameplay (no stat saving).
void reset_game(uint8_t save_stats);
// Handle AI turn.
void process_ai_turn(void);
// Move piece to new location based on screen touch.
uint8_t process_touch_for_column_select(uint16_t touch_x);
// Handle the piece placement in the current column.
uint8_t place_piece_in_current_column(void);

#endif /* INC_GAMEPLAY_H_ */
