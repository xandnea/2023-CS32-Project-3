#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Helpers
bool isDirectlyOnSquare(Character* character);
bool posIsSame(Actor* a1, Actor* a2);
//bool isOnDirSquare(Character* ch);
bool canMoveInDir(Character* ch, int dir);
bool isAtFork(Character* ch);
void possibleMoveOptions(Character* ch, int& moveX, int& moveY);
void teleportRandom(Character* ch);

// ACTOR IMPLEMENTATION
Actor::Actor(StudentWorld* studentWorld, int imageID, int startX, int startY, int startDir, int depth) 
	: GraphObject( imageID, startX, startY, startDir, depth, 1 )
{
	m_studentWorld = studentWorld;
	m_active = true;
}

Actor::~Actor() { } // nothing dynamically allocated

// CHARACTER IMPLEMENTATION
Character::Character(StudentWorld* studentWorld, Board* board, int initialState, int imageID, int startX, int startY, int depth)
	: Actor(studentWorld, imageID, startX, startY, 0, depth)
{
	m_board = board;
	m_state = initialState;
	m_ticksToMove = 0;
	m_walkDir = right;
}

Character::~Character()
{

}

// PLAYER AVATAR IMPLEMENTATION
PlayerAvatar::PlayerAvatar(StudentWorld* studentWorld, Board* board, int player, int imageID, int startX, int startY)
	: Character(studentWorld, board, WAITING_TO_ROLL, imageID, startX, startY, 0)
{
	m_player = player;
	m_numCoins = 0;
	m_numStars = 0;
	m_vortex = false;
	m_dieRoll = 0;
}

std::string PlayerAvatar::hasVortex() const
{
	if (checkVortex())
		return " VOR";
	else
		return "";
}

void PlayerAvatar::swapCoins(PlayerAvatar* player) 
{
	int temp = player->getNumCoins();
	player->setNumCoins(this->getNumCoins());
	this->setNumCoins(temp);
}

void PlayerAvatar::swapStars(PlayerAvatar* player)
{
	int temp = player->getNumStars();
	player->setNumStars(this->getNumStars());
	this->setNumStars(temp);
}

void PlayerAvatar::swapPosAndState(PlayerAvatar* player)
{
	int tempX, tempY, tempTicks, tempWalkDir, tempDir, tempState;

	tempX = player->getX();
	tempY = player->getY();
	tempTicks = player->getTicks();
	tempWalkDir = player->getWalkDir();
	tempDir = player->getDirection();
	tempState = player->getState();

	player->moveTo(this->getX(), this->getY());
	player->setTicks(this->getTicks());
	player->setWalkDir(this->getWalkDir());
	player->setDirection(this->getDirection());
	player->setState(this->getState());

	this->moveTo(tempX, tempY);
	this->setTicks(tempTicks);
	this->setWalkDir(tempWalkDir);
	this->setDirection(tempDir);
	this->setState(tempState);
}

void PlayerAvatar::teleportPlayer(int x, int y)
{
	this->moveTo(x, y);
}

void PlayerAvatar::teleportPlayerRandom()
{
	teleportRandom(this);
}

