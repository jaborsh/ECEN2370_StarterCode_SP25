/*
 * gameplay.c
 *
 *  Created on: Apr 23, 2025
 *      Author: jakeb
 */

#include "gameplay.h"
#include "final.h"
#include "ai.h"

uint8_t game_board[BOARD_ROWS][BOARD_COLS] = {0}; // Required for empty space initialization
uint8_t current_player = PLAYER_RED;
uint8_t current_column = BOARD_COLS / 2;
uint8_t game_state = GAME_ACTIVE;
uint8_t game_mode = GAME_MODE_TWO_PLAYER;

uint32_t red_score = 0;
uint32_t yellow_score = 0;
uint32_t game_time_seconds = 0;
uint32_t game_start_time = 0;

void init_game(uint8_t mode) {
    for (uint8_t row = 0; row < BOARD_ROWS; row++) {
        for (uint8_t col = 0; col < BOARD_COLS; col++) {
            game_board[row][col] = PLAYER_NONE;
        }
    }
    
    current_player = PLAYER_RED;
    current_column = BOARD_COLS / 2;
    game_state = GAME_ACTIVE;
    game_mode = mode;
    
    // Initialize AI if in one-player mode
    if (game_mode == GAME_MODE_ONE_PLAYER) {
        init_ai_random();
    }
    
    game_start_time = HAL_GetTick();
    
    draw_board();
    draw_player_indicator(current_column, get_player_color(current_player));
}

void draw_board(void) {
    LCD_Clear(0, LCD_COLOR_WHITE);
    
    uint16_t board_width = LCD_PIXEL_WIDTH;
    uint16_t board_height = LCD_PIXEL_HEIGHT;
    uint16_t cell_width = board_width / BOARD_COLS;
    uint16_t cell_height = board_height / BOARD_ROWS;
    uint16_t cell_size = (cell_width < cell_height) ? cell_width : cell_height;
    uint16_t board_offset_x = (LCD_PIXEL_WIDTH - (BOARD_COLS * cell_size)) / 2;
    uint16_t board_offset_y = (LCD_PIXEL_HEIGHT - (BOARD_ROWS * cell_size)) / 2;
    
    LCD_Draw_FillRectangle(board_offset_x, board_offset_y, 
                          BOARD_COLS * cell_size, BOARD_ROWS * cell_size, BOARD_COLOR);
    
    for (uint8_t row = 0; row < BOARD_ROWS; row++) {
        for (uint8_t col = 0; col < BOARD_COLS; col++) {
            uint16_t x = board_offset_x + (col * cell_size) + (cell_size / 2);
            uint16_t y = board_offset_y + (row * cell_size) + (cell_size / 2);
            uint16_t radius = (cell_size / 2) - 3;
            
            if (game_board[row][col] == PLAYER_NONE) {
                LCD_Draw_Circle_Fill(x, y, radius, EMPTY_COLOR);
            } else {
                uint16_t color = get_player_color(game_board[row][col]);
                LCD_Draw_Circle_Fill(x, y, radius, color);
            }
        }
    }
}

uint16_t get_player_color(uint8_t player) {
    if (player == PLAYER_RED) {
        return LCD_COLOR_RED;
    } else if (player == PLAYER_YELLOW) {
        return LCD_COLOR_YELLOW;
    }
    return EMPTY_COLOR;
}

void draw_player_indicator(uint8_t column, uint16_t color) {
    uint16_t board_width = LCD_PIXEL_WIDTH;
    uint16_t cell_size = board_width / BOARD_COLS;
    uint16_t board_offset_x = (LCD_PIXEL_WIDTH - (BOARD_COLS * cell_size)) / 2;
    uint16_t indicator_x = board_offset_x + (column * cell_size) + (cell_size / 2);
    uint16_t indicator_y = 15;
    uint16_t indicator_radius = cell_size / 3;
    
    LCD_Draw_FillRectangle(0, 0, LCD_PIXEL_WIDTH, indicator_y + indicator_radius + 5, LCD_COLOR_WHITE);
    LCD_Draw_Circle_Fill(indicator_x, indicator_y, indicator_radius, color);
}

void draw_piece(uint8_t row, uint8_t col, uint8_t player) {
    uint16_t cell_size = LCD_PIXEL_WIDTH / BOARD_COLS;
    uint16_t board_offset_x = (LCD_PIXEL_WIDTH - (BOARD_COLS * cell_size)) / 2;
    uint16_t board_offset_y = (LCD_PIXEL_HEIGHT - (BOARD_ROWS * cell_size)) / 2;
    uint16_t x = board_offset_x + (col * cell_size) + (cell_size / 2);
    uint16_t y = board_offset_y + (row * cell_size) + (cell_size / 2);
    uint16_t radius = (cell_size / 2) - 3;
    
    LCD_Draw_Circle_Fill(x, y, radius, get_player_color(player));
}

int8_t drop_piece(uint8_t column, uint8_t player) {
    if (game_board[0][column] != PLAYER_NONE) {
        return -1;
    }
    
    int8_t row;
    for (row = BOARD_ROWS - 1; row >= 0; row--) {
        if (game_board[row][column] == PLAYER_NONE) {
            break;
        }
    }
    
    game_board[row][column] = player;
    draw_piece(row, column, player);
    
    return row; 
}

void switch_player(void) {
    if (current_player == PLAYER_RED) {
        current_player = PLAYER_YELLOW;
    } else {
        current_player = PLAYER_RED;
    }
    
    draw_player_indicator(current_column, get_player_color(current_player));
}

