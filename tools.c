#include "includes/common.h"

int     clamp(int value, int min, int max) {
  return (value < min) ? min : (value > max) ? max : value;
}

int     random_int(int lower, int upper) {
  int random = (rand() % (upper - lower + 1)) + lower;
  return random;
}

double  distance_between_coords(coord_t center_1, coord_t center_2) {
  return sqrt(pow(center_2.x - center_1.x, 2) + pow(center_2.y - center_1.y, 2));
}
