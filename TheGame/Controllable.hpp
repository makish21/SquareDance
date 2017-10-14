#pragma once

class Controllable
{
public:
	virtual ~Controllable()
	{
	};

	virtual void moveLeft() = 0;
	virtual void moveRight() = 0;
	virtual void stopMovingLeft() = 0;
	virtual void stopMovingRight() = 0;
};