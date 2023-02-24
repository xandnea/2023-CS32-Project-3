#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

const int WAITING_TO_ROLL = 0;
const int WALKING = 1;

const int PLAYER_ONE = 1;
const int PLAYER_TWO = 2;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
	Actor(StudentWorld* studentWorld, int imageID, int startX, int startY, int startDir, int depth); // need to set GraphObject IID, x, and y 

	inline
		bool isActive() { return m_active; } // check if object is active
	inline
		void setActive(bool active) { m_active = active; } // setter
	inline
		StudentWorld* getWorld() { return m_studentWorld; } // return pointer to student world

	virtual void doSomething() = 0; // pure virtual 

	virtual ~Actor();

private:
	bool m_active;
	StudentWorld* m_studentWorld;

};

class PlayerAvatar : public Actor {
public:
	// Constructor
	PlayerAvatar(StudentWorld* studentWorld, int player, int imageID, int startX, int startY);

	// Getters
	inline
		int getPlayerNum() { return m_player; }
	inline
		int getNumCoins() { return m_numCoins; }
	inline
		int getNumStars() { return m_numStars; }
	inline
		bool checkVortex() { return m_vortex; }
	inline
		int getTicks() { return m_ticksToMove; }
	inline
		int getState() { return m_state; }
	
	// Setters
	inline
		void setNumCoins(int coins) { m_numCoins += coins; }
	inline
		void setNumStars(int stars) { m_numStars += stars; }
	inline
		void setVortex(bool vortex) { m_vortex = vortex; }
	inline
		void setTicks(int ticks) { m_ticksToMove = ticks; }
	inline
		void changeTicks(int ticks) { m_ticksToMove += ticks; }
	inline
		void setState(int state) { m_state = state; }

	void swapCoins(PlayerAvatar& playerAvatar); // might want to use a pointer to playerAvatar instead
	
	void swapPosAndState(PlayerAvatar& playerAvatar);

	void teleportPlayer(int x, int y);

	void doSomething(); // no longer pure virtual

	// Destructor
	~PlayerAvatar();

private:
	int m_player;
	int m_numCoins;
	int m_numStars;
	bool m_vortex;
	int m_ticksToMove;
	int m_state;
};

class Square : public Actor
{
public:
	// Constructor
	Square(StudentWorld* studentWorld, int imageID, int startX, int startY, int startDir);

	// Destructor
	~Square();

private:

};
#endif // ACTOR_H_
