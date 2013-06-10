/**
@file Entity.cpp

Contiene la implementación de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"

// Componentes
#include "Component.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "GUI/CameraController.h"
#include "GUI/HudController.h" //PT
#include "GUI/ShopController.h" //PT

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageTF.h"
#include "Logic/Entity/Messages/MessageBoolTF.h"

namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
				_map(0), _type(""), _name(""), _transform(Matrix4::IDENTITY),
				_isPlayer(false), _activated(false) // UNDONE ƒ®§ _pos(TLogicalPosition()) ya llama al ctro por defecto, por defecto, valga la rebuznancia
	{

	} // CEntity
	
	//---------------------------------------------------------

	CEntity::~CEntity()
	{
		assert(!_map && "¡¡Antes de destruir la entidad debe desacoplarse del mapa!!");

		destroyAllComponents();

	} // ~CEntity
	
	//---------------------------------------------------------

	bool CEntity::spawn(CMap *map, const Map::CEntity *entityInfo) 
	{
		// Leemos las propiedades comunes
		_map = map;
		_type = entityInfo->getType();
		_logicInput = false;
		_offsetHeight  = 0;
		Vector3 position = Vector3::ZERO;	
		_pos = new CLogicalPosition();
		if(entityInfo->hasAttribute("name"))
			_name = entityInfo->getStringAttribute("name");	

		if(entityInfo->hasAttribute("logicInput"))
			_logicInput = entityInfo->getBoolAttribute("logicInput");

		if(entityInfo->hasAttribute("degrees"))
			_pos->setDegree(entityInfo->getFloatAttribute("degrees"));

		if(entityInfo->hasAttribute("height"))
			_pos->setHeight(entityInfo->getFloatAttribute("height"));
			
			
		if(entityInfo->hasAttribute("sense"))
			_pos->setSense(static_cast<Logic::Sense>(entityInfo->getIntAttribute("sense")));
		else
			//situación anómala, se lanzaría una excepción o trazas por consola. Se le asigna por defecto dirección LEFT
			_pos->setSense(Logic::LogicalPosition::LEFT);


		//PT
		/*if(entityInfo->hasAttribute("initialMaterial"))
			_initialMaterial = entityInfo->getStringAttribute("initialMaterial");	
		else
			_initialMaterial = "marine";
			*/
		if(entityInfo->hasAttribute("initialMaterial0"))
			_initialMaterial0 = entityInfo->getStringAttribute("initialMaterial0");	
		else
			_initialMaterial0 = "marine";

		if(entityInfo->hasAttribute("initialMaterial1"))
			_initialMaterial1 = entityInfo->getStringAttribute("initialMaterial1");	
		else
			_initialMaterial1 = "";
		//FIN PT
		
		_initialScale= 1.0f;	
		if(entityInfo->hasAttribute("initialScale"))
			_initialScale = entityInfo->getFloatAttribute("initialScale");	
		
		if(entityInfo->hasAttribute("offsetHeight"))					
		{
			_offsetHeight=entityInfo->getFloatAttribute("offsetHeight");
		}

		if(entityInfo->hasAttribute("base"))					
		{
			_pos->setBase(entityInfo->getIntAttribute("base"));
			this->setOriginBase(_pos->getBase());
		}
		if(entityInfo->hasAttribute("ring"))
			_pos->setRing(static_cast<Ring>(entityInfo->getIntAttribute("ring")));
		else			
			//situación anómala, se lanzaría una excepción o trazas por consola. Se le asigna el anillo central para que 
			//pese a todo no pete.
			_pos->setRing(Logic::LogicalPosition::CENTRAL_RING);  

		// UNDONE ƒ®§ Este height ya se inicializa a 0 en el ctor por defecto de TLogicalPosition
		//_pos->_height = 0;

		// UNDONE ƒ®§: Esta información de física es necesaria para alguien más?
		//if(entityInfo->hasAttribute("angularBox"))					
		//	_angularBox = entityInfo->getFloatAttribute("angularBox");

		if (_logicInput)
		{
			position=fromLogicalToCartesian(_pos->getDegree(),_pos->getHeight(), _pos->getBase(),_pos->getRing());
			_transform.setTrans(position);
			
			setYaw(Math::fromDegreesToRadians(-_pos->getDegree()));
			
			//  UNDONE FRS: Con el modelo spartan.mesh bien orientado este HACK ya no es necesario
			if (_pos->getSense()==LogicalPosition::RIGHT)
			{
				if (_type=="Medusa" || _type=="Spider" || _type=="Sinbad"|| _type=="Cancerbero")
					this->setYaw(Math::fromDegreesToRadians(360-_pos->getDegree()+180));			
			}
			else
				if (_type=="Medusa"|| _type=="Spider"|| _type=="Sinbad"|| _type=="Cancerbero")
					this->setYaw(Math::fromDegreesToRadians(360-_pos->getDegree()+180));
				
		
		}
		else //logicInput=false
		{
			position = CServer::getSingletonPtr()->getRingPosition(_pos->getRing());						
			_transform.setTrans(position);
		}

		if(entityInfo->hasAttribute("position"))
		{
			position = entityInfo->getVector3Attribute("position");
			_transform.setTrans(position);
		}

		if(entityInfo->hasAttribute("isPlayer"))
			setIsPlayer( entityInfo->getBoolAttribute("isPlayer") );		

		// Inicializamos los componentes
		bool correct = true;
		TComponentMap::const_iterator it; // TODO FRS acceso secuencial mejor con vector TComponentList::const_iterator it;		
			for( it = _components.begin(); it != _components.end() && correct; ++it )
				//PT. comento todo esto, y solo dejo la linea que se necesita
				
				//if (this->getEntityID() == 46)
			 //       correct = it->second->spawn(this,map,entityInfo) && correct;
				//else
				//	correct = it->second->spawn(this,map,entityInfo) && correct;
				//// correct = (*it)->spawn(this,map,entityInfo) && correct;

				correct = it->second->spawn(this,map,entityInfo) && correct;

		return correct;

	} // spawn

	//---------------------------------------------------------

	bool CEntity::activate() 
	{
		if ( this->getType() == "Camera" )
		{
			//CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getCameraController()->setControlledCamera(this);
		}

		if(_isPlayer) {
			CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(this);		
		}

		// Activamos los componentes
		TComponentMap::const_iterator it;
		// TODO TComponentList::const_iterator it;

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;
			for( it = _components.begin(); it != _components.end(); ++it )
				_activated = it->second->activate() && _activated;
				//_activated = (*it)->activate() && _activated;

		return _activated;

	} // activate

	//---------------------------------------------------------

	void CEntity::deactivate() 
	{
		// Si éramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		//if (isPlayer())
		//	setIsPlayer(false);
		if ( this->getType() == "Camera" )
		{
			//CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getCameraController()->removeControlledCamera(this);
		}

		TComponentMap::const_iterator it; // TODO TComponentList::const_iterator it;

		// Desactivamos los componentes
		for( it = _components.begin(); it != _components.end(); ++it )
			it->second->deactivate();
			// TODO (*it)->deactivate();

		_activated = false;

	} // deactivate

	//---------------------------------------------------------

	void CEntity::setIsPlayer(bool isPlayer) 
	{ 		
		if(isPlayer == _isPlayer)
			return;

		_isPlayer = isPlayer; 
		if(_isPlayer) {
			CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(this);
			GUI::CServer::getSingletonPtr()->getHudController()->setControlledHud(this); //PT
			GUI::CServer::getSingletonPtr()->getShopController()->setControlledShop(this); //PT
		} else { //if not player. no HUD, no SHOP
			if(CServer::getSingletonPtr()->getPlayer() == this)
				CServer::getSingletonPtr()->setPlayer(0);
			if(GUI::CServer::getSingletonPtr()->getPlayerController()->getControlledAvatar() == this)
				GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(0);
		}
	} // setIsPlayer

	//---------------------------------------------------------

	 const Vector3 CEntity::fromLogicalToCartesian(const float grados, const float altura, const unsigned short base, const Logic::LogicalPosition::Ring ring)
	 {		 
		float offset=0;// se trata de un offset de radio, no de altura

		if (this->getType().compare("Altar")==0)
		{
			offset=-12;
		}

		return (Math::fromCylindricalToCartesian( grados, CServer::getSingletonPtr()->getRingRadio(ring) + offset, CServer::getSingletonPtr()->getRingPosition(ring).y + altura + _offsetHeight));
	 
	 }

	
	//---------------------------------------------------------
	
	 const float CEntity::getY(const unsigned short base, const Logic::Ring ring)
	{ 	
		Vector3 position = Vector3::ZERO;
		position = CServer::getSingletonPtr()->getRingPosition(ring);	
		return position.y;
	}

	//---------------------------------------------------------
	
	 void CEntity::tick(unsigned int msecs) 
	{
		TComponentMap::const_iterator it;
		//TODO TComponentList::const_iterator it;
		for( it = _components.begin(); it != _components.end(); ++it )
			if ( it->second->isAwake() )
				it->second->tick(msecs);
			// TODO (*it)->tick(msecs);

	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component)
	{
		_components[component->getComponentId()] = component;
		component->setEntity(this);
	} // addComponent

	//---------------------------------------------------------

	// TODO FRS Búsqueda por [getComponentID()] que pa eso tenemos maps amoshombreiya!
	bool CEntity::removeComponent(IComponent* component)
	{
		TComponentMap::const_iterator it = _components.begin();

		bool removed = false;
		// Buscamos el componente hasta el final, por si aparecía
		// más de una vez... (no tendría mucho sentido, pero por si
		// acaso).
		while (it != _components.end()) 
		{
			if (it->second == component)
			{
				it = _components.erase(it);
				removed = true;
			}
			else
				++it;
		}
		if (removed)
			component->setEntity(0);
		return removed;

	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents()
	{		
		TComponentMap::const_iterator it = _components.begin();
		while (it != _components.end()) {
		     delete it->second;
			_components.erase(it++);			
		}		
		// TODO FRS si compaginamos vector + map (busquedas) esto seria mejor secuencialmente
	//	while(!_components.empty())
	//	{
	//		c = _components.back();
	//		_components.pop_back();
	//		delete c;
	//	}

	} // destroyAllComponents

	//---------------------------------------------------------

	bool CEntity::emitMessage(CMessage *message, IComponent* emitter)
	{
		if(!_activated) // HACK Si la entidad no está activa, no recibe mensajes
			return false;

		// Interceptamos los mensajes que además de al resto de los
		// componentes, interesan a la propia entidad.
		switch(message->getType())
		{
		case Message::SET_TRANSFORM:
			CMessageTF *maux = static_cast<CMessageTF*>(message);
			_transform = maux->getTransform();
		}

		TComponentMap::const_iterator it;
		// TODO TComponentList::const_iterator it;

		message->grab();
		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		for( it = _components.begin(); it != _components.end(); ++it )
		{
			// Al emisor no se le envia el mensaje.
			if( emitter != it->second )
				anyReceiver = it->second->set(message) || anyReceiver;
		// TODO	if( emitter != (*it) )
		//		anyReceiver = (*it)->set(message) || anyReceiver;
		}

		message->release();
		return anyReceiver;

	} // emitMessage

	//---------------------------------------------------------

	void CEntity::setTransform(const Matrix4& transform) 
	{
		_transform = transform;

		// Avisamos a los componentes del cambio.
		CMessageTF *message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		message->setTransform(_transform);
		//message->setBool(true); // [ƒ®§] Interesante si sólo queremos comprobar colisiones en cambios de posición y no en todos los SET_TRANSFORM
		emitMessage(message);

	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position) 
	{
		_transform.setTrans(position);

		// Avisamos a los componentes del cambio.
		CMessageTF *message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		message->setTransform(_transform);
		//message->setBool(true); // [ƒ®§] Interesante si sólo queremos comprobar colisiones en cambios de posición y no en todos los SET_TRANSFORM
		emitMessage(message);

	} // setPosition

	//---------------------------------------------------------

	void CEntity::setLogicalPosition(const Logic::CLogicalPosition *pos)
	{
		_pos->setBase(pos->getBase());
		_pos->setDegree(pos->getDegree());
		_pos->setRing(pos->getRing());
		_pos->setSense(pos->getSense());
		_pos->setHeight(pos->getHeight());

		setPosition(
			fromLogicalToCartesian(
				_pos->getDegree(),
				_pos->getHeight(),
				_pos->getBase(),
				_pos->getRing()
			)
		);

	} //setLogicalPosition
		
	//---------------------------------------------------------

	void CEntity::setOrientation(const Matrix3& orientation) 
	{
		_transform = orientation;
		
		// Avisamos a los componentes del cambio.
		CMessageTF *message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		message->setTransform(_transform);
		emitMessage(message);

	} // setOrientation

	//---------------------------------------------------------

	Matrix3 CEntity::getOrientation() const
	{
		Matrix3 orientation;
		_transform.extract3x3Matrix(orientation);
		return orientation;

	} // getOrientation

	//---------------------------------------------------------

	void CEntity::setYaw(float yaw) 
	{
		Math::setYaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // setYaw

	//---------------------------------------------------------

	void CEntity::setRoll(float roll) 
	{
		Math::setRoll(roll,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // setRoll

	//---------------------------------------------------------

	void CEntity::setPitch(float pitch) 
	{
		Math::setPitch(pitch,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // setPitch

	//---------------------------------------------------------

	void CEntity::setPitchYaw(float pitch,float yaw) 
	{
		Math::setPitchYaw(pitch,yaw,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // setPitchYaw

	//---------------------------------------------------------

	void CEntity::yaw(float yaw) 
	{
		Math::yaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // yaw

	//---------------------------------------------------------

	void CEntity::roll(float roll) 
	{
		Math::roll(roll,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // roll

	//---------------------------------------------------------

	void CEntity::pitch(float pitch)
	{
		Math::pitch(pitch,_transform);

		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	

	} // pitch

	//---------------------------------------------------------

	void CEntity::detachFromMap()
	{
		_map->removeEntity(this);

		TComponentMap::const_iterator it = _components.begin();
		TComponentMap::const_iterator end = _components.end();
		for (; it != end; ++it) {
		     it->second->detachFromMap();		
		}		
	}

	//---------------------------------------------------------

	void CEntity::attachToMap(CMap *map)
	{
		map->addEntity(this);

		TComponentMap::const_iterator it = _components.begin();
		TComponentMap::const_iterator end = _components.end();
		for (; it != end; ++it) {
		     it->second->attachToMap(map);		
		}		
	}
} // namespace Logic
