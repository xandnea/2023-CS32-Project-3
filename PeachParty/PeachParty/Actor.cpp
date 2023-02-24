#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATION
Actor::Actor(StudentWorld* studentWorld, int imageID, int startX, int startY, int startDir, int depth) 
	: GraphObject( imageID, startX, startY, startDir, depth, 1 )
{
	m_studentWorld = studentWorld;
	m_active = true;
}

Actor::~Actor() { } // nothing dynamically allocated

// PLAYER AVATAR IMPLEMENTATION
PlayerAvatar::PlayerAvatar(StudentWorld* studentWorld, int player, int imageID, int startX, int startY)
	: Actor(studentWorld, imageID, startX, startY, 0, 1 )
{
	m_player = player;
	m_numCoins = 0;
	m_numStars = 0;
}

void PlayerAvatar::swapCoins(PlayerAvatar& player) 
{
	int temp = player.getNumCoins();
	player.setNumCoins(this->getNumCoins());
	this->setNumCoins(temp);
}

void PlayerAvatar::swapPosAndState(PlayerAvatar& player)
{
	int tempX, tempY, tempState;

	tempX = player.getX();
	tempY = player.getY();
	tempState = player.getState();

	player.moveTo(this->getX(), this->getY());
	player.setState(this->getState());

	this->moveTo(tempX, tempY);
	this->setState(tempState);
}

void PlayerAvatar::teleportPlayer(int x, int y)
{
	this->moveTo(x, y);
}

void PlayerAvatar::doSomething()
{
	int die_roll;

	if (getState() == WAITING_TO_ROLL) {
		// if the avatar has an invalid direction (due to being teleported):

		int action = getWorld()->getAction(getPlayerNum());

		if (action == ACTION_ROLL) {
			die_roll = randInt(1, 10);
			setTicks(die_roll * 8);
			setState(WALKING);
		}
		// else if (action == ACTION_FIRE) {
		//
		// }
		else { return; } // user didn't press a key or pressed another key
	}
	if (getState() == WALKING) {
		// if the avatar is directly on top of a directional square
		// else if the avatar is directly on top of a square at a fork 
		// if ()
	}
}

PlayerAvatar::~PlayerAvatar() { } // idk what to delete

// SQUARE IMPLEMENTATION
Square::Square(StudentWorld* studentWorld, int imageID, int startX, int startY, int startDir)
	: Actor(studentWorld, imageID, startX, startY, startDir, 1)
{

}

Square::~Square()
{

}