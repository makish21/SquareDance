#include "Subject.h"


Subject::Subject()
{
}

Subject::~Subject()
{
	m_observers.clear();
}

void Subject::addObserver(Observer * observer)
{
	m_observers.push_back(observer);
}

void Subject::removeObserver(Observer * observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer));
}

void Subject::notify(const GameEvent & event)
{
	for (auto i = m_observers.begin(); i != m_observers.end(); i++)
	{
		(*i)->onNotify(event);
	}
}
