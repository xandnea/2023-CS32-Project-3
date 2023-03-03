#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Board;

const int WAITING_TO_ROLL = 0;
const int WALKING = 1;
const int PAUSED = 2;

const int PLAYER_ONE = 1;
const int PLAYER_TWO = 2;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* studentWorld, int imageID, int startX, int startY, int startDir, int depth); // need to set GraphObject IID, x, and y 

	// Getters
	inline
		bool isActive() const { return m_active; }
	inline
		StudentWorld* getWorld() { return m_studentWorld; }

	// Setters
	inline
		void setActive(bool active) { m_active = active; }

	// Pure Virtual
	virtual void doSomething() = 0;
	
	inline
		virtual bool isDirectionalSquare() { return false; }
	inline
		virtual void whenImpacted() {}
	inline
		virtual bool isImpactable() { return false; }
	inline
		virtual bool isEnemy() { return false; }

	virtual ~Actor();

private:
	bool m_active;
	StudentWorld* m_studentWorld;

};

class Character : public Actor
{
public:
	// Constructor
	Character(StudentWorld* studentWorld, Board* board, int initialState, int imageID, int startX, int startY, int depth);

	// Getters
	inline
		Board* getBoard() const { return m_board; }
	inline
		int getTicks() const { return m_ticksToMove; }
	inline
		int getState() const { return m_state; }
	inline
		int getWalkDir() const { return m_walkDir; }

	// Setters
	inline
		void setTicks(int ticks) { m_ticksToMove = ticks; }
	inline
		void changeTicks(int ticks) { m_ticksToMove += ticks; }
	inline
		void setState(int state) { m_state = state; }
	inline
		void setWalkDir(int dir) { m_walkDir = dir; }

	// Destructor
	virtual ~Character();

private:
	Board* m_board;
	int m_ticksToMove;
	int m_state;
	int m_walkDir;
};

class PlayerAvatar : public Character {
public:
	// Constructor
	PlayerAvatar(StudentWorld* studentWorld, Board* board, int player, int imageID, int startX, int startY);

	// Getters
	inline
		int getPlayerNum() const { return m_player; }
	inline
		int getNumCoins() const { return m_numCoins; }
	inline
		int getNumStars() const { return m_numStars; }
	inline
		int getDieRoll() const { return m_dieRoll; }
	inline
		bool checkVortex() const { return m_vortex; }

	// Setters
	inline
		void setNumCoins(int coins) { m_numCoins = coins; }
	inline
		void setNumStars(int stars) { m_numStars = stars; }
	inline
		void addCoins(int coins)
	{
		if (m_numCoins + coins < 0)
			m_numCoins = 0;
		else
			m_numCoins += coins;
	}
	inline
		void addStars(int stars)
	{
		if (m_numStars + stars < 0)
			m_numStars = 0;
		else
			m_numStars += stars;
	}
	inline
		void setDieRoll(int roll) { m_dieRoll = roll; }
	inline
		void changeDieRoll(int roll) 
	{ 
		if (m_dieRoll + roll < 0)
			m_dieRoll = 0;
		else
			m_dieRoll += roll;
	}
	inline
		void setVortex(bool vortex) { m_vortex = vortex; }

	std::string hasVortex() const;

	void swapCoins(PlayerAvatar* playerAvatar);

	void swapStars(PlayerAvatar* playerAvatar);

	void swapPosAndState(PlayerAvatar* playerAvatar);

	void teleportPlayer(int x, int y);

	void teleportPlayerRandom();

	void doSomething(); // no longer pure virtual

	// Destructor
	virtual ~PlayerAvatar();

private:
	int m_player;
	int m_numCoins;
	int m_numStars;
	int m_dieRoll;
	bool m_vortex;
};

class Square : public Actor
{
public:
	// Constructor
	Square(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY, int startDir);

	// Getter
	inline
		PlayerAvatar* getPeach() const { return Peach; }
	inline
		PlayerAvatar* getYoshi() const { return Yoshi; }
	inline
		int isPeachOn() const { return m_peachOn; }
	inline
		int isYoshiOn() const { return m_yoshiOn; }
	//Setter
	inline
		void setPeachOn(bool isOn) { m_peachOn = isOn; }
	inline
		void setYoshiOn(bool isOn) { m_yoshiOn = isOn; }

	inline
		virtual bool isImpactable() { return false; }

	bool checkIfLandedOn(Character* ch);

