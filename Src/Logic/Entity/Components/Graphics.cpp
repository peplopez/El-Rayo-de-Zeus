/**
@file Graphics.cpp

Contiene la implementaci�n del componente que controla la representaci�n
gr�fica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Logic/Maps/EntityFactory.h"


#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageTF.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageUIntString.h"
#include "Logic/Entity/Messages/MessageUInt.h"



namespace Logic 
{
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicalEntity){			
			_scene->remove(_graphicalEntity);
			delete _graphicalEntity;
		}

	} // ~CGraphics

	//---------------------------------------------------------

	void CGraphics::detachFromMap()
	{
		_scene->remove(_graphicalEntity);
	}

	//---------------------------------------------------------

	void CGraphics::attachToMap(CMap* map)
	{
		_scene = map->getGraphicScene();
		_scene->add(_graphicalEntity);
	}

	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getGraphicScene();
			assert(_scene && "Escena gr�fica es NULL");

		assert(entityInfo->hasAttribute("model"));
			_model = entityInfo->getStringAttribute("model");

		_graphicalEntity = createGraphicalEntity(entityInfo);
			if(!_graphicalEntity)
				return false;
		
		Vector3 initialScale = Vector3::UNIT_SCALE;

	// HACK FRS Esto lo suyo es que el modelo ya lo traiga , no?
	// o meter la escala como vector en el map
		if(_entity->getType() == "World"
			&& _entity->getLogicalPosition()->getRing() == LogicalPosition::CENTRAL_RING)
			initialScale = Vector3(1.3f,1.0f,1.3f);
	//

		else if(entityInfo->hasAttribute("initialScale") )
			initialScale *=  entityInfo->getFloatAttribute("initialScale");
			
		if(entityInfo->hasAttribute("material"))
		{
			_graphicalEntity->setMaterial(entityInfo->getStringAttribute("material"));
		}

		if(entityInfo->hasAttribute("submaterial0"))
		{
			_graphicalEntity->setSubEntityMaterial(entityInfo->getStringAttribute("submaterial0"), 0);
		}

		if(entityInfo->hasAttribute("submaterial1"))
		{
			_graphicalEntity->setSubEntityMaterial(entityInfo->getStringAttribute("submaterial1"), 1);
		}

		if(entityInfo->hasAttribute("initialMaterial"))
			_initialMaterial = entityInfo->getStringAttribute("initialMaterial");
	


		_graphicalEntity->setTransform(_entity->getTransform());
		_graphicalEntity->setScale(initialScale);
				

	// ATTACHs
	// TODO  FRS Esto estar�a guapo tron extraerlo directamente como lista de pares desde el map.txt
		if(entityInfo->hasAttribute("modelFacial"))
			_graphicalEntity->attach( Graphics::TAttachPoint::FACE, 
				entityInfo->getStringAttribute("modelFacial") );

		if(entityInfo->hasAttribute("modelWeapon"))
			_graphicalEntity->attach( Graphics::TAttachPoint::HAND_R, 
				entityInfo->getStringAttribute("modelWeapon") );

		if(entityInfo->hasAttribute("modelShield"))
			_graphicalEntity->attach( Graphics::TAttachPoint::ARM_L, 
				entityInfo->getStringAttribute("modelShield") );		

		if(entityInfo->hasAttribute("modelHelmet"))
			_graphicalEntity->attach( Graphics::TAttachPoint::HEAD, 
				entityInfo->getStringAttribute("modelHelmet") );	
	//
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CGraphics::accept(const CMessage *message)
	{
		return	 message->getType() == Message::SET_TRANSFORM ||
				 message->getType() == Message::SET_MATERIAL ||
				 message->getType() == Message::SET_SUBENTITY_MATERIAL ||
				 message->getType() == Message::ATTACH ||
				 message->getType() == Message::SET_SCALE ||
				 message->getType() == Message::SET_INITIAL_MATERIAL;

	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(CMessage *message)
	{
		switch( message->getType() ) {
		
		case Message::SET_TRANSFORM: {			
			CMessageTF *rxMsg = static_cast<CMessageTF*>(message);
			_graphicalEntity->setTransform(rxMsg->getTransform());			
		}	break;
	
		case Message::SET_MATERIAL: {			
			CMessageString *rxMsg = static_cast<CMessageString*>(message);
			_graphicalEntity->setMaterial(rxMsg->getString());			
		}	break;

		case Message::SET_SUBENTITY_MATERIAL:{
			CMessageUIntString *rxMsg = static_cast<CMessageUIntString*>(message);
			_graphicalEntity->setSubEntityMaterial(rxMsg->getString(), rxMsg->getUInt());
		}	break;

		case Message::SET_INITIAL_MATERIAL:{
			CMessageUInt *rxMsg = static_cast<CMessageUInt*>(message);
			_graphicalEntity->setSubEntityMaterial(_initialMaterial, rxMsg->getUInt());
		}	break;

		case Message::ATTACH: {
			CMessageString *rxMsg = static_cast<CMessageString*>(message);
			switch( message->getAction() ) {
				case Message::ATTACH_TO_HEAD:	
					_graphicalEntity->attach( Graphics::TAttachPoint::HEAD, rxMsg->getString() );	
					break;
				case Message::ATTACH_TO_HAND:	
					_graphicalEntity->attach( Graphics::TAttachPoint::HAND_R,	rxMsg->getString() );	
					break;
				case Message::DETACH_FROM_HEAD:	
					_graphicalEntity->detach( Graphics::TAttachPoint::HEAD );	
					break;
				case Message::DETACH_FROM_HAND:	
					_graphicalEntity->detach( Graphics::TAttachPoint::HAND_R );	
					break;
			}
		} break;
		case Message::SET_SCALE:{
				CMessageFloat *maux4 = static_cast<CMessageFloat*>(message);
				
				Vector3 escalaInicial=_graphicalEntity->getScale();
				//(maux4->getFloat(),maux4->getFloat(),maux4->getFloat());
				if (maux4->getAction()==Message::X_AXIS)
					escalaInicial.x=maux4->getFloat();
				if (maux4->getAction()==Message::Y_AXIS)
					escalaInicial.y=maux4->getFloat();
				if (maux4->getAction()==Message::Z_AXIS)
					escalaInicial.z=maux4->getFloat();
				if (maux4->getAction()==Message::UNDEF)
				{
					escalaInicial.x=maux4->getFloat();
					escalaInicial.y=maux4->getFloat();
					escalaInicial.z=maux4->getFloat();
				}
					
				_graphicalEntity->setScale(escalaInicial);
				
			} break;

		} // switch

	} // process

	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicalEntity(const Map::CEntity *entityInfo)
	{		
		assert( _scene && "LOGIC::GRAPHICS>> No existe escena gr�fica!");
		assert( _model.length() > 0  && "LOGIC::GRAPHICS>> No existe modelo!");	
		
		bool isStatic = false;
			if(entityInfo->hasAttribute("static"))
				isStatic = entityInfo->getBoolAttribute("static");

		std::stringstream ssAux; // FRS Importante a�adir ID para evitar entidades gr�ficas con = nombre
			ssAux << _entity->getName() << _entity->getEntityID();
			std::string	name = ssAux.str();

		Graphics::CEntity* graphicalEntity = new Graphics::CEntity(name, _model, isStatic);
			if( _scene->add(graphicalEntity) )		
				return graphicalEntity;
			else
				return 0;
		

	} // createGraphicalEntity
	
	

} // namespace Logic

