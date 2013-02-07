/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Graphics.h"
#include "LifeController.h"


#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"
#include "Logic/Maps/EntityFactory.h"

namespace Logic 
{
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicsEntity)
		{
			_scene->removeEntity(_graphicsEntity);
			delete _graphicsEntity;
			_graphicsEntity = 0;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();

		 

		if(entityInfo->hasAttribute("model"))
			_model = entityInfo->getStringAttribute("model");
	

		_graphicsEntity = createGraphicsEntity(entityInfo);


		if(_entity->getType().compare("Altar")==0)
		{	
			_graphicsEntity->setScale(3);
		}

		if(_entity->getName().compare("Tubo")==0)
		{	
			_graphicsEntity->setScale(Vector3(2,10,2));
		}

		
		if(_entity->getType().compare("AnimatedEntity")==0)
		{	
			_graphicsEntity->setScale(0.5);
		}

		if(_entity->getType().compare("World")==0)
		{	
			if(_entity->getRing()==LogicalPosition::ANILLO_CENTRAL)
			_graphicsEntity->setScale(Vector3(1.3,1.0,1.3));
		}

		
		if(!_graphicsEntity)
			return false;

		return true;

	} // spawn
	
	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		bool isStatic = false;
		if(entityInfo->hasAttribute("static"))
			isStatic = entityInfo->getBoolAttribute("static");

		float scale = 1.0;
		if (entityInfo->hasAttribute("scale"))
			scale = entityInfo->getFloatAttribute("scale");

		if(isStatic)
		{
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(),_model);
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity))
				return 0;
		}
		else
		{
			_graphicsEntity = new Graphics::CEntity(_entity->getName(),_model);
			if(!_scene->addEntity(_graphicsEntity))
				return 0;
		}

		_graphicsEntity->setScale(scale);
		_graphicsEntity->setTransform(_entity->getTransform());
		

		return _graphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	bool CGraphics::accept(const TMessage &message)
	{
		return message._type == Message::SET_TRANSFORM || message._type == Message::SET_TRANSFORM_QUAT || message._type == Message::SET_SHADER ;

	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::SET_TRANSFORM:
			_graphicsEntity->setTransform(message._transform);
			break;
		case Message::SET_TRANSFORM_QUAT:
			_graphicsEntity->setTransform(message._quat);
			break;
		case Message::SET_SHADER:
			_graphicsEntity->setMaterial(message._string);
			break;
		}

	} // process

} // namespace Logic