void PlayerAvatar::doSomething()
{
	if (getState() == WAITING_TO_ROLL) {
		// if the avatar has an invalid direction (due to being teleported): [done in randomTeleport()]

		int action = getWorld()->getAction(getPlayerNum());

		if (action == ACTION_ROLL) {
			int roll = randInt(1, 10);
			//int roll = 1;
			setDieRoll(roll);
			setTicks(m_dieRoll * 8);
			setState(WALKING);
		}
		else if (action == ACTION_FIRE) {
			if (checkVortex())
			{
				setVortex(false);
				getWorld()->fireVortex(getX(), getY(), getWalkDir());
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			else
			{
				return;
			}
		}
		else { return; } // user didn't press a key or pressed another key
	}
	if (getState() == WALKING) {

		int dir = getWalkDir();
		int moveX = 0, moveY = 0;
		int graphX = getX() / SPRITE_WIDTH, graphY = getY() / SPRITE_HEIGHT;

		if (!isDirectlyOnSquare(this))
		{
			if (dir == right)
				moveX += 2;
			if (dir == left)
				moveX -= 2;
			if (dir == up)
				moveY += 2;
			if (dir == down)
				moveY -= 2;

			moveTo(getX() + moveX, getY() + moveY);
			changeTicks(-1);
			if (getTicks() == 0)
				setState(WAITING_TO_ROLL);

			return;
		}

		Actor* square = getWorld()->getPointerToSquareAt(getX(), getY());

		if (square->isDirectionalSquare())
		{
			possibleMoveOptions(this, moveX, moveY);

			if (getWalkDir() == left)
				setDirection(left);
			else
				setDirection(right);

			moveTo(getX() + moveX, getY() + moveY);
			changeTicks(-1);
			changeDieRoll(-1);
			if (getTicks() == 0)
				setState(WAITING_TO_ROLL);
		}
		else if (isAtFork(this))
		{
			int action = getWorld()->getAction(getPlayerNum());

			if (action == ACTION_LEFT && getWalkDir() == right)
				return;
			if (action == ACTION_RIGHT && getWalkDir() == left)
				return;
			if (action == ACTION_DOWN && getWalkDir() == up)
				return;
			if (action == ACTION_UP && getWalkDir() == down)
				return;

			switch (action)
			{
			case ACTION_LEFT:
				if (canMoveInDir(this, left))
				{
					setWalkDir(left);
					setDirection(left);
					moveTo(getX() - 2, getY());
					changeTicks(-1);
					changeDieRoll(-1);
					if (getTicks() == 0)
						setState(WAITING_TO_ROLL);
				}
				else
					return;
				break;
			case ACTION_RIGHT:
				if (canMoveInDir(this, right))
				{
					setWalkDir(right);
					setDirection(right);
					moveTo(getX() + 2, getY());
					changeTicks(-1);
					changeDieRoll(-1);
					if (getTicks() == 0)
						setState(WAITING_TO_ROLL);
				}
				else
					return;
				break;
			case ACTION_UP:
				if (canMoveInDir(this, up))
				{
					setWalkDir(up);
					setDirection(right);
					moveTo(getX(), getY() + 2);
					changeTicks(-1);
					changeDieRoll(-1);
					if (getTicks() == 0)
						setState(WAITING_TO_ROLL);
				}
				else
					return;
				break;
			case ACTION_DOWN:
				if (canMoveInDir(this, down))
				{
					setWalkDir(down);
					setDirection(right);
					moveTo(getX(), getY() - 2);
					changeTicks(-1);
					changeDieRoll(-1);
					if (getTicks() == 0)
						setState(WAITING_TO_ROLL);
				}
				else
					return;
				break;
			}
		}
		else 
		{
			possibleMoveOptions(this, moveX, moveY);

			if (getWalkDir() == left)
				setDirection(left);
			else
				setDirection(right);

			moveTo(getX() + moveX, getY() + moveY);
			changeTicks(-1);
			changeDieRoll(-1);
			if (getTicks() == 0)
				setState(WAITING_TO_ROLL);
		}
	}
}

PlayerAvatar::~PlayerAvatar() 
{ 

}

// SQUARE IMPLEMENTATION
Square::Square(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY, int startDir)
	: Actor(studentWorld, imageID, startX, startY, startDir, 1)
{
	Peach = peach;
	Yoshi = yoshi;
	m_peachOn = true;
	m_yoshiOn = true;
}

bool Square::checkIfLandedOn(Character* ch)
{
	if (isDirectlyOnSquare(ch) && posIsSame(ch, this))
		return true;
	else
		return false;
}

Square::~Square()
{

}

// COIN SQUARE IMPLEMENTATION
CoinSquare::CoinSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, char sign, int imageID, int startX, int startY)
	: Square(studentWorld, peach, yoshi, imageID, startX, startY, 0)
{
	m_sign = sign;
}

