//---------------------------------------------------------------------------
// AnimSet.cpp
//---------------------------------------------------------------------------

/**
@file AnimSet.cpp

Contiene la implementación de la clase que reprenta una correspondencia entre nombrés lógicos de animación y nombres físicos

@see Graphics::CAnimSet

@author Jose Luis López
@date Mayo, 2013
*/
#include <assert.h>
#include "AnimSet.h"

//#include "Graphics\Entity.h"
//#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

namespace Graphics 
{
	
	/**
	Constructor de la clase.

	@param entityInfo información del Map para leer la información acerca de la animación
	*/
	CAnimSet::CAnimSet()
	{
	}

	/**
	Destructor de la clase.
	*/
	CAnimSet::~CAnimSet()
	{
		_animationSet.clear();
		delete this;
	}

	//--------------------------------------------------------
		
	bool CAnimSet::addAnimation(const Logic::AnimationName animEnum, const std::string realName)
	{
		_animationSet[animEnum]=realName;

		return true; // TODO, hacer que devuelva false ante un error.
	} // addAnimation
	
	std::string CAnimSet::getAnimation(const Logic::AnimationName animEnum)
	{
		std::string animation;
		animation=_animationSet[animEnum];
		return animation;
	} // getAnimation

	//--------------------------------------------------------
	

} // namespace Graphics
