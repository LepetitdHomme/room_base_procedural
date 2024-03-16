#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <math.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))
#define DEBUG_MSG(message) printf("Debug: %s, File %s, Line %d\n", message, __FILE__, __LINE__)

#define DEBUG 0
#define DEBUG_GRID 1
#define DEBUG_GRAPH 0
#define DEBUG_SCREEN 0
#define DEBUG_COLLISIONS 0
#define DEBUG_LIGHT_MAP 0

/* WINDOW/SCREEN */
#define WINDOW_WIDTH 1200
#define RATIO_WIDTH 16
#define RATIO_HEIGHT 9
#define WINDOW_HEIGHT (WINDOW_WIDTH * RATIO_HEIGHT) / RATIO_WIDTH

/* GRID GENERATION */
#define SCALE 100
#define MIN_COMPLEXITY 1
#define MAX_COMPLEXITY 10

/* ROOM GENERATION */
#define MIN_CORRIDOR_SIZE 3
#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 10
#define DISTANCE_BETWEEN_ROOMS 3
#define BASE_ROOM_NUMBER 10

/* PLAYER */
#define PLAYER_SPEED 5

/* LIGHT */
#define LIGHT_DETAIL 2
#define LIGHT_ANGLE 360
#define RADIAN (M_PI / 180)

#endif
