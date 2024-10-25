#ifndef MACROS_H
#define MACROS_H


/* Coordinates for verious things */
#define MIN_X 1
#define MIN_Y 7
#define MAX_X 100
#define MAX_Y 40
#define STATUS_LINE_X 0
#define STATUS_LINE_Y MIN_Y - 4

/* Character Values */
#define TOP_LEFT_CORNER -38
#define TOP_RIGHT_CORNER -65
#define BOTTOM_LEFT_CORNER -64
#define BOTTOM_RIGHT_CORNER -39
#define VERTICLE -77
#define HORIZONTAL -60
#define BODY_SEGMENT 4
#define HEAD 2
#define FOOD 3
#define ARROW_KEY_INDICATOR -32
#define RIGHT_ARROW_SYMBOL 16
#define LEFT_ARROW_SYMBOL 17
#define UP_ARROW_SYMBOL 30
#define DOWN_ARROW_SYMBOL 31

/* colors for snake and fruit */
#define PLAYER_COLOR 2
#define OPPONENT_COLOR 4
#define FOOD_COLOR 6
#define DEFAULT_COLOR 15

/* Colision Status */
#define PLAYER_COLIDE 1
#define OPPONENT_COLIDE 2

#define GAME_OVER 3

#endif
