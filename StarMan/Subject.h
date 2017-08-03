#pragma once

#include <vector>
#include "Observer.h"

class Subject
{
private:
	std::vector<Observer*> m_Observers;
	size_t	m_numObservers;

public:
	Subject();
	~Subject();

	void AddObserver(Observer* ob)
	{
		m_Observers.push_back(ob);
	}

	void DelObserver(Observer* ob)
	{
		// find and delete observer

	}


protected:
	void notify(const CBaseEntity& entity, int message)
	{
		for (size_t i = 0; i < m_Observers.size(); i++)
		{
			m_Observers[i]->onNotify(entity, message);
		}
	}
};

