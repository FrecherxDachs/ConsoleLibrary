#include "InvaderController.h"
#include "GameFieldController.h"
#include "GameFigureController.h"
#include "CollisionDetectorController.h"

#include "GameField.h"
#include "Invader.h"
#include "GameFigure.h"
#include "../ConsoleLibrary/Console.h"

using namespace Model;

namespace Controller
{
	GameFieldController::GameFieldController(std::vector<COLOR_ID>* gameColorIds)
	{
		m_InvaderController = new InvaderController();
		m_GameFigureController = new GameFigureController();
		m_CollisionDetectorController = new CollisionDetectorController();
		m_GameColorIds = gameColorIds;
	}

	GameFieldController::~GameFieldController()
	{
		delete m_CollisionDetectorController;
		delete m_GameFieldModel;
		delete m_InvaderController;
		delete m_GameColorIds;
	}
	
	void GameFieldController::setGameFieldModel(GameField* gameField)
	{
		m_GameFieldModel = gameField;
	}

	void GameFieldController::initializeGameField()
	{
		// Invader Initialisation
		std::vector<Invader*>* invaderVector = m_InvaderController->getDefaultInvaderVector(m_GameColorIds);

		m_GameFieldModel->setInvaderVector(invaderVector);

		// Shield Initialisation
		std::vector<Shield*>* shieldVector = this->getDefaultShieldVector();

		m_GameFieldModel->setShieldVector(shieldVector);

		// Player Initialisation
		Vector2D* positionPlayer = new Vector2D(PLAYERPOSITIONX, PLAYERPOSITIONY);
		Player* player = new Player(positionPlayer);
		player->setColor((*m_GameColorIds)[0]);

		m_GameFieldModel->setPlayer(player);
	}

	void GameFieldController::updateGameField()
	{
		std::vector<Invader*>* invaderVector = &m_GameFieldModel->getInvaderVector();
		std::vector<Bullet*>* bulletVector = &m_GameFieldModel->getBulletVector();

		// Compute Collisions
		bool gameOver = m_CollisionDetectorController->computeCollisionOfGameFigure(&m_GameFieldModel->getInvaderVector(), &m_GameFieldModel->getShieldVector(), &m_GameFieldModel->getBulletVector(), &m_GameFieldModel->getPlayer());
		
		if (gameOver)
		{
			this->initializeGameField();
		}

		// Update according to direction vector
		for (std::vector<Invader*>::iterator iterator = invaderVector->begin(); iterator != invaderVector->end(); ++iterator)
		{
			Invader* invader = *iterator;
			Vector2D* invaderDirection = &invader->getDirection();
			Vector2D* invaderPosition = &invader->getPosition();

			*invaderPosition += *invaderDirection;
		}

		for (int bulletCounter = 0; bulletCounter < bulletVector->size(); bulletCounter++)
		{
			Bullet* bullet = (*bulletVector)[bulletCounter];
			Vector2D* bulletDirection = &bullet->getDirection();
			Vector2D* bulletPosition = &bullet->getPosition();

			// Bullet out of bounds
			if (bulletPosition->getY() - 1 < 0 || bulletPosition->getY() + 1 > GAMEMATRIXHEIGTH)
			{
				bulletVector->erase(bulletVector->begin() + bulletCounter);
				delete bullet;

				bulletCounter--;
			} 
			else
			{
				*bulletPosition += *bulletDirection;
			}
		}
	}

	std::vector<Shield*>* GameFieldController::getDefaultShieldVector()
	{
		int totalShieldPieces = SHIELDWIDTH * SHIELDCOUNT;
		std::vector<Shield*>* shieldVector = new std::vector<Shield*>(totalShieldPieces);

		Colors charColor = BLUE;
		Colors backgroundColor = MAGENTA; 

		int x = SHIELDOFFSETX;

		for (int shieldCounter = 0; shieldCounter < totalShieldPieces; shieldCounter++)
		{
			// for cleaning up initialization values
			delete (*shieldVector)[shieldCounter];

			Vector2D* position = new Vector2D(x, SHIELDPOSITIONY);
			Shield* tempShield = new Shield(position);
			tempShield->setColor((*m_GameColorIds)[1]);
			(*shieldVector)[shieldCounter] = tempShield;

			if (shieldCounter % SHIELDWIDTH == SHIELDWIDTH - 1)
			{
				x += SHIELDPADDINGX;
			} 
			else
			{
				x++;
			}
		}

		return shieldVector;
	}

	void GameFieldController::shootBullet(GameFigure* gameFigure)
	{
		std::vector<Bullet*>* bulletVector = &m_GameFieldModel->getBulletVector();
		
		Invader* currentInvader = dynamic_cast<Model::Invader*>(gameFigure);
		Player* currentPlayer = dynamic_cast<Model::Player*>(gameFigure);

		Colors charColor = WHITE;
		Colors backgroundColor = BLACK;

		Bullet* bullet = nullptr;

		if (currentInvader != nullptr)
		{
			Vector2D* invaderPosition = &gameFigure->getPosition();
			Vector2D* bulletPosition = new Vector2D(invaderPosition->getX(), invaderPosition->getY() + 1);
			Vector2D* bulletDirection = new Vector2D(0, 1);
			bullet = new Bullet(bulletPosition, bulletDirection, gameFigure);
		}
		else if (currentPlayer != nullptr)
		{
			Vector2D* playerPosition = &gameFigure->getPosition();
			Vector2D* bulletPosition = new Vector2D(playerPosition->getX(), playerPosition->getY() - 1);
			Vector2D* bulletDirection = new Vector2D(0, -1);
			bullet = new Bullet(bulletPosition, bulletDirection, gameFigure);
		}

		if (bullet != nullptr)
		{
			bullet->setColor((*m_GameColorIds)[2]);
			bulletVector->push_back(bullet);
		}
	}
}