#include "includes/common.h"

int   random_int(int lower, int upper) {
  int random = (rand() % (upper - lower + 1)) + lower;
  return random;
}
