
// Includes
#include "Circle.h"

#define PI 3.1415926535897932


/*----------------------------------------------------------------------------------------------------------------------*/
namespace Physics
{
	float CCircle::computeArea()
	{
		return PI * _radius * _radius;
	}
	
	//----------------------------------------------------
	
	void CCircle::move(Vector2 diffPos)
	{
		_position += diffPos;

		if (_position.x > 360)
			_position.x -= 360;
		else if (_position.x < 0)
			_position.x += 360;

		if (_position.y - _radius < 0)
			_position.y = _radius;

	}

	//----------------------------------------------------
	
	float CCircle::getDegree()
	{
		return _position.x;
	}

	//----------------------------------------------------

	float CCircle::getHeight()
	{
		float height = _position.y - _radius;
		return height;
	}
	
}
