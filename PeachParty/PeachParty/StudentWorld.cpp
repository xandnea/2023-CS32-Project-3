#include "StudentWorld.h"
#include "Actor.h"
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
    Peach = nullptr;
    Yoshi = nullptr;
    m_bank = 0;
}

int StudentWorld::init() 
{
    // might need to change back to "board01.txt" for part 1
    m_boardFile = "board0" + to_string(getBoardNumber()) + ".txt";

    Actor* actor;

    //Board bd;
    
    string board_file = assetPath() + m_boardFile; // replace "board01.txt" w/ m_boardFile
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
        cerr << "Could not find board01.txt data file\n";
    else if (result == Board::load_fail_bad_format)
        cerr << "Your board was improperly formatted\n";
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";

        Board* boardP = &bd;

        for (int X = 0; X < 16; X++)
            for (int Y = 0; Y < 16; Y++)
            {
                Board::GridEntry ge = bd.getContentsOf(X, Y);

                switch (ge) {
                case Board::empty:
                    break;
                case Board::boo:
                    actor = new Boo(this, boardP, IID_BOO, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    actor = new CoinSquare(this, Peach, Yoshi, PLUS, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::bowser:
                    actor = new Bowser(this, boardP, IID_BOWSER, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    actor = new CoinSquare(this, Peach, Yoshi, PLUS, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::player:
                    Peach = new PlayerAvatar(this, boardP, PLAYER_ONE, IID_PEACH, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    Yoshi = new PlayerAvatar(this, boardP, PLAYER_TWO, IID_YOSHI, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actor = new CoinSquare(this, Peach, Yoshi, PLUS, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::red_coin_square:
                    actor = new CoinSquare(this, Peach, Yoshi, MINUS, IID_RED_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::blue_coin_square:
                    actor = new CoinSquare(this, Peach, Yoshi, PLUS, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::star_square:
                    actor = new StarSquare(this, Peach, Yoshi, IID_STAR_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::event_square:
                    actor = new EventSquare(this, Peach, Yoshi, IID_EVENT_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::bank_square:
                    actor = new BankSquare(this, Peach, Yoshi, IID_BANK_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::right_dir_square:
                    actor = new DirectionalSquare(this, Peach, Yoshi, '>', IID_DIR_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::left_dir_square:
                    actor = new DirectionalSquare(this, Peach, Yoshi, '<', IID_DIR_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::up_dir_square:
                    actor = new DirectionalSquare(this, Peach, Yoshi, '^', IID_DIR_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                case Board::down_dir_square:
                    actor = new DirectionalSquare(this, Peach, Yoshi, 'v', IID_DIR_SQUARE, SPRITE_WIDTH * X, SPRITE_HEIGHT * Y);
                    actors.push_back(actor);
                    break;
                }
            }
    }


            


    startCountdownTimer(99);  
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    Peach->doSomething();
    Yoshi->doSomething();
    
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->isActive())
            actors[i]->doSomething();
        else 
        {
            delete actors[i];
            actors.erase(actors.begin() + i);
        }
    }

    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        if (Peach->getNumStars() > Yoshi->getNumStars())
        {
            setFinalScore(Peach->getNumStars(), Peach->getNumCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (Peach->getNumStars() < Yoshi->getNumStars())
        {
            setFinalScore(Yoshi->getNumStars(), Yoshi->getNumCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else // if peach and yoshi stars are equal
        {
            if (Peach->getNumCoins() > Yoshi->getNumCoins())
            {
                setFinalScore(Peach->getNumStars(), Peach->getNumCoins());
                return GWSTATUS_PEACH_WON;
            }
            else if (Peach->getNumCoins() < Yoshi->getNumCoins())
            {
                setFinalScore(Yoshi->getNumStars(), Yoshi->getNumCoins());
                return GWSTATUS_YOSHI_WON;
            }
            else // if peach and yoshi stars and coins are equal
            {
                int winner = randInt(1, 2);
                if (winner == 1)
                {
                    setFinalScore(Peach->getNumStars(), Peach->getNumCoins());
                    return GWSTATUS_PEACH_WON;
                }
                else
                {
                    setFinalScore(Yoshi->getNumStars(), Yoshi->getNumCoins());
                    return GWSTATUS_YOSHI_WON;
                }
            }
        }
    }

    setGameStatText("P1 Roll: " + to_string(Peach->getDieRoll()) + " Stars: " + to_string(Peach->getNumStars()) + " $$: "
        + to_string(Peach->getNumCoins()) + Peach->hasVortex() + " | Time: " + to_string(timeRemaining()) + " | Bank: "
        + to_string(m_bank) + " | P2 Roll: " + to_string(Yoshi->getDieRoll()) + " Stars: "
        + to_string(Yoshi->getNumStars()) + " $$: " + to_string(Yoshi->getNumCoins()) + Yoshi->hasVortex());
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete Peach;
    delete Yoshi;

    for (int i = 0; i < actors.size(); i++) {
        delete actors[i];
    }
    actors.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}