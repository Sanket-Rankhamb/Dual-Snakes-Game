#ifndef MACROS_H
#define MACROS_H

/* Coordinates for verious things */
#define MIN_X 1
#define MIN_Y 7
#define MAX_X 50
#define MAX_Y 35
#define STATUS_LINE_X 0
#define STATUS_LINE_Y MIN_Y - 3

/* colors for snake and fruit */
#define PLAYER_SNAKE_COLOR 1
#define OPPONENT_SNAKE_COLOR 4
#define FOOD_COLOR 5
#define DEFAULT_COLOR 15

/* Border Drawing Characters */
#define TOP_LEFT_CORNER -38
#define TOP_RIGHT_CORNER -65
#define BOTTOM_LEFT_CORNER -64
#define BOTTOM_RIGHT_CORNER -39
#define VERTICLE -77
#define HORIZONTAL -60

/* snake and Food Drawing Characters */
#define BODY_SEGMENT 4
#define HEAD 2
#define FOOD 3

/* Colision Status */
#define PLAYER_COLIDE 1
#define OPPONENT_COLIDE 2

#define GAME_OVER 3

#endif