void CoinSquare::doSomething()
{
	if (!isActive())
		return;

	if (checkIfLandedOn(getPeach()) && (getPeach()->getState() == WAITING_TO_ROLL)) // if peach is on
	{
		if (isPeachOn() == false) // if peachOn is false, set to true and add/subtract coins
		{
			setPeachOn(true);
			if (getSign() == '+')
			{
				getPeach()->addCoins(3);
				getWorld()->playSound(SOUND_GIVE_COIN);
			}
			if (getSign() == '-')
			{
				getPeach()->addCoins(-3);
				getWorld()->playSound(SOUND_TAKE_COIN);
			}
		}
	}
	else
		setPeachOn(false);

	if (checkIfLandedOn(getYoshi()) && (getYoshi()->getState() == WAITING_TO_ROLL))
	{
		if (isYoshiOn() == false)
		{
			setYoshiOn(true);
			if (getSign() == '+')
			{
				getYoshi()->addCoins(3);
				getWorld()->playSound(SOUND_GIVE_COIN);
			}
			if (getSign() == '-')
			{
				getYoshi()->addCoins(-3);
				getWorld()->playSound(SOUND_TAKE_COIN);
			}
		}
	}
	else
		setYoshiOn(false);
}

CoinSquare::~CoinSquare()
{

}

// STAR SQUARE IMPLEMENTATION
StarSquare::StarSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY)
	: Square(studentWorld, peach, yoshi, imageID, startX, startY, 0)
{
	/*m_pTicksOn = 0;
	m_yTicksOn = 0;*/
}

void StarSquare::doSomething() // might need to check for ticks (but i didn't see the need) 
{
	if (checkIfLandedOn(getPeach()))
	{
		if (isPeachOn() == true)
			return;

		setPeachOn(true);
		if (getPeach()->getNumCoins() < 20)
			return;
		else
		{
			getPeach()->addCoins(-20);
			getPeach()->addStars(1);
			getWorld()->playSound(SOUND_GIVE_STAR);
		}

	}
	else
		setPeachOn(false);

	if (checkIfLandedOn(getYoshi()))
	{
		if (isYoshiOn() == true)
			return;

		setYoshiOn(true);
		if (getYoshi()->getNumCoins() < 20)
			return;
		else
		{
			getYoshi()->addCoins(-20);
			getYoshi()->addStars(1);
			getWorld()->playSound(SOUND_GIVE_STAR);
		}
	}
	else
		setYoshiOn(false);
}

StarSquare::~StarSquare()
{

}

// DIRECTIONAL SQUARE
DirectionalSquare::DirectionalSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, char dir, int imageID, int startX, int startY)
	: Square(studentWorld, peach, yoshi, imageID, startX, startY, 0)
{
	m_dir = dir;

	switch (dir) 
	{
	case '^':
		setDirection(up);
		break;
	case 'v':
		setDirection(down);
		break;
	case '<':
		setDirection(left);
		break;
	case '>':
		setDirection(right);
		break;
	}
}

void DirectionalSquare::doSomething() // not detecting dir square in first column
{
	if (checkIfLandedOn(getPeach()))
	{
		if (isPeachOn() == true)
			return;

		setPeachOn(true);

		switch (getDir())
		{
		case '<':
			getPeach()->setWalkDir(left);
			getPeach()->setDirection(left);
			break;
		case '>':
			getPeach()->setWalkDir(right);
			getPeach()->setDirection(right);
			break;
		case '^':
			getPeach()->setWalkDir(up);
			getPeach()->setDirection(right);
			break;
		case 'v':
			getPeach()->setWalkDir(down);
			getPeach()->setDirection(right);
			break;

		}
	}
	else
		setPeachOn(false);

	if (checkIfLandedOn(getYoshi()))
	{
		if (isYoshiOn() == true)
			return;

		setYoshiOn(true);

		switch (getDir())
		{
		case '<':
			getYoshi()->setWalkDir(left);
			getYoshi()->setDirection(left);
			break;
		case '>':
			getYoshi()->setWalkDir(right);
			getYoshi()->setDirection(right);
			break;
		case '^':
			getYoshi()->setWalkDir(up);
			getYoshi()->setDirection(right);
			break;
		case 'v':
			getYoshi()->setWalkDir(down);
			getYoshi()->setDirection(right);
			break;
		}
	}
	else
		setYoshiOn(false);
}

DirectionalSquare::~DirectionalSquare()
{

}

// BANK SQUARE IMPLEMENTATION
BankSquare::BankSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY)
	: Square(studentWorld, peach, yoshi, imageID, startX, startY, 0)
{

}

