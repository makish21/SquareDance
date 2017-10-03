#pragma once
#include "Controllable.hpp"

class Command
{
public:
	virtual void execute(Controllable&) = 0;
};

class LeftCommand : public Command
{
public:
	virtual void execute(Controllable& controllable)
	{
		controllable.left();
	}
};

class RightCommand : public Command
{
public:
	virtual void execute(Controllable& controllable)
	{
		controllable.right();
	}
};