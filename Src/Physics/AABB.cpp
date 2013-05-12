
// Includes
#include "AABB.h"


/*----------------------------------------------------------------------------------------------------------------------*/
namespace Physics
{
	float CAABB::computeArea()
	{
		return (_max.x - _min.x) + (_max.y - _min.y); //base * h
	}
	
	//----------------------------------------------------
	
	void CAABB::move(Vector2 diffPos)
	{
		_min += diffPos;
		_max += diffPos;
	}
	
	//----------------------------------------------------
	
	float CAABB::getDegree()
	{
		return _max.x - (_max.x - _min.x) * 0.5f;
	}

	//----------------------------------------------------

	float CAABB::getHeight()
	{
		return _max.y - (_max.y - _min.y);
	}
	
}
