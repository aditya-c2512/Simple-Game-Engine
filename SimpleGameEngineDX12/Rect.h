#pragma once

class Rect
{
public:
	Rect() :width(0), height(0), left(0), top(0)
	{
	}
	Rect(int width, int height) :left(0), top(0), width(width), height(height)
	{
	}
	Rect(int left, int top, int width, int height) :left(left), top(top), width(width), height(height)
	{
	}
	Rect(const Rect& rect) :left(rect.left), top(rect.top), width(rect.width), height(rect.height)
	{
	}
	~Rect()
	{
	}
public:
	int width, height, left, top;
};