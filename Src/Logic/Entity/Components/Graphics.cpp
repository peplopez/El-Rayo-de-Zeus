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
#include "Map/Entity.h"

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
		if(_graphicalEntity)				
			_scene->remove(_graphicalEntity);
		
		delete	_graphicalEntity;	
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
			assert(_scene && "Escena gráfica es NULL");

		assert(entityInfo->hasAttribute("model"));
			_model = entityInfo->getStringAttribute("model");

		_graphicalEntity = createGraphicalEntity(entityInfo);
			if(!_graphicalEntity)
				return false;

		_graphicalEntity->setTransform(_entity->getTransform());

	// READ MATERIALS & COLORS
		std::string varMaterial("modelMaterial0");	
		std::string varColor("modelColor0");
		const unsigned int N = _graphicalEntity->getNumSubEntities();
			for(unsigned int entIndex = 0; entIndex < N; ++entIndex) 
			{
				if( entityInfo->hasAttribute(varMaterial) )
					_graphicalEntity->setMaterial(	
						entityInfo->getStringAttribute(varMaterial), entIndex);
				if( entityInfo->hasAttribute(varColor) ) 				
					_graphicalEntity->setColor(		
						entityInfo->getStringAttribute(varColor),	 entIndex); 
			
				++varMaterial	[	varMaterial.length()- 1 ];
				++varColor		[	varColor.length()	- 1 ];
			}
	
	// SCALE
		if(entityInfo->hasAttribute("modelScale") )
			_graphicalEntity->setScale( entityInfo->getFloatAttribute("modelScale") );

	// ATTACHs
	// TODO FRS Esto estaría guapo tron extraerlo directamente como lista de pares desde el map.txt
		if(entityInfo->hasAttribute("modelFacial"))
			_graphicalEntity->attach( Graphics::TAttachPoint::FACE, 
				entityInfo->getStringAttribute("modelFacial") );

		if(entityInfo->hasAttribute("modelWeaponR"))
			_graphicalEntity->attach( Graphics::TAttachPoint::HAND_R, 
				entityInfo->getStringAttribute("modelWeaponR") );

		if(entityInfo->hasAttribute("modelWeaponL"))
			_graphicalEntity->attach( Graphics::TAttachPoint::HAND_L, 
				entityInfo->getStringAttribute("modelWeaponL") );

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
				 message->getType() == Message::RESET_MATERIAL ||			
				 message->getType() == Message::ATTACH ||
				 message->getType() == Message::SET_SCALE;

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
			CMessageUIntString *rxMsg = static_cast<CMessageUIntString*>(message);
			_graphicalEntity->setMaterial(rxMsg->getString(), rxMsg->getUInt() );		
		}	break;

		case Message::RESET_MATERIAL:{
			CMessageUInt *rxMsg = static_cast<CMessageUInt*>(message);
			_graphicalEntity->resetLastMaterial( rxMsg->getUInt() );
		}	break;

		case Message::ATTACH: {
			CMessageString *rxMsg = static_cast<CMessageString*>(message);
			switch( message->getAction() ) {
				case Message::ATTACH_TO_HEAD:	
					_graphicalEntity->attach( Graphics::TAttachPoint::HEAD, rxMsg->getString() );	
					break;
				case Message::ATTACH_TO_HAND:	
					_graphicalEntity->attach( Graphics::TAttachPoint::HAND_R, rxMsg->getString() );	
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
		assert( _scene && "LOGIC::GRAPHICS>> No existe escena gráfica!");
		assert( _model.length() > 0  && "LOGIC::GRAPHICS>> No existe modelo!");	
		
		bool isStatic = false;
			if(entityInfo->hasAttribute("static"))
				isStatic = entityInfo->getBoolAttribute("static");

		Graphics::CEntity* graphicalEntity = 
			new Graphics::CEntity(_entity->getGraphicalName(), _model, isStatic);
		if( _scene->add(graphicalEntity) )		
			return graphicalEntity;
		else
			return 0;
		

	} // createGraphicalEntity
	
	

} // namespace Logic

