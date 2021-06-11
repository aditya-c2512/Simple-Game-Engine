#pragma once

class Point
{
public :
	Point() : x(0), y(0) {}
	Point(int px, int py) : x(px), y(py) {}
	Point(const Point& p) : x(p.x), y(p.y) {}
	~Point() {}

public :
	int x = 0; int y = 0;
};