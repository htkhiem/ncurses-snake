#ifndef SNAKE_UTILS
#define SNAKE_UTILS
#include <chrono>
#include <cmath>
#include <iostream>
#include <queue>
#include <thread>

enum class Direction {
  left,
  up,
  down,
  right,
  leftup,
  leftdown,
  upleft,
  upright,
  downleft,
  downright,
  rightup,
  rightdown
};

struct Position {
  int x, y;
  Position(int _x, int _y) : x(_x), y(_y) {}
  bool operator==(Position other) { return (x == other.x && y == other.y); }
};

#endif // SNAKE_UTILS
