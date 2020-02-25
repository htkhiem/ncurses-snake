#include "snake.hpp"

SnakeSegment::SnakeSegment(int _x, int _y) : x(_x), y(_y), next(nullptr) {}

Snake::Snake(int width, int height)
    : current_dir(Direction::right), growing(false), w(width), h(height) {
  back = new SnakeSegment(ceil(w / 2), ceil(h / 2));
  front = new SnakeSegment(ceil(w / 2) + 1, ceil(h / 2));
  back->next = front;
}

Snake::~Snake() { // Start from back of snake (front of queue)
  SnakeSegment *temp = back, *temp_next = back->next;
  do {
    delete temp;
    temp = temp_next;
    temp_next = temp_next->next;
  } while (temp_next);
}

void Snake::grow() { growing = true; }

void Snake::move(Direction move_dir) {
  current_dir = move_dir;
  SnakeSegment *new_segment = new SnakeSegment(front->x, front->y);
  switch (current_dir) {
  case Direction::left:
    new_segment->x--;
    break;
  case Direction::up:
    new_segment->y--;
    break;
  case Direction::right:
    new_segment->x++;
    break;
  case Direction::down:
    new_segment->y++;
    break;
  }

  // Wall collison detection
  if (new_segment->x < 0 || new_segment->x >= w || new_segment->y < 0 ||
      new_segment->y >= h)
    throw std::range_error("Snake hit wall!");

  // Bite detection
  else {
    SnakeSegment *cursor = back;
    while (cursor) {
      if (cursor->x == new_segment->x && cursor->y == new_segment->y)
        throw range_error("Snake bit itself!");
      cursor = cursor->next;
    }
  }

  front->next = new_segment;
  front = new_segment;
  if (growing)
    growing = false;
  else {
    SnakeSegment *new_back = back->next;
    delete back;
    back = new_back;
  }
}
