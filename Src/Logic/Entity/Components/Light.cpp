/**
@file Light.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CLight
@see Logic::IComponent

@author Emilio Santalla
@date Marzo, 2013
*/

#include "Light.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Light.h"
#include "Logic/Maps/EntityFactory.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageTF.h"


namespace Logic 
{
	IMP_FACTORY(CLight);
	
	//---------------------------------------------------------

	CLight::~CLight() 
	{
		if(_graphicalLight)
		{
			_scene->remove(_graphicalLight);			
			delete _graphicalLight;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CLight::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getGraphicScene();
			assert(_scene && "Escena gráfica es NULL");
		assert(entityInfo->hasAttribute("type"));
			_type = entityInfo->getStringAttribute("type");

		_graphicalLight = createGraphicalLight(entityInfo);
			if(!_graphicalLight)
				return false;
		
		_graphicalLight->setPosition(_entity->getPosition());
	

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLight::accept(const CMessage *message)
	{
		return	 message->getType() == Message::SET_TRANSFORM;
	} // accept
	
	//---------------------------------------------------------

	void CLight::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::SET_TRANSFORM:
			{
			CMessageTF *maux = static_cast<CMessageTF*>(message);
			Vector3 posaux;
			Vector3 scaleaux;
			Quaternion orientaux;
			maux->getTransform().decomposition(posaux, scaleaux, orientaux);
			_graphicalLight->setPosition(posaux);
			}
			break;
		}

	} // process

	//---------------------------------------------------------

	Graphics::CLight* CLight::createGraphicalLight(const Map::CEntity *entityInfo)
	{		
		assert( _scene && "LOGIC::GRAPHICS>> No existe escena gráfica!");
		
		Graphics::CLight* light = new Graphics::CLight(_entity->getName(), _scene);

		if(entityInfo->hasAttribute("lightType"))
		{
			light->setType(entityInfo->getStringAttribute("lightType"));
		}
		
		if(entityInfo->hasAttribute("diffuseColour"))
		{
			Vector3 diffuseColour = entityInfo->getVector3Attribute("diffuseColour");
			light->setDiffuseColour(diffuseColour.x, diffuseColour.y, diffuseColour.z);
		}

		if(entityInfo->hasAttribute("specularColour"))
		{
			Vector3 specularColour = entityInfo->getVector3Attribute("specularColour");
			light->setDiffuseColour(specularColour.x, specularColour.y, specularColour.z);
		}

		if(entityInfo->hasAttribute("direction"))
		{
			Vector3 direction = entityInfo->getVector3Attribute("direction");
			light->setDirection(direction);
		}

		_scene->add(light);

		return light;

	} // createGraphicalEntity
	
	

} // namespace Logic