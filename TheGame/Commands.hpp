#pragma once
#include "Controllable.hpp"

class Command
{
public:
	virtual void execute(Controllable&) = 0;
};

class MoveLeftCommand : public Command
{
public:
	virtual void execute(Controllable& controllable)
	{
		controllable.moveLeft();
	}
};

class MoveRightCommand : public Command
{
public:
	virtual void execute(Controllable& controllable)
	{
		controllable.moveRight();
	}
};

class StopMovingLeftCommand : public Command
{
public:
	virtual void execute(Controllable& controllable)
	{
		controllable.stopMovingLeft();
	}
};

class StopMovingRightCommand : public Command
{
public:
	virtual void execute(Controllable& controllable)
	{
		controllable.stopMovingRight();
	}
};