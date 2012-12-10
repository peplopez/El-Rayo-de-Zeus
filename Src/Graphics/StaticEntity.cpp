//---------------------------------------------------------------------------
// StaticEntity.cpp
//---------------------------------------------------------------------------

/**
@file StaticEntity.cpp

Contiene la implementación de la clase que representa una entidad gráfica 
estática.

@see Graphics::CStaticEntity
@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "StaticEntity.h"
#include "Scene.h"

#include <assert.h>

#include <OgreSceneManager.h>
#include <OgreStaticGeometry.h>

namespace Graphics 
{
		
	bool CStaticEntity::addToStaticGeometry()
	{
		if(_entityNode)
		{
			_scene->getStaticGeometry()->addSceneNode(_entityNode);
			_entityNode->detachAllObjects();
			return true;
		}
		return false;

	} // addToStaticGeometry


} // namespace Graphics
