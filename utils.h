#ifndef SNAKE_UTILS
#define SNAKE_UTILS
#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <cmath>


enum class Direction {left, up, down, right};

struct Position {
	int x, y;
	Position(int _x, int _y) : x(_x), y(_y) {} 
	bool operator==(Position other) {
		return (x == other.x && y == other.y);
	}
};

#endif // SNAKE_UTILS