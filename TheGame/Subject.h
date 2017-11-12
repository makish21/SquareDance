#pragma once
#include <deque>
#include <algorithm>

#include "Observer.hpp"

class Subject
{
public:
	Subject();
	~Subject();

	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);

protected:
	void notify(const GameEvent& event);

private:
	std::deque<Observer*> m_observers;
};

