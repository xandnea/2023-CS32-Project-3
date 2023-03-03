#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include "Board.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

const char PLUS = '+';
const char MINUS = '-';
const char RIGHT = '>';
const char LEFT = '<';
const char UP = '^';
const char DOWN = 'v';

class Actor;
class PlayerAvatar;
class Vortex;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);

	// Getters
	inline
		int getBank() const { return m_bank; }

	// Setters
	inline
		void setBank(int bank) { m_bank = bank; }
	inline
		void addToBank(int add) { m_bank += add; }

	Actor* getPointerToSquareAt(int X, int Y);
	void deleteSquareAt(int X, int Y);
	void createDroppingSquareAt(int X, int Y);
	void fireVortex(int X, int Y, int dir);
	void checkIfVortexHit();

	virtual int init();
	virtual int move();
	virtual void cleanUp();

private:
	std::string m_boardFile;
	std::vector<Actor*> actors;
	Board bd;
	PlayerAvatar* Peach;
	PlayerAvatar* Yoshi;
	Vortex* vortex;
	int m_bank;
};

#endif // STUDENTWORLD_H_
