#include "../ConsoleLibrary/Console.h"

#include "InvaderController.h"

#include "Invader.h"
#include "Vector2D.h"


using namespace Model;

namespace Controller
{
	std::vector<Invader*>* InvaderController::getDefaultInvaderArray()
	{
		vector<Invader*>* invaderVector = new vector<Invader*>(INVADERARRAYLENGTH);

		int x = 0;
		int y = 0;

		for (int invaderCounter = 0; invaderCounter < INVADERARRAYLENGTH; invaderCounter++)
		{
			if (invaderCounter % 10 == 0)
			{
				y += INVADEROFFSETY;
				x = INVADEROFFSETX;
			}

			Vector2D* position = new Vector2D(x, y);
			Vector2D* direction = new Vector2D(x, y);
			Invader* tempInvader; 
			
			if (y == INVADEROFFSETY)
			{
				tempInvader = new Invader(position, direction, 2); 
				tempInvader->GameFigure::setCharColor(RED);
			} else {
				tempInvader = new Invader(position, direction, 1); 
				tempInvader->setCharColor(GREEN);
			}

			(*invaderVector)[invaderCounter] = tempInvader;
			x += INVADERPADDING;	
		}

		return invaderVector;
	}
}