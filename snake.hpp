#ifndef SNAKE_HPP
#define SNAKE_HPP
#include "utils.h"
#include <exception>

using namespace std;

class SnakeSegment {
public:
  int x, y;
  Direction dir;
  SnakeSegment *next;
  SnakeSegment(int _x, int _y, Direction _d);
};
class Snake {
public:
  SnakeSegment *front,
      *back; // Note: reversed queue (front is back, freedom is slavery)
  bool growing;
  int w, h;
  Direction current_dir;
  Snake(int width, int height);
  ~Snake();
  void move();
  void move(Direction move_dir);
  void grow();
};

#endif // SNAKE_HPP
