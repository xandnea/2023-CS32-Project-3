#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Helpers
bool isDirectlyOnSquare(Character* character);
bool posIsSame(Actor* a1, Actor* a2);
bool isOnDirSquare(Character* ch);
bool canMoveInDir(Character* ch, int dir);
bool isAtFork(Character* ch);
void possibleMoveOptions(Character* ch, int& moveX, int& moveY);

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
	m_ticksToMove = 0; // maybe need to change
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

void PlayerAvatar::teleportRandom()
{
	int randX = randInt(0, 16);
	int randY = randInt(0, 16);

	while (getBoard()->getContentsOf(randX, randY) == Board::empty)
	{
		randX = randInt(0, 16);
		randY = randInt(0, 16);
	}

	randX *= SPRITE_WIDTH;
	randY *= SPRITE_HEIGHT;

	this->moveTo(randX, randY);

	randX /= SPRITE_WIDTH;
	randY /= SPRITE_HEIGHT;

	int dir = getWalkDir();

	if ((getBoard()->getContentsOf(randX, randY + 1) == Board::empty) || (getBoard()->getContentsOf(randX, randY - 1) == Board::empty))
	{
		int rand = randInt(1, 2);
		
		if (rand == 1)
		{
			setWalkDir(right);
			setDirection(right);
		}
		else
		{
			setWalkDir(left);
			setDirection(left);
		}
	}
	if ((getBoard()->getContentsOf(randX + 1, randY) == Board::empty) || (getBoard()->getContentsOf(randX - 1, randY) == Board::empty))
	{
		int rand = randInt(1, 2);

		if (rand == 1)
		{
			setWalkDir(up);
			setDirection(right);
		}
		else
		{
			setWalkDir(down);
			setDirection(right);
		}
	}
}

void PlayerAvatar::doSomething()
{
	if (getState() == WAITING_TO_ROLL) {
		// if the avatar has an invalid direction (due to being teleported): [done in randomTeleport()]

		int action = getWorld()->getAction(getPlayerNum());

		if (action == ACTION_ROLL) {
			//int roll = randInt(1, 10);
			int roll = 1;
			setDieRoll(roll);
			setTicks(m_dieRoll * 8);
			setState(WALKING);
		}
		// else if (action == ACTION_FIRE) {
		//
		// }
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

		if (isOnDirSquare(this))
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

bool Square::checkIfLandedOn(PlayerAvatar* player)
{
	if (isDirectlyOnSquare(player) && posIsSame(player, this))
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

	if (getPeach() == nullptr || getYoshi() == nullptr)
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

// does it give a star if the player lands directly?
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

void BankSquare::doSomething() // some squares don't register
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
		setPeachOn(false);
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
			getPeach()->teleportRandom();
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
			getPeach()->swapPosAndState(getYoshi());
			setYoshiOn(true);
			setPeachOn(true);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 3:
			getPeach()->setVortex(true);
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
			getYoshi()->teleportRandom();
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
			getYoshi()->swapPosAndState(getPeach());
			setPeachOn(true);
			setYoshiOn(true);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 3:
			getYoshi()->setVortex(true);
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
Enemy::Enemy(StudentWorld* studentWorld, Board* board, int imageID, int startX, int startY)
	: Character(studentWorld, board, PAUSED, imageID, startX, startY, 0)
{
	m_travelDist = 0;
}

Enemy::~Enemy()
{

}

// BOWSER IMPLEMENTATION
Bowser::Bowser(StudentWorld* studentWorld, Board* board, int imageID, int startX, int startY)
	: Enemy(studentWorld, board, imageID, startX, startY)
{

}

void Bowser::doSomething()
{

}

void Bowser::whenImpacted()
{

}

Bowser::~Bowser()
{

}

// BOO IMPLEMENTATION
Boo::Boo(StudentWorld* studentWorld, Board* board, int imageID, int startX, int startY)
	: Enemy(studentWorld, board, imageID, startX, startY)
{

}

void Boo::doSomething()
{

}

void Boo::whenImpacted()
{

}

Boo::~Boo()
{

}

// HELPER FUNCTIONS
bool isDirectlyOnSquare(Character* character)
{
	if (character == nullptr)
		return false;
	
	int pX = character->getX();
	int pY = character->getY();

	if (pX % SPRITE_WIDTH == 0 && pY % SPRITE_HEIGHT == 0)
		return true;
	else
		return false;
}

bool isOnDirSquare(Character* ch)
{
	int graphX = ch->getX() / SPRITE_WIDTH, graphY = ch->getY() / SPRITE_HEIGHT;

	if (ch->getBoard()->getContentsOf(graphX, graphY) == Board::right_dir_square)
		return true;
	if (ch->getBoard()->getContentsOf(graphX, graphY) == Board::left_dir_square)
		return true;
	if (ch->getBoard()->getContentsOf(graphX, graphY) == Board::up_dir_square)
		return true;
	if (ch->getBoard()->getContentsOf(graphX, graphY) == Board::down_dir_square)
		return true;

	return false;
}

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