#include "snake.hpp"

SnakeSegment::SnakeSegment(int _x, int _y, Direction _d)
    : x(_x), y(_y), dir(_d), next(nullptr) {}

Snake::Snake(int width, int height)
    : current_dir(Direction::right), growing(false), w(width), h(height) {
  back = new SnakeSegment(ceil(w / 2), ceil(h / 2), Direction::right);
  front = new SnakeSegment(ceil(w / 2) + 1, ceil(h / 2), Direction::right);
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
  SnakeSegment *new_segment =
      new SnakeSegment(front->x, front->y, move_dir);

  switch (move_dir) {
  case Direction::left:
    switch (current_dir) {
    case Direction::up:
      front->dir = Direction::upleft;
      break;
    case Direction::down:
      front->dir = Direction::downleft;
      break;
    default:
      front->dir = Direction::left;
    }
    new_segment->x--;
    break;
  case Direction::up:
    switch (current_dir) {
    case Direction::left:
      front->dir = Direction::leftup;
      break;
    case Direction::right:
      front->dir = Direction::rightup;
      break;
    default:
      front->dir = Direction::up;
    }
    new_segment->y--;
    break;
  case Direction::right:
    switch (current_dir) {
    case Direction::up:
      front->dir = Direction::upright;
      break;
    case Direction::down:
      front->dir = Direction::downright;
      break;
    default:
      front->dir = Direction::right;
    }
    new_segment->x++;
    break;
  case Direction::down:
    switch (current_dir) {
    case Direction::left:
      front->dir = Direction::leftdown;
      break;
    case Direction::right:
      front->dir = Direction::rightdown;
      break;
    default:
      front->dir = Direction::down;
    }
    new_segment->y++;
    break;
  }
  current_dir = move_dir;
  
  // Wall collison detection
  if (new_segment->x < 1 || new_segment->x >= w - 1 || new_segment->y < 0 ||
      new_segment->y >= h + 2)
    throw std::string("Your snake hit a wall!");

  // Bite detection
  else {
    SnakeSegment *cursor = back;
    while (cursor) {
      if (cursor->x == new_segment->x && cursor->y == new_segment->y)
        throw std::string("Your snake bit itself!");
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
