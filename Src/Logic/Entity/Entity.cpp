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

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageTF.h"
#include "Logic/Entity/Messages/MessageBoolTF.h"




namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
				_map(0), _type(""), _name(""), _transform(Matrix4::IDENTITY),
				_isPlayer(false), _activated(false), _pos(TLogicalPosition())
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
		_entityInfo=entityInfo; // TODO [ƒ®§] Esto para qué es necesario?
		_type = entityInfo->getType();
		_logicInput=false;
		Vector3 posicion=Vector3::ZERO;	
		
		if(entityInfo->hasAttribute("name"))
			_name = entityInfo->getStringAttribute("name");		

		if(entityInfo->hasAttribute("logicInput"))
			_logicInput = entityInfo->getBoolAttribute("logicInput");


		if (_logicInput)
		{
			if(entityInfo->hasAttribute("degrees"))
				_pos._degrees = entityInfo->getFloatAttribute("degrees");

			//if(entityInfo->hasAttribute("radio")) //LO podremos poner en "ring" en el futuro y ahorrarnoslo
				//_pos._radio = entityInfo->getFloatAttribute("radio");
			
			if(entityInfo->hasAttribute("sense"))
				switch (entityInfo->getIntAttribute("sense"))
				{
					case Logic::LogicalPosition::IZQUIERDA:
					{
						_pos._sense = Logic::LogicalPosition::IZQUIERDA;						
						break;
					}
					case Logic::LogicalPosition::DERECHA:
					{
						_pos._sense = Logic::LogicalPosition::DERECHA;
						break;
					}
					default:
						{
						_pos._sense= Logic::LogicalPosition::IZQUIERDA;
						//situación anómala, se lanzaría una excepción o trazas por consola. Se le asigna por defecto dirección izquierda
						//pese a todo no pete.
						}
			}

			if(entityInfo->hasAttribute("base"))					
				_pos._base = entityInfo->getIntAttribute("base");
			// ahora empezamos a hacer la composicion de la posición, calculamos x, z
			Vector3 posicion=Vector3::ZERO;
				//Math::fromPolarToCartesian(_pos._degrees,_pos._radio);

			if(entityInfo->hasAttribute("ring"))
			{
				switch (entityInfo->getIntAttribute("ring"))
				{
					case Logic::LogicalPosition::ANILLO_INFERIOR:
					{
						_pos._ring = Logic::LogicalPosition::ANILLO_INFERIOR;
						break;
					}
					case Logic::LogicalPosition::ANILLO_CENTRAL:
					{
						_pos._ring = Logic::LogicalPosition::ANILLO_CENTRAL;				
						break;
					}
					case Logic::LogicalPosition::ANILLO_SUPERIOR:
					{
						_pos._ring = Logic::LogicalPosition::ANILLO_SUPERIOR;
						break;
					}
					default:
						{
						_pos._ring= Logic::LogicalPosition::ANILLO_CENTRAL;  // TODO [ƒ®§] Esto de mezclar spanglish no queda muy fino, va a haber que normalizar todo al inglés...
						//situación anómala, se lanzaría una excepción o trazas por consola. Se le asigna el anillo central para que 
						//pese a todo no pete.
						}
			}
			//	posicion=CServer::getSingletonPtr()->getRingPositions(_pos._base,_pos._ring);
				posicion=this->fromLogicalToCartesian(_pos._degrees,_pos._base,_pos._ring);
			}




			if(entityInfo->hasAttribute("angularBox"))					
				_angularBox = entityInfo->getFloatAttribute("angularBox");


			if(entityInfo->hasAttribute("sense"))
				switch (entityInfo->getIntAttribute("sense"))
				{//	_pos._sense = entityInfo->getIntAttribute("sense");		
				case Logic::LogicalPosition::IZQUIERDA:
					{
						_pos._sense = Logic::LogicalPosition::IZQUIERDA;
						float yaw = Math::fromDegreesToRadians(90);
						Math::yaw(yaw,_transform);
						break;
					}
					case Logic::LogicalPosition::DERECHA:
					{
						_pos._sense = Logic::LogicalPosition::DERECHA;
						float yaw = Math::fromDegreesToRadians(0);
						Math::yaw(yaw,_transform);
						break;
					}
					default:
						{
						_pos._sense= Logic::LogicalPosition::IZQUIERDA;
						float yaw = Math::fromDegreesToRadians(90);
						Math::yaw(yaw,_transform);
						//situación anómala, se lanzaría una excepción o trazas por consola. Se le asigna el sentido izquierda 					
						}
			}		
			
			//aplicamos la transformación, las coordenadas
			_transform.setTrans(posicion);

			}
		else//logicInput=false
			{
					Vector3 position =Vector3::ZERO;
					if (this->getType().compare("World")==0)
					{			
						if(entityInfo->hasAttribute("base"))					
							_pos._base = entityInfo->getIntAttribute("base");
						if(entityInfo->hasAttribute("ring"))
						{
							switch (entityInfo->getIntAttribute("ring"))
							{
								case Logic::LogicalPosition::ANILLO_INFERIOR:
								{
									_pos._ring = Logic::LogicalPosition::ANILLO_INFERIOR;									
									break;
								}
								case Logic::LogicalPosition::ANILLO_CENTRAL:
								{
									_pos._ring = Logic::LogicalPosition::ANILLO_CENTRAL;
									break;
								}
								case Logic::LogicalPosition::ANILLO_SUPERIOR:
								{
									_pos._ring = Logic::LogicalPosition::ANILLO_SUPERIOR;
									break;
								}
								default:
									{
									_pos._ring= Logic::LogicalPosition::ANILLO_CENTRAL;
									//situación anómala, se lanzaría una excepción o trazas por consola. Se le asigna el anillo central para que 
									//pese a todo no pete.
									}								
							}

						}
						position=CServer::getSingletonPtr()->getRingPositions(_pos._base,_pos._ring);						
						_transform.setTrans(position);
					}
					else
					{
						if(entityInfo->hasAttribute("position"))
						{
							Vector3 position = entityInfo->getVector3Attribute("position");
							_transform.setTrans(position);
						}
					}				
			}
		
		// TODO limpiar comentarios, por seguridad ya estamos usando git 
		/* arreglamos la orientación */
		/*Vector3 centro=Vector3(0,-125,0);
		Vector3 vectorCentroEntidad = -(centro-_transform.getTrans());
		vectorCentroEntidad.normalise();
		Vector3 actualDirection=Math::getDirection(this->getYaw());
		Vector3 directionPerp= Vector3::UNIT_Y.crossProduct(vectorCentroEntidad);
		directionPerp.normalise();
		Quaternion rotacionDestino=actualDirection.getRotationTo(directionPerp);
		*/
	//	Matrix4 orientacion = this->getOrientation();
		if (this->getType().compare("AnimatedEntity")==0)
		{
			//_entity->setDegree(_entity->getDegree()-_angularSpeed); 
			if (this->getSense()==LogicalPosition::DERECHA)
				this->setYaw(-Math::fromDegreesToRadians(this->getDegree()));
			else
				this->setYaw(Math::fromDegreesToRadians(360-this->getDegree()+180));
			
		}
		
		if (this->getType().compare("Player")==0)
			this->yaw(Math::PI);
		
		//	this->setOrientation(rotacionDestino);
		//this->setYaw(Math::fromDegreesToRadians(-90));
		//this->setYaw(rotacionDestino);
		// Por comodidad en el mapa escribimos los ángulos en grados.
		if(entityInfo->hasAttribute("orientation"))
		{
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation"));
			Math::yaw(yaw,_transform); // HACK revisar
		}

		if(entityInfo->hasAttribute("isPlayer"))
			setIsPlayer( entityInfo->getBoolAttribute("isPlayer") );		

		// Inicializamos los componentes
		TComponentList::const_iterator it;

		bool correct = true;

		for( it = _components.begin(); it != _components.end() && correct; ++it )
			correct = (*it)->spawn(this,map,entityInfo) && correct;

		return correct;

	} // spawn

	//---------------------------------------------------------

	bool CEntity::activate() 
	{
		if (this->getType().compare("Camera")==0) // TODO [ƒ®§] El compare es más eficiente que el == "Camera"?
		{
			//CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getCameraController()->setControlledCamera(this);
			int i=0;
		}

		// Activamos los componentes
		TComponentList::const_iterator it;

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;
			for( it = _components.begin(); it != _components.end(); ++it )
				_activated = (*it)->activate() && _activated;
		return _activated;

	} // activate

	//---------------------------------------------------------

	void CEntity::deactivate() 
	{
		// Si éramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		if (isPlayer())
			setIsPlayer(false);

		TComponentList::const_iterator it;

		// Desactivamos los componentes
		for( it = _components.begin(); it != _components.end(); ++it )
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
	 const Vector3 CEntity::fromLogicalToCartesian(const float grados, const unsigned short base, const Logic::LogicalPosition::Ring ring)
	 {		 
		float offset=0;

		if (this->getType().compare("Altar")==0){
			offset=-9;
		}

		Vector3 resultado=Vector3::ZERO;
		resultado=Math::fromPolarToCartesian(grados, CServer::getSingletonPtr()->getRingRadio(base,ring)+offset);
		resultado.y=CServer::getSingletonPtr()->getRingPositions(base,ring).y+126;

		return resultado;
	 }
	 
	  const float CEntity::getY(const unsigned short base, const Logic::LogicalPosition::Ring ring)
	  { 	
		  Vector3 position=Vector3::ZERO;
		  position=CServer::getSingletonPtr()->getRingPositions(base,ring);	
		  return position.y;
	  }

	  const float CEntity::getYJump(const unsigned short base, const Logic::LogicalPosition::Ring ring)
	  { 	
		  Vector3 position=Vector3::ZERO;
		  position=CServer::getSingletonPtr()->getRingPositions(base,ring);	
		  return position.y+5;
	  }

	void CEntity::tick(unsigned int msecs) 
	{
		TComponentList::const_iterator it;

		for( it = _components.begin(); it != _components.end(); ++it )
			(*it)->tick(msecs);

	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component)
	{
		_components.push_back(component);
		component->setEntity(this);

	} // addComponent

	//---------------------------------------------------------

	bool CEntity::removeComponent(IComponent* component)
	{
		TComponentList::const_iterator it = _components.begin();

		bool removed = false;
		// Buscamos el componente hasta el final, por si aparecía
		// más de una vez... (no tendría mucho sentido, pero por si
		// acaso).
		while (it != _components.end()) 
		{
			if (*it == component)
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
		IComponent* c;
		while(!_components.empty())
		{
			c = _components.back();
			_components.pop_back();
			delete c;
		}

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

		TComponentList::const_iterator it;

		message->grab();
		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		for( it = _components.begin(); it != _components.end(); ++it )
		{
			// Al emisor no se le envia el mensaje.
			if( emitter != (*it) )
				anyReceiver = (*it)->set(message) || anyReceiver;
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


	void CEntity::setLogicalPosition(const Logic::TLogicalPosition &pos)
	{
		_pos._base=pos._base;
		_pos._degrees=pos._degrees;
		_pos._ring=pos._ring;
		_pos._sense=pos._sense;
		
		const Vector3 position=fromLogicalToCartesian(_pos._degrees,_pos._base,_pos._ring);
		setPosition(position);


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


	void CEntity::setOrientation(const Quaternion &quat)
	{
		_quat=quat;
		// Avisamos a los componentes del cambio.
		CMessageTF* message = new CMessageTF();
		message->setType(Message::SET_TRANSFORM);
		//message._quat = _quat;
		message->setTransform(_transform);
		emitMessage(message);	
	}
	

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

	void CEntity::setDegree(const float &degree)
	{
		_pos._degrees=degree;
	}

	void CEntity::setSense(const LogicalPosition::Sense &sense)
	{
		_pos._sense=sense;
	}
	
	void CEntity::setRing(const LogicalPosition::Ring &ring)
	{
		_pos._ring=ring;
	}
	const float CEntity::getRadio()
	{
		
		return CServer::getSingletonPtr()->getRingRadio(_pos._base,_pos._ring);
	}

	/*void CEntity::setIsJumping(bool &isJumpingPointer)
	{		
		_isJumping=&isJumpingPointer;
	}*/
} // namespace Logic