uint8_t process_touch_for_column_select(uint16_t touch_x) {
    uint16_t cell_size = LCD_PIXEL_WIDTH / BOARD_COLS;
    uint16_t board_offset_x = (LCD_PIXEL_WIDTH - (BOARD_COLS * cell_size)) / 2;
    
    uint8_t selected_column;
    if (touch_x < board_offset_x) {
        selected_column = 0;
    } 
    else if (touch_x >= (board_offset_x + BOARD_COLS * cell_size)) {
        selected_column = BOARD_COLS - 1;
    }
    else {
        selected_column = (touch_x - board_offset_x) / cell_size;
    }
    
    if (selected_column != current_column) {
        current_column = selected_column;
        draw_player_indicator(current_column, get_player_color(current_player));
        return 1;
    }
    
    return 0;
}

uint8_t place_piece_in_current_column(void) {
    if (game_state != GAME_ACTIVE) {
        return 0;
    }
    
    if (game_mode == GAME_MODE_ONE_PLAYER && current_player != PLAYER_RED) {
        return 0;
    }
    
    int8_t row = drop_piece(current_column, current_player);
    if (row >= 0) {
        if (check_win(row, current_column, current_player)) {
            game_state = GAME_WIN;
            update_game_time();
            
            if (current_player == PLAYER_RED) {
                red_score++;
            } else {
                yellow_score++;
            }
            return 1;
        }
        
        if (check_draw()) {
            game_state = GAME_DRAW;
            update_game_time();
            return 1;
        }
        
        switch_player();
        
        if (game_mode == GAME_MODE_ONE_PLAYER && current_player == PLAYER_YELLOW) {
            HAL_Delay(500);
            process_ai_turn();
        }
        
        return 1;
    }
    
    return 0;
}

uint8_t process_touch_for_column(uint16_t touch_x) {
    uint16_t cell_size = LCD_PIXEL_WIDTH / BOARD_COLS;
    uint16_t board_offset_x = (LCD_PIXEL_WIDTH - (BOARD_COLS * cell_size)) / 2;
    
    uint8_t selected_column;
    if (touch_x < board_offset_x) {
        selected_column = 0;
    } else if (touch_x >= (board_offset_x + BOARD_COLS * cell_size)) {
        selected_column = BOARD_COLS - 1;
    } else {
        selected_column = (touch_x - board_offset_x) / cell_size;
    }
    
    uint8_t action_taken = 0;
    if (game_mode == GAME_MODE_ONE_PLAYER && current_player != PLAYER_RED) {
        return action_taken;
    }
    
    if (selected_column == current_column) {
        int8_t row = drop_piece(current_column, current_player);
        
        if (row >= 0) {
            action_taken = 1;
            if (check_win(row, current_column, current_player)) {
                game_state = GAME_WIN;
                update_game_time();
                
                if (current_player == PLAYER_RED) {
                    red_score++;
                } else {
                    yellow_score++;
                }
                return action_taken;
            }
            
            if (check_draw()) {
                game_state = GAME_DRAW;
                update_game_time();
                return action_taken;
            }
            
            switch_player();
            if (game_mode == GAME_MODE_ONE_PLAYER && current_player == PLAYER_YELLOW) {
                HAL_Delay(500);
                process_ai_turn();
            }
        }
    } else {
        current_column = selected_column;
        draw_player_indicator(current_column, get_player_color(current_player));
    }
    
    return action_taken;
}

// Handle AI's turn
void process_ai_turn(void) {
    if (game_state != GAME_ACTIVE || current_player != PLAYER_YELLOW) {
        return;
    }
    
    uint8_t ai_column = ai_get_move(game_board);
    current_column = ai_column;
    draw_player_indicator(current_column, get_player_color(current_player));
    HAL_Delay(300);
    int8_t row = drop_piece(current_column, current_player);
    
    if (row >= 0) {
        if (check_win(row, current_column, current_player)) {
            game_state = GAME_WIN;
            update_game_time();
            
            if (current_player == PLAYER_RED) {
                red_score++;
            } else {
                yellow_score++;
            }
            return;
        }
        
        if (check_draw()) {
            game_state = GAME_DRAW;
            update_game_time();
            return;
        }
        
        switch_player();
    }
}

uint8_t check_win(uint8_t row, uint8_t col, uint8_t player) {
    int8_t directions[4][2] = {
        {0, 1},  // horizontal
        {1, 0},  // vertical
        {1, 1},  // diagonal up-right
        {1, -1}  // diagonal up-left
    };
    
    for (uint8_t dir = 0; dir < 4; dir++) {
        uint8_t count = 1;
        
        // Bidirectional check
        for (int8_t sign = -1; sign <= 1; sign += 2) {
            for (uint8_t i = 1; i < WIN_COUNT; i++) {
                int8_t r = row + i * sign * directions[dir][0];
                int8_t c = col + i * sign * directions[dir][1];
                
                if (r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS && 
                    game_board[r][c] == player) {
                    count++;
                } else {
                    break;
                }
            }
        }
        
        if (count >= WIN_COUNT) {
            return GAME_WON;
        }
    }
    
    return GAME_CONTINUE;
}

uint8_t check_draw(void) {
    for (uint8_t col = 0; col < BOARD_COLS; col++) {
        if (game_board[0][col] == PLAYER_NONE) {
            return GAME_CONTINUE;
        }
    }
    return GAME_WON;
}

void update_game_time(void) {
    game_time_seconds = (HAL_GetTick() - game_start_time) / 1000; // Convert to seconds.
}

void reset_game(uint8_t save_stats) {
    if (!save_stats) {
        game_time_seconds = 0;
        if (game_state == GAME_WIN) {
            if (current_player == PLAYER_RED) {
                red_score--;
            } else {
                yellow_score--;
            }
        }
    }
    
    init_game(game_mode);
}