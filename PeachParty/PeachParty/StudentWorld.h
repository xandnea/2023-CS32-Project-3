#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h" // required?
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// helpers
	void getPlayerStartingPos(int &x, int &y);

private:
	Board m_board;
	std::string m_boardFile;
	std::vector<Actor*> Actors;
	Actor* Peach;
	Actor* Yoshi;
	int m_bank;
};

#endif // STUDENTWORLD_H_
