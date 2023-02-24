#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
}

int StudentWorld::init() 
{
    // might need to check if the file is inputted correctly
    m_boardFile = "board0" + to_string(getBoardNumber()) + ".txt";
    m_board.loadBoard(m_boardFile);

    m_bank = 0;

    int X, Y;
    Actor* actor;

    // check every position on the board and insert the objects accordingly
    for (X = 0; X < 16; X++)
        for (Y = 0; Y < 16; Y++) {
            char curr = m_board.getContentsOf(X, Y);
            
            switch (curr) 
            {
            case '@':
                Peach = new PlayerAvatar(this, PLAYER_ONE, IID_PEACH, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Yoshi = new PlayerAvatar(this, PLAYER_TWO, IID_YOSHI, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                actor = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                break;
            case '+':
                actor = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                break;
            case '-':
                actor = new CoinSquare(this, IID_RED_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                break;
            case '*':
                actor = new StarSquare(this, IID_STAR_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                break;
            case '!':
                actor = new EventSquare(this, IID_EVENT_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                break;
            case 'B':
                actor = new Bowser(this, IID_BOWSER, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                actor = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                break;
            case 'b':
                actor = new Boo(this, IID_BOO, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                actor = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                Actors.push_back(actor);
                break;
            case '>':
                break;
            case '<':
                break;
            case '^':
                break;
            case 'v':
                break;
            case ' ':
                break;
            }
        }

    startCountdownTimer(99);  
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}

void StudentWorld::getPlayerStartingPos(int& x, int& y)
{
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++) {
            if (m_board.getContentsOf(i, j) == '@')
                x = i; y = j;
        }
}