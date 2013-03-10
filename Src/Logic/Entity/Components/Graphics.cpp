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

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"
#include "Logic/Maps/EntityFactory.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageTF.h"
#include "Logic/Entity/Messages/MessageString.h"

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

		_scene = map->getGraphicScene();
		createGraphicsEntity(entityInfo);

		// HACK FRS Esto lo suyo es que el modelo ya lo traiga , no?
		if(_entity->getType().compare("Altar")==0)			
			_graphicsEntity->setScale(3);	
		if(_entity->getName().compare("Tubo")==0)		
			_graphicsEntity->setScale(Vector3(2,10,2));	
		if(_entity->getType().compare("AnimatedEntity")==0)			
			_graphicsEntity->setScale(0.5);
		if(_entity->getType().compare("World")==0)
			if(_entity->getRing()==LogicalPosition::CENTRAL_RING)
				_graphicsEntity->setScale(Vector3(1.3,1.0,1.3));
		
		if(!_graphicsEntity)
			return false;

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CGraphics::accept(const CMessage *message)
	{
		return message->getType() == Message::SET_TRANSFORM || message->getType() == Message::SET_TRANSFORM_QUAT || message->getType() == Message::SET_SHADER ;

	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(CMessage *message)
	{
		CMessageTF *maux = static_cast<CMessageTF*>(message);
		CMessageString *maux2 = static_cast<CMessageString*>(message);
		switch(message->getType())
		{
		case Message::SET_TRANSFORM:
			_graphicsEntity->setTransform(maux->getTransform());
			break;
		case Message::SET_TRANSFORM_QUAT:
			//graphicsEntity->setTransform(message._quat);
			break;
		case Message::SET_SHADER:
			_graphicsEntity->setMaterial(maux2->getString());
			break;
		}

	} // process

	//---------------------------------------------------------

	void CGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{		
		assert(!_scene && "LOGIC::GRAPHICS>> No existe escena gráfica!");		

		assert(entityInfo->hasAttribute("model"));
			_model = entityInfo->getStringAttribute("model");

		bool isStatic = false;
			if(entityInfo->hasAttribute("static"))
				isStatic = entityInfo->getBoolAttribute("static");

		float scale = 1.0;
			if (entityInfo->hasAttribute("scale"))
				scale = entityInfo->getFloatAttribute("scale");

		// CREATE STATIC
		if(isStatic){
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(),_model);
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity)) {
				_graphicsEntity = 0;
				return;
			}

		// CREATE NO STATIC
		} else {
			_graphicsEntity = new Graphics::CEntity(_entity->getName(),_model);
			if(!_scene->addEntity(_graphicsEntity)){
				_graphicsEntity = 0;
				return;
			}
		}
		_graphicsEntity->setScale(scale);
		_graphicsEntity->setTransform(_entity->getTransform());

	} // createGraphicsEntity
	
	

} // namespace Logic

