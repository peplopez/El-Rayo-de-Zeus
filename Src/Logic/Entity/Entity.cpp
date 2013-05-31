/**
@file Entity.cpp

Contiene la implementaci�n de la clase Entity, que representa una entidad
de juego. Es una colecci�n de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llans�
@date Julio, 2010
*/

#include "Entity.h"

#include <assert.h>

#include <GUI/Server.h>
#include <GUI/PlayerController.h>
#include <GUI/CameraController.h>

#include <Logic/Entity/Messages/Message.h>
#include <Logic/Entity/Messages/MessageTF.h>
#include <Logic/Entity/Messages/MessageBoolTF.h>
#include <Logic/Maps/Map.h>
#include <Logic/Server.h>

#include <Map/MapEntity.h>

// Componentes
#include "Component.h"



namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
				_map(0), _type(""), _name(""), _transform(Matrix4::IDENTITY),
				_isPlayer(false), _activated(false) // UNDONE ��� _pos(TLogicalPosition()) ya llama al ctro por defecto, por defecto, valga la rebuznancia
	{

	} // CEntity
	
	//---------------------------------------------------------

	CEntity::~CEntity()
	{
		assert(!_map && "��Antes de destruir la entidad debe desacoplarse del mapa!!");

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
			
		if(entityInfo->hasAttribute("sense"))
			_pos->setSense(static_cast<Logic::Sense>(entityInfo->getIntAttribute("sense")));
		else
			//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna por defecto direcci�n LEFT
			_pos->setSense(Logic::LogicalPosition::LEFT);

		if(entityInfo->hasAttribute("initialMaterial"))
			_initialMaterial = entityInfo->getStringAttribute("initialMaterial");	
		else
			_initialMaterial = "marine";	
		
		_initialScale= 1.0f;	
		if(entityInfo->hasAttribute("initialScale"))
			_initialScale = entityInfo->getFloatAttribute("initialScale");	
		
		if(entityInfo->hasAttribute("offsetHeight"))					
		{
			_offsetHeight=entityInfo->getIntAttribute("offsetHeight");
		}

		if(entityInfo->hasAttribute("base"))					
		{
			_pos->setBase(entityInfo->getIntAttribute("base"));
			this->setOriginBase(_pos->getBase());
		}
		if(entityInfo->hasAttribute("ring"))
			_pos->setRing(static_cast<Ring>(entityInfo->getIntAttribute("ring")));
		else			
			//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna el anillo central para que 
			//pese a todo no pete.
			_pos->setRing(Logic::LogicalPosition::CENTRAL_RING);  

		// UNDONE ��� Este height ya se inicializa a 0 en el ctor por defecto de TLogicalPosition
		//_pos->_height = 0;

		// UNDONE ���: Esta informaci�n de f�sica es necesaria para alguien m�s?
		//if(entityInfo->hasAttribute("angularBox"))					
		//	_angularBox = entityInfo->getFloatAttribute("angularBox");

		if (_logicInput)
		{
			position=fromLogicalToCartesian(_pos->getDegree(),_pos->getHeight(), _pos->getBase(),_pos->getRing());
			_transform.setTrans(position);
			
			setYaw(Math::fromDegreesToRadians(-_pos->getDegree()));
			
			//  UNDONE FRS: Con el modelo spartan.mesh bien orientado este HACK ya no es necesario
			//if (_pos->getSense()==LogicalPosition::RIGHT)
			//	{
			//	if (_type=="Player" || _type=="OtherPlayer")
			//		this->setYaw(-Math::fromDegreesToRadians(_pos->getDegree()));
			//	else
			//		this->setYaw(Math::fromDegreesToRadians(360-_pos->getDegree()+180));		
			//	}
			//else
			//	if (_type=="Player" || _type=="OtherPlayer")				
			//		//this->setYaw(Math::fromDegreesToRadians(360-_pos->getDegree()+180));
			//		this->setYaw(-Math::fromDegreesToRadians(_pos->getDegree()));
			//	else
			//		//this->setYaw(-Math::fromDegreesToRadians(_pos->getDegree()));
			//	this->setYaw(Math::fromDegreesToRadians(360-_pos->getDegree()+180));
		
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
		TComponentVector::const_iterator it = _compoList.begin(); 	
		TComponentVector::const_iterator end = _compoList.end(); 
			for( ; it != end && correct; ++it )				
				correct = (*it)->spawn(this,map,entityInfo) && correct;

		return correct;
	} // spawn

	//---------------------------------------------------------

	bool CEntity::activate() 
	{
		if ( this->getType() == "Camera" )				
			GUI::CServer::getSingletonPtr()->getCameraController()->setControlledCamera(this);
		
		if (isPlayer())
			setIsPlayer(true);

		// ACTIVACI�N
		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;
		TComponentVector::const_iterator it = _compoList.begin(); 	
		TComponentVector::const_iterator end = _compoList.end(); 
			for(; it != end; ++it )
				_activated = (*it)->activate() && _activated;

		return _activated;
	} // activate

	//---------------------------------------------------------

	void CEntity::deactivate() 
	{
		// Si �ramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		//if (isPlayer())
		//	setIsPlayer(false);

		// Desactivamos los componentes
		TComponentVector::const_iterator it = _compoList.begin(); 	
		TComponentVector::const_iterator end = _compoList.end(); 
			for(; it != end; ++it )
				(*it)->deactivate();

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
		} else {
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

		if (_type == "Altar")		
			offset=-12;

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
		TComponentVector::const_iterator it = _compoList.begin(); 	
		TComponentVector::const_iterator end = _compoList.end(); 
			for(; it != end; ++it )
				if ( (*it)->isAwake() )
					(*it)->tick(msecs);
	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component)
	{
		assert( !_compoTable[component->getComponentId()] && "Componente cargado por segunda vez en la misma entidad!");
		_compoTable[component->getComponentId()] = component;
		_compoList.push_back(component);	
		component->setEntity(this);
	} // addComponent

	//---------------------------------------------------------

	
	bool CEntity::removeComponent(IComponent* component)
	{	
		if( !_compoTable[component->getComponentId()] )
			return false;

		_compoTable.erase(component->getComponentId()); // erase by key
		_compoList.erase( std::remove(_compoList.begin(), _compoList.end(), component), _compoList.end() ); // TODO FRS Revisar
		/* FRS std::remove doesn't actually erase the element from the container, 
		but it does return the new end iterator which can be passed to container_type::erase to do the REAL 
		removal of the extra elements that are now at the end of the container
		Because vectors use an array as their underlying storage, erasing elements in positions
		other than the vector end causes the container to relocate all the elements after the segment erased
		to their new positions. This is generally an inefficient operation compared to the one performed for the 
		same operation by other kinds of sequence containers (such as list or forward_list).
			FRS Asumimos que en pocas ocasiones se ejecutar� esta operaci�n 
		*/	

		component->setEntity(0);

		return true;
	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents()
	{		
		TComponentVector::const_iterator it = _compoList.begin(); 	
		TComponentVector::const_iterator end = _compoList.end(); 
			for(; it != end; ++it ) 
				 delete (*it);

		_compoList.clear();
		_compoTable.clear();		
	} // destroyAllComponents

	//---------------------------------------------------------

	bool CEntity::emitMessage(CMessage *message, IComponent* emitter)
	{
		if(!_activated) // HACK Si la entidad no est� activa, no recibe mensajes
			return false;

		// Interceptamos los mensajes que adem�s de al resto de los
		// componentes, interesan a la propia entidad.
		switch(message->getType())
		{
		case Message::SET_TRANSFORM:
			CMessageTF *maux = static_cast<CMessageTF*>(message);
			_transform = maux->getTransform();
		}
		
		message->grab();

		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		TComponentVector::const_iterator it = _compoList.begin(); 	
		TComponentVector::const_iterator end = _compoList.end(); 
			for(; it != end; ++it ) // Al emisor no se le envia el mensaje.				
				if(*it != emitter)
					anyReceiver = (*it)->set(message) || anyReceiver;			

		message->release();

		return anyReceiver;
	} // emitMessage

	//---------------------------------------------------------

	void CEntity::detachFromMap()
	{
		_map->removeEntity(this);

		TComponentVector::const_iterator it = _compoList.begin(); 	
		TComponentVector::const_iterator end = _compoList.end(); 
		for (; it != end; ++it) {
		     (*it)->detachFromMap();		
		}		
	}

	//---------------------------------------------------------

	void CEntity::attachToMap(CMap *map)
	{
		map->addEntity(this);

		TComponentVector::const_iterator it = _compoList.begin(); 	
		TComponentVector::const_iterator end = _compoList.end(); 
		for (; it != end; ++it) {
		     (*it)->attachToMap(map);		
		}		
	}
	


	/*********************
		GET's & SET's
	********************/

	void CEntity::setTransform(const Matrix4& transform) 
	{
		_transform = transform;

		// Avisamos a los componentes del cambio.
		CMessageTF *message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		message->setTransform(_transform);
		//message->setBool(true); // [���] Interesante si s�lo queremos comprobar colisiones en cambios de posici�n y no en todos los SET_TRANSFORM
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
		//message->setBool(true); // [���] Interesante si s�lo queremos comprobar colisiones en cambios de posici�n y no en todos los SET_TRANSFORM
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


} // namespace Logic
