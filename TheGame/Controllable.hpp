#pragma once

class Controllable
{
public:
	virtual ~Controllable()
	{
	};

	virtual void left() = 0;
	virtual void right() = 0;
};