void BankSquare::doSomething() 
{
	if (checkIfLandedOn(getPeach()))
	{
		if (isPeachOn() == true)
			return;

		setPeachOn(true);

		if (getPeach()->getState() == WALKING)
		{
			int tempCoins = 5;
			if (getPeach()->getNumCoins() < 5)
				tempCoins = getPeach()->getNumCoins();
			getPeach()->addCoins(-5);
			getWorld()->addToBank(tempCoins);
			getWorld()->playSound(SOUND_DEPOSIT_BANK);
		}
		else if (getPeach()->getState() == WAITING_TO_ROLL)
		{
			getPeach()->addCoins(getWorld()->getBank());
			getWorld()->setBank(0);
			getWorld()->playSound(SOUND_WITHDRAW_BANK);
		}
	}
	else
		setPeachOn(false);

	if (checkIfLandedOn(getYoshi()))
	{
		if (isYoshiOn() == true)
			return;

		setYoshiOn(true);

		if (getYoshi()->getState() == WALKING)
		{
			int tempCoins = 5;
			if (getYoshi()->getNumCoins() < 5)
				tempCoins = getYoshi()->getNumCoins();
			getYoshi()->addCoins(-5);
			getWorld()->addToBank(tempCoins);
			getWorld()->playSound(SOUND_DEPOSIT_BANK);
		}
		else if (getYoshi()->getState() == WAITING_TO_ROLL)
		{
			getYoshi()->addCoins(getWorld()->getBank());
			getWorld()->setBank(0);
			getWorld()->playSound(SOUND_WITHDRAW_BANK);
		}
	}
	else
		setYoshiOn(false);
}

BankSquare::~BankSquare()
{

}

// EVENT SQUARE IMPLEMENTATION
EventSquare::EventSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY)
	: Square(studentWorld, peach, yoshi, imageID, startX, startY, 0)
{

}

void EventSquare::doSomething()
{
	if (checkIfLandedOn(getPeach()) && (getPeach()->getState() == WAITING_TO_ROLL))
	{
		if (isPeachOn() == true)
			return;

		setPeachOn(true);

		int option = randInt(1, 3); 

		switch (option)
		{
		case 1:
			getPeach()->teleportPlayerRandom();
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
			getPeach()->swapPosAndState(getYoshi());
			setYoshiOn(true);
			setPeachOn(true);
			getPeach()->setDieRoll(0);
			getYoshi()->setDieRoll(0);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 3:
			getPeach()->setVortex(true);
			getPeach()->setDieRoll(0);
			getWorld()->playSound(SOUND_GIVE_VORTEX);
			break;
		}
	}
	else
		setPeachOn(false);

	if (checkIfLandedOn(getYoshi()) && (getYoshi()->getState() == WAITING_TO_ROLL))
	{
		if (isYoshiOn() == true)
			return;

		setYoshiOn(true);

		int option = randInt(1, 3);

		switch (option)
		{
		case 1:
			getYoshi()->teleportPlayerRandom();
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
			getYoshi()->swapPosAndState(getPeach());
			setPeachOn(true);
			setYoshiOn(true);
			getPeach()->setDieRoll(0);
			getYoshi()->setDieRoll(0);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 3:
			getYoshi()->setVortex(true);
			getYoshi()->setDieRoll(0);
			getWorld()->playSound(SOUND_GIVE_VORTEX);
			break;
		}
	}
	else
		setYoshiOn(false);
}

EventSquare::~EventSquare()
{

}

// DROPPING SQUARE IMPLEMENTATION
DroppingSquare::DroppingSquare(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY)
	: Square(studentWorld, peach, yoshi, imageID, startX, startY, 0)
{

}