	// Destructor
	virtual ~Square();

private:
	PlayerAvatar* Peach;
	PlayerAvatar* Yoshi;
	bool m_peachOn;
	bool m_yoshiOn;
};

class CoinSquare : public Square
{
public:
	// Constructor
	CoinSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, char sign, int imageID, int startX, int startY);

	// Getter
	inline
		char getSign() { return m_sign; }

	void doSomething(); // no longer pure virtual

	// Destructor
	virtual ~CoinSquare();

private:
	char m_sign;
};

class StarSquare : public Square
{
public:
	// Constructor
	StarSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY);

	//// Getters
	//inline
	//	int getPTicksOn() const { return m_pTicksOn; }
	//inline
	//	int getYTicksOn() const { return m_yTicksOn; }

	//// Setters
	//inline
	//	void setPTicksOn(int ticks) { m_pTicksOn = ticks; }
	//inline
	//	void setYTicksOn(int ticks) { m_yTicksOn = ticks; }

	void doSomething(); // no longer pure virtual

	// Destructor
	virtual ~StarSquare();

private:
	//int m_pTicksOn;
	//int m_yTicksOn;
};

class DirectionalSquare : public Square
{
public:
	// Constructor
	DirectionalSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, char dir, int imageID, int startX, int startY);

	// Getters
	inline
		char getDir() const { return m_dir; }

	// Setters
	inline
		void setDir(char dir) { m_dir = dir; }

	inline
		virtual bool isDirectionalSquare() { return true; }

	void doSomething(); // no longer pure virtual

	// Destructor
	virtual ~DirectionalSquare();

private:
	char m_dir;
};

class BankSquare : public Square
{
public:
	// Constructor
	BankSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY);

	void doSomething(); // no longer pure virtual

	// Destructor
	virtual ~BankSquare();

private:

};

class EventSquare : public Square
{
public:
	// Constructor
	EventSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY);

	void doSomething(); // no longer pure virtual

	// Destructor
	virtual ~EventSquare();

private:

};

class DroppingSquare : public Square
{
public:
	// Constructor
	DroppingSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY);

	void doSomething(); // no longer pure virtual

	// Destructor
	virtual ~DroppingSquare();

private:

};

class Enemy : public Character
{
public:
	// Constructor
	Enemy(StudentWorld* studentWorld, Board* board, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY);

	// Getters
	inline
		PlayerAvatar* getPeach() const { return Peach; }
	inline
		PlayerAvatar* getYoshi() const { return Yoshi; }
	inline
		int getTravelDist() const { return m_travelDist; }
	inline
		int getPauseCounter() const { return m_pauseCounter; }
	inline
		int isPeachOn() const { return m_peachOn; }
	inline
		int isYoshiOn() const { return m_yoshiOn; }

	//Setter
	inline
		void setPeachOn(bool isOn) { m_peachOn = isOn; }
	inline
		void setYoshiOn(bool isOn) { m_yoshiOn = isOn; }
	inline
		void changeTravelDist(int change) { m_travelDist += change; }
	inline
		void setPauseCounter(int counter) { m_pauseCounter = counter; }
	inline
		void changePauseCounter(int change) { m_pauseCounter += change; }

	inline
		virtual bool isImpactable() { return true; }
	inline
		virtual bool isEnemy() { return true; }

	// Destructor
	virtual ~Enemy();

private:
	PlayerAvatar* Peach;
	PlayerAvatar* Yoshi;
	bool m_peachOn;
	bool m_yoshiOn;
	int m_travelDist;
	int m_pauseCounter;
};

class Bowser : public Enemy
{
public:
	// Constructor
	Bowser(StudentWorld* studentWorld, Board* board, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY);

	void doSomething();

	void whenImpacted();

	// Destructor
	virtual ~Bowser();

private:

};

class Boo : public Enemy
{
public:
	// Constructor
	Boo(StudentWorld* studentWorld, Board* board, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY);

	void doSomething();

	void whenImpacted();

	// Destructor
	virtual ~Boo();

private:
};

class Vortex : public Actor
{
public:
	// Constructor
	Vortex(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY, int dir);

	// Getters
	inline
		PlayerAvatar* getPeach() const { return Peach; }
	inline
		PlayerAvatar* getYoshi() const { return Yoshi; }
	inline
		int getDir() const { return m_dir; }
	
	void doSomething();

	// Destructor
	virtual ~Vortex();

private:
	PlayerAvatar* Peach;
	PlayerAvatar* Yoshi;
	int m_dir;
};


#endif // ACTOR_H_