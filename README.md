# STM32 Board Connect 4 Game (ECEN 2370)

## Overview

This project is a Connect 4 game to be played on the STM32 Discovery Series board with LCD display
with ILI9341 controller and STMPE811 Touch Controller.

There are two modes:

-   Single-player: Player vs. AI opponent
-   Two-player: Play against a friend (or enemy!)

Players will select columns by touching the screens and secure drop location by pressing the user
button. Wins and tracked for both red and yellow players. The duration of each game session is
recorded. Post-game a choice to start a new game or restart is presented along with session
statistics and round duration.

## Project Scope and Timeline

### Scope

-   Game logic implementation for Connect 4 rules
-   Win detection
-   UI for LCD touchscreen
-   Input handling and physical button interaction
-   AI opponent for single-player mode
-   Statistic Tracking

### Timeline

#### Drawing & Configuration (Days 1-3)

-   Create Connect 4 Board
-   Create methodology for red and yellow players
-   Implement board configuration for players to occupy tiles

#### Game Logic (Days 4-7)

-   Create playable logic
-   Create functional piece placement
-   Create turn functionality
-   Create win conditions
-   Create draw conditions

#### UI (Days 8-11)

-   Develop menu system for one-player and two-player modes
-   Implement game statistic tracking
-   Implement new game vs. restart button

#### Optimizations (Days 12-14)

-   Transition from selecting column with duplicative touch to button control
-   Optimize button locations with admittedly janky logic to determine where to press on screen

#### AI (Day 14)

-   Create randomly generated AI strategy

## Testing

The testing strategy for the Connect 4 game was purely manual testing. Manual tests were used to
cover core game mechanics to include the main menu navigation, the board display, single-player
mode, two-player mode, and the final screen.

These manual tests verified the touch screen use as well as user button interaction. Some
"vibe-only" tests were conducted to ensure extended playtimes did not break the gameplay, such as if
a player went "AFK" for an extended amount of time, or interaction buttons were spammed excessively.

## Documentation

The project somewhat follows Object-Oriented fundamentals (to the degree that files relate to their
core components). These include:

-   `ai` for AI implementation
-   `ApplicationCode` for overall game driving
-   `button_driver` for user button interaction
-   `final` for logic related to the final screen
-   `fonts` for text rendering
-   `gameplay` for the core gameplay loop
-   `ili9341` for ili9341 integration
-   `LCD_Driver` for LCD integration and rendering
-   `main` for the application to run
-   `stm32fxx*` for integration with the hardware
-   `stmpe811` for integration with the STMPE811 controller.

### Key Functions:

-   `ApplicationInit()` - Initializes all hardware and software components required for the Connect4
    game
-   `Connect4()` - Initializes the core game loop that handles menus, gameplay, and coordination
    between functions.
-   `draw_board()` - Renders the game board
-   `drop_piece(uint8_t column, uint8_t player)` - Places a game piece
-   `check_win(uint8_t row, uint8_t col, uint8_t player)` - Checks if victory has been achieved
-   `check_draw()` - Checks if victory has been capitulated by both players
-   `process_touch_for_column_select(uint16_t touch_x)` - Processes x-coordinate touch for column
    selection
-   `show_final_screen(uint8_t game_state, uint8_t current_player, uint32_t red_score, uint32_t yellow_score, uint32_t game_time_seconds)` -
    Displays the result screen
-   `process_final_screen_touch(uint16_t x, uint16_t y)` - Determines new game vs restart
-   `ai_get_move(uint8_t board[BOARD_ROWS][BOARD_COLS])` - Determines AI's next (random) move

### More Documentation

The Connect4 board is represented by a 2D array with six vertical slots and seven horizontal slots.
Three players (yes, three) are determined by macros: none (empty spaces), red, and yellow.

The application code initiaties the game, then an indefinite (until victory/draw) loop continually
checks touch/button interaction to determine piece placement.

## Struggles / Obstacles

One of the most significant challenges faced was learning how to use the touchscreen. While one
excuse is that it's a cheap screen, a more reasonable explanation is realizing that the x/y
coordinates are passed by pointers and updated with controller functions. Similarly, before
arbitrary delays were introduced into the gameplay loop, a screen touch or a button press would
register multiple times rapidly causing multiple pieces to be placed simultaneously, column behavior
to be buggy, and the game loop changing too quickly to be reasonably played.

As mentioned, arbitrary delays were used to resolve this problem. Falling and rising edges were also
used for the user button to ensure that a release of the button had to be registered before future
interaction would be permitted.

A second difficulty was designing the game board. The strategy for drawing the board was to begin
with simply a purely blue rectangle, then overlay the rectangle with a series of white circles.

With these white circles placed, the game could begin, and then the same positions could be used by
the gameplay loop with each subsequent player turn.

## Learning

Working with an STM32 Discovery Board is challenging. As a computer science major, hardware
integration is certainly not in my tool kit; at least, it wasn't before this class. Software (such
as the Connect4 game itself) felt like second nature. Interacting with the hardware in conjunction
with the programming took quite the effort.

I've grown to appreciate abstraction. With limited familiarity with the STM32 HAL, the more I used
it the more I wanted to continue using it rather than approach things that had already been resolved
with my own (likely worse) solutions.

## If I were to restart...

If I were to restart, I'd be more conscious about OOP as will be discussed in the improvements
section. I'd also spend the entire five seconds necessary to understand the X/Y layout of the LCD
screen instead of randomly changing greater-than and less-than symbols every time something wouldn't
work the way I wanted. While it was unattempted credit, I think addressing the mathematical
algorithm for a smarter AI would be an enjoyable challenge and would take precedence over the purely
random mechanics. I'd also experiment with the gyro to see if I could get the extra-credit tilt
mechanics to work properly.

Many of my ideas surrounding a restart overlap with suggested improvements. So, without further ado:

## Improvements

The project lacks OOP design. Code relies heavily on global variables. While mostly isolated in
logical implementation, `gameplay`, `ai`, and `final` all rely upon one another. Maintenance at this
point, if the project were to be continued, would be difficult and require a significant amount of
refactoring.

With enough time, this project could be vastly improved by using structs to define things like the
board, players, handlers to manage hardware integration, the AI, and the game state. In doing so, if
nothing else, much of the global variable reliance would be removed.

If a more OOP-centric approach was taken, the maintainability of the project would improve vastly.
Likewise, it would make it easier to detect bugs and resolve issues. It would also make it easier to
expand functionality from the current position; multiple levels of AI difficulty could be
introduced, screen rendering could be made prettier, and plenty more.
