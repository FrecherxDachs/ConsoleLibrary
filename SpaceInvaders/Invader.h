#pragma once

#ifndef INVADER_H
#define INVADER_H

#include "GameFigure.h"

namespace Model
{
	class Invader : public GameFigure
	{
	private:
		int m_HitPoints;
	public:
		Invader(Vector2D* position, Vector2D* direction, int hitpoints);
		int getHitPoints();

		void setHitPoints(int hitpoints);

		void decrementHitPoints();

		int isPolymorphic() { return 3; };  
	};
}

#endif