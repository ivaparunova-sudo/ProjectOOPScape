#pragma once
class Point
{
	int x;
	int y;

public:

	Point();
	Point(int x, int y);

	int getX() const;
	int getY() const;

	void setY(int y);
	void setX(int x);

	bool operator==(const Point& other) const;

};