void DroppingSquare::doSomething()
{
	if (checkIfLandedOn(getPeach()) && (getPeach()->getState() == WAITING_TO_ROLL))
	{
		if (isPeachOn() == true)
			return;

		setPeachOn(true);

		int option = randInt(1, 2);

		switch (option)
		{
		case 1:
			getPeach()->addCoins(-10);
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			break;
		case 2:
			getPeach()->addStars(-1);
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			break;
		}
	}
	else
		setPeachOn(false);

	if (checkIfLandedOn(getYoshi()) && (getYoshi()->getState() == WAITING_TO_ROLL))
	{
		if (isYoshiOn() == true)
			return;

		setYoshiOn(true);

		int option = randInt(1, 2);

		switch (option)
		{
		case 1:
			getYoshi()->addCoins(-10);
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			break;
		case 2:
			getYoshi()->addStars(-1);
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			break;
		}
	}
	else
		setYoshiOn(false);
}

DroppingSquare::~DroppingSquare()
{

}

// ENEMY IMPLEMENTATION
Enemy::Enemy(StudentWorld* studentWorld, Board* board, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY)
	: Character(studentWorld, board, PAUSED, imageID, startX, startY, 0)
{
	Peach = peach;
	Yoshi = yoshi;
	m_peachOn = false;
	m_yoshiOn = false;
	m_travelDist = 0;
	m_pauseCounter = 180;
}

Enemy::~Enemy()
{

}

// BOWSER IMPLEMENTATION
Bowser::Bowser(StudentWorld* studentWorld, Board* board, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY)
	: Enemy(studentWorld, board, peach, yoshi, imageID, startX, startY)
{

}

void Bowser::doSomething()
{
	if (getState() == PAUSED)
	{
		if (posIsSame(getPeach(), this))
		{
			if (isPeachOn() == false)
			{
				setPeachOn(true);

				int option = randInt(1, 2);

				if (option == 1)
				{
					getPeach()->setNumCoins(0);
					getPeach()->setNumStars(0);
					getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				}
			}
		}
		else
			setPeachOn(false);

		if (posIsSame(getYoshi(), this))
		{
			if (isYoshiOn() == false)
			{
				setYoshiOn(true);

				int option = randInt(1, 2);

				if (option == 1)
				{
					getYoshi()->setNumCoins(0);
					getYoshi()->setNumStars(0);
					getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				}
			}
		}
		else
			setYoshiOn(false);

		changePauseCounter(-1);
		if (getPauseCounter() == 0)
		{
			int squares_to_move = randInt(1, 10);
			setTicks(squares_to_move * 8);

			int dir = randInt(0, 3) * 90;
			while (!canMoveInDir(this, dir))
			{
				dir = randInt(0, 3) * 90;
			}
			switch (dir)
			{
			case left:
				setWalkDir(left);
				setDirection(left);
				break;
			case right:
				setWalkDir(right);
				setDirection(right);
				break;
			case up:
				setWalkDir(up);
				setDirection(right);
				break;
			case down:
				setWalkDir(down);
				setDirection(right);
				break;

			}

			setState(WALKING);
		}
	}
	if (getState() == WALKING) // sometimes bowser gets stuck in the top left corner
	{
		int walkDir = getWalkDir();
		int moveX = 0, moveY = 0;

		if (!isDirectlyOnSquare(this))
		{
			if (walkDir == right)
				moveX += 2;
			if (walkDir == left)
				moveX -= 2;
			if (walkDir == up)
				moveY += 2;
			if (walkDir == down)
				moveY -= 2;

			moveTo(getX() + moveX, getY() + moveY);
			changeTicks(-1);
			if (getTicks() == 0)
			{
				setState(PAUSED);
				setPauseCounter(180);

				int chance = randInt(1, 4);
				if (chance == 1)
				{
					getWorld()->deleteSquareAt(getX(), getY());
					getWorld()->createDroppingSquareAt(getX(), getY());
					getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
					return;
				}
			}
			return;
		}
		if (isDirectlyOnSquare(this) && isAtFork(this))
		{
			int dir = randInt(0, 3) * 90;
			while (!canMoveInDir(this, dir))
			{
				dir = randInt(0, 3) * 90;
			}
			switch (dir)
			{
			case left:
				setWalkDir(left);
				setDirection(left);
				break;
			case right:
				setWalkDir(right);
				setDirection(right);
				break;
			case up:
				setWalkDir(up);
				setDirection(right);
				break;
			case down:
				setWalkDir(down);
				setDirection(right);
				break;

			}
		}
		
		possibleMoveOptions(this, moveX, moveY);

		if (getWalkDir() == left)
			setDirection(left);
		else
			setDirection(right);

		moveTo(getX() + moveX, getY() + moveY);
		changeTicks(-1);
	}
}

