#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
	Actor(int X, int Y, int startDir); // need to set GraphObject IID, x, and y 

	inline
		bool isActive() { return m_active; } // check if object is active

	virtual void doSomething() = 0; // prure virtual 

	virtual ~Actor();

private:
	bool m_active;

};

#endif // ACTOR_H_
