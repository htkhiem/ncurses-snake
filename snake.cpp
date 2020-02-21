#include "snake.hpp"

SnakeSegment::SnakeSegment(int _x, int _y): x(_x), y(_y), next(nullptr) {}

Snake::Snake(int width, int height) : 
	current_dir(Direction::right), growing(false), w(width), h(height) {
	front = back = new SnakeSegment(ceil(w/2), ceil(h/2));
}

Snake::~Snake() { // Start from back of snake (front of queue)
	SnakeSegment* temp = front, * temp_next = front->next;
	do {
		delete temp;
		temp = temp_next;
		temp_next = temp_next->next;
	} while (temp_next);
}

void Snake::grow() {
	growing = true;
}

void Snake::move(Direction move_dir) { // TODO: collison detection
	current_dir = move_dir;
	SnakeSegment* new_segment = new SnakeSegment(front->x, front->y);
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
	if (new_segment->x < 0 || new_segment->x >= w ||
		new_segment->y < 0 || new_segment->y >= h)
		throw std::range_error("Hit wall");
	front->next = new_segment;
	front = new_segment;
	if (growing) growing = false;
	else {
		SnakeSegment* new_back = back->next;
		delete back;
		back = new_back;
	}
}