void Bowser::whenImpacted()
{
	teleportRandom(this);
	setState(PAUSED);
	setPauseCounter(180);
}

Bowser::~Bowser()
{

}

// BOO IMPLEMENTATION
Boo::Boo(StudentWorld* studentWorld, Board* board, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY)
	: Enemy(studentWorld, board, peach, yoshi, imageID, startX, startY)
{
	
}

void Boo::doSomething()
{
	if (getState() == PAUSED)
	{
		if (posIsSame(getPeach(), this))
		{
			if (isPeachOn() == false)
			{
				setPeachOn(true);

				int option = randInt(1, 2);

				if (option == 1)
					getPeach()->swapCoins(getYoshi());
				else if (option == 2)
					getPeach()->swapStars(getYoshi());
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
		}
		else
			setPeachOn(false);

		if (posIsSame(getYoshi(), this))
		{
			if (isYoshiOn() == false)
			{
				setYoshiOn(true);

				int option = randInt(1, 2);

				if (option == 1)
					getYoshi()->swapCoins(getPeach());
				else if (option == 2)
					getYoshi()->swapStars(getPeach());
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
		}
		else
			setYoshiOn(false);

		changePauseCounter(-1);
		if (getPauseCounter() == 0)
		{
			int squares_to_move = randInt(1, 3);
			setTicks(squares_to_move * 8);

			int dir = randInt(0, 3) * 90;
			while (!canMoveInDir(this, dir))
			{
				dir = randInt(0, 3) * 90;
			}
			switch (dir)
			{
			case left:
				setWalkDir(left);
				setDirection(left);
				break;
			case right:
				setWalkDir(right);
				setDirection(right);
				break;
			case up:
				setWalkDir(up);
				setDirection(right);
				break;
			case down:
				setWalkDir(down);
				setDirection(right);
				break;

			}

			setState(WALKING);
		}
	}
	if (getState() == WALKING) // sometimes boo gets stuck in the top left corner
	{
		int walkDir = getWalkDir();
		int moveX = 0, moveY = 0;

		if (!isDirectlyOnSquare(this))
		{
			if (walkDir == right)
				moveX += 2;
			if (walkDir == left)
				moveX -= 2;
			if (walkDir == up)
				moveY += 2;
			if (walkDir == down)
				moveY -= 2;

			moveTo(getX() + moveX, getY() + moveY);
			changeTicks(-1);
			if (getTicks() == 0)
			{
				setState(PAUSED);
				setPauseCounter(180);
			}
			return;
		}
		if (isDirectlyOnSquare(this) && isAtFork(this))
		{
			int dir = randInt(0, 3) * 90;
			while (!canMoveInDir(this, dir))
			{
				dir = randInt(0, 3) * 90;
			}
			switch (dir)
			{
			case left:
				setWalkDir(left);
				setDirection(left);
				break;
			case right:
				setWalkDir(right);
				setDirection(right);
				break;
			case up:
				setWalkDir(up);
				setDirection(right);
				break;
			case down:
				setWalkDir(down);
				setDirection(right);
				break;

			}
		}

		possibleMoveOptions(this, moveX, moveY);

		if (getWalkDir() == left)
			setDirection(left);
		else
			setDirection(right);

		moveTo(getX() + moveX, getY() + moveY);
		changeTicks(-1);
		if (getTicks() == 0)
		{
			setState(PAUSED);
			setPauseCounter(180);
		}
	}
}

void Boo::whenImpacted()
{
	teleportRandom(this);
	setState(PAUSED);
	setPauseCounter(180);
}

Boo::~Boo()
{

}

// VORTEX IMPLEMENTATION
Vortex::Vortex(StudentWorld* studentWorld, PlayerAvatar* peach, PlayerAvatar* yoshi, int imageID, int startX, int startY, int dir)
	: Actor(studentWorld, imageID, startX, startY, dir, 0)
{
	Peach = peach;
	Yoshi = yoshi;
	m_dir = dir;

	switch (m_dir)
	{
	case up:
		setDirection(up);
		break;
	case down:
		setDirection(down);
		break;
	case left:
		setDirection(left);
		break;
	case right:
		setDirection(right);
		break;
	}
}

void Vortex::doSomething()
{
	if (!isActive())
		return;

	switch (getDir())
	{
	case up:
		moveTo(getX(), getY() + 2);
		break;
	case down:
		moveTo(getX(), getY() - 2);
		break;
	case left:
		moveTo(getX() - 2, getY());
		break;
	case right:
		moveTo(getX() + 2, getY());
		break;
	}

	if ((getX() < 0) || (getX() >= VIEW_WIDTH) || (getY() < 0) || (getY() >= VIEW_HEIGHT))
		setActive(false);

}

Vortex::~Vortex()
{

}

// HELPER FUNCTIONS
bool isDirectlyOnSquare(Character* character)
{
	if (character == nullptr)
		return false;
	
	int pX = character->getX();
	int pY = character->getY();

	if ((pX == 0) && (pY % SPRITE_HEIGHT == 0))
		return true;
	else if ((pX % SPRITE_WIDTH == 0) && (pY == 0))
		return true;
	else if ((pX % SPRITE_WIDTH == 0) && (pY % SPRITE_HEIGHT == 0))
		return true;
	else
		return false;
}

//bool isOnDirSquare(Character* ch)
//{
//	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;
//
//	if (ch->getBoard()->getContentsOf(graphX, graphY) == Board::right_dir_square)
//		return true;
//	if (ch->getBoard()->getContentsOf(graphX, graphY) == Board::left_dir_square)
//		return true;
//	if (ch->getBoard()->getContentsOf(graphX, graphY) == Board::up_dir_square)
//		return true;
//	if (ch->getBoard()->getContentsOf(graphX, graphY) == Board::down_dir_square)
//		return true;
//
//	return false;
//}

bool posIsSame(Actor* a1, Actor* a2)
{
	if (a1 == nullptr || a2 == nullptr)
		return false;

	if ((a1->getX() == a2->getX()) && (a1->getY() == a2->getY()))
		return true;
	else
		return false;
}

bool canMoveInDir(Character* ch, int dir)
{
	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;

	switch (dir)
	{
	case ch->right:
		if (ch->getBoard()->getContentsOf(graphX + 1, graphY) == Board::empty)
			return false;
		break;
	case ch->left:
		if (ch->getBoard()->getContentsOf(graphX - 1, graphY) == Board::empty)
			return false;
		break;
	case ch->up:
		if (ch->getBoard()->getContentsOf(graphX, graphY + 1) == Board::empty)
			return false;
		break;
	case ch->down:
		if (ch->getBoard()->getContentsOf(graphX, graphY - 1) == Board::empty)
			return false;
		break;
	}

	return true;
}

bool canMoveRight(Character* ch)
{
	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;

	if (ch->getBoard()->getContentsOf(graphX + 1, graphY) == Board::empty)
		return false;

	return true;
}

bool canMoveLeft(Character* ch)
{
	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;

	if (ch->getBoard()->getContentsOf(graphX - 1, graphY) == Board::empty)
		return false;

	return true;
}

bool canMoveUp(Character* ch)
{
	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;

	if (ch->getBoard()->getContentsOf(graphX, graphY + 1) == Board::empty)
		return false;

	return true;
}

bool canMoveDown(Character* ch)
{
	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;

	if (ch->getBoard()->getContentsOf(graphX, graphY - 1) == Board::empty)
		return false;

	return true;
}

bool isAtFork(Character* ch)
{
	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;
	int dir = ch->getWalkDir();
	int paths = 0;

	if (canMoveRight(ch))
		paths++;
	if (canMoveLeft(ch))
		paths++;
	if (canMoveUp(ch))
		paths++;
	if (canMoveDown(ch))
		paths++;
	
	if (paths > 2)
		return true;
	else
		return false;

}

void possibleMoveOptions(Character* ch, int& moveX, int& moveY)
{
	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;
	int dir = ch->getWalkDir();

	switch (dir)
	{
	case ch->right:
		if ((ch->getBoard()->getContentsOf(graphX + 1, graphY) == Board::empty) && (ch->getBoard()->getContentsOf(graphX, graphY + 1) == Board::empty)) // top right border
		{
			ch->setWalkDir(ch->down);
			moveY -= 2;
		}
		else if ((ch->getBoard()->getContentsOf(graphX + 1, graphY) == Board::empty) && (ch->getBoard()->getContentsOf(graphX, graphY - 1) == Board::empty)) // bottom right border
		{
			ch->setWalkDir(ch->up);
			moveY += 2;
		}
		else
		{
			moveX += 2;
		}
		break;
	case ch->left:
		if ((ch->getBoard()->getContentsOf(graphX - 1, graphY) == Board::empty) && (ch->getBoard()->getContentsOf(graphX, graphY + 1) == Board::empty)) // top left border
		{
			ch->setWalkDir(ch->down);
			moveY -= 2;
		}
		else if ((ch->getBoard()->getContentsOf(graphX - 1, graphY) == Board::empty) && (ch->getBoard()->getContentsOf(graphX, graphY - 1) == Board::empty)) // bottom left border
		{
			ch->setWalkDir(ch->up);
			moveY += 2;
		}
		else
		{
			moveX -= 2;
		}
		break;
	case ch->up:
		if ((ch->getBoard()->getContentsOf(graphX, graphY + 1) == Board::empty) && (ch->getBoard()->getContentsOf(graphX + 1, graphY) == Board::empty)) // top right border
		{
			ch->setWalkDir(ch->left);
			moveX -= 2;
		}
		else if ((ch->getBoard()->getContentsOf(graphX, graphY + 1) == Board::empty) && (ch->getBoard()->getContentsOf(graphX - 1, graphY) == Board::empty)) // top left border
		{
			ch->setWalkDir(ch->right);
			moveX += 2;
		}
		else
		{
			moveY += 2;
		}
		break;
	case ch->down:
		if ((ch->getBoard()->getContentsOf(graphX, graphY - 1) == Board::empty) && (ch->getBoard()->getContentsOf(graphX + 1, graphY) == Board::empty)) // bottom right border
		{
			ch->setWalkDir(ch->left);
			moveX -= 2;
		}
		else if ((ch->getBoard()->getContentsOf(graphX, graphY - 1) == Board::empty) && (ch->getBoard()->getContentsOf(graphX - 1, graphY) == Board::empty)) // bottom left border
		{
			ch->setWalkDir(ch->right);
			moveX += 2;
		}
		else
		{
			moveY -= 2;
		}
		break;
	}
}

void teleportRandom(Character* ch)
{
	int randX = randInt(0, 16);
	int randY = randInt(0, 16);

	while (ch->getBoard()->getContentsOf(randX, randY) == Board::empty)
	{
		randX = randInt(0, 16);
		randY = randInt(0, 16);
	}

	randX *= SPRITE_WIDTH;
	randY *= SPRITE_HEIGHT;

	ch->moveTo(randX, randY);

	randX /= SPRITE_WIDTH;
	randY /= SPRITE_HEIGHT;

	int dir = ch->getWalkDir();

	if ((ch->getBoard()->getContentsOf(randX, randY + 1) == Board::empty) || (ch->getBoard()->getContentsOf(randX, randY - 1) == Board::empty))
	{
		int rand = randInt(1, 2);

		if (rand == 1)
		{
			ch->setWalkDir(ch->right);
			ch->setDirection(ch->right);
		}
		else
		{
			ch->setWalkDir(ch->left);
			ch->setDirection(ch->left);
		}
	}
	if ((ch->getBoard()->getContentsOf(randX + 1, randY) == Board::empty) || (ch->getBoard()->getContentsOf(randX - 1, randY) == Board::empty))
	{
		int rand = randInt(1, 2);

		if (rand == 1)
		{
			ch->setWalkDir(ch->up);
			ch->setDirection(ch->right);
		}
		else
		{
			ch->setWalkDir(ch->down);
			ch->setDirection(ch->right);
		}
	}
}

//bool characterIsOnSquare(Character* c, Square* s)
//{
//	while (c != nullptr && s != nullptr)
//	{
//		if (isDirectlyOnSquare(c) && posIsSame(c, s))
//			return true;
//		else
//			return false;
//	}
//}