#ifndef SNAKE_UTILS
#define SNAKE_UTILS
#include <cmath>
#include <cstdio>
#include <iostream>
#include <cstdlib> // simple rand() will do
#include <cstring>
#include <ctime>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <exception>

#define BORDER_COLOUR 1
#define SNAKE_COLOUR 2

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

void DrawBorder(size_t width, size_t height, int score);

#endif // SNAKE_UTILS
