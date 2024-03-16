#ifndef ENUMS_H
#define ENUMS_H

enum Type {
  EMPTY,// 0
  WALL_UP,// 1
  WALL_DOWN,// 2
  WALL_LEFT,// 3
  WALL_RIGHT,// 4
  CORNER_TOP_LEFT,// 5
  CORNER_TOP_RIGHT,// 6
  CORNER_BOT_LEFT,// 7
  CORNER_BOT_RIGHT,// 8
  FLOOR,// 9
  CORRIDOR,// 10
  DOOR_UP,// 11
  DOOR_UP_OPEN,// 12
  DOOR_DOWN,// 13
  DOOR_DOWN_OPEN, // 14
  DOOR_LEFT,// 15
  DOOR_RIGHT,// 16
};

enum Dir { UP, DOWN, LEFT, RIGHT };
enum Octant { E, NE, N, NW, W, SW, S, SE };

enum Orientation { VERTICAL, HORIZONTAL };

#endif
