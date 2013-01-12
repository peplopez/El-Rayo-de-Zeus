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

// Componentes
#include "Component.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
				_map(0), _type(""), _name(""), _transform(Matrix4::IDENTITY),
				_isPlayer(false), _activated(false)
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
		_entityInfo=entityInfo;
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

			if(entityInfo->hasAttribute("radio")) //LO podremos poner en "ring" en el futuro y ahorrarnoslo
				_pos._radio = entityInfo->getFloatAttribute("radio");
			
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
						//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna por defecto direcci�n izquierda
						//pese a todo no pete.
						}
			}

			// ahora empezamos a hacer la composicion de la posici�n, calculamos x, z
			Vector3 posicion=Math::fromPolarToCartesian(_pos._degrees,_pos._radio);

			if(entityInfo->hasAttribute("ring"))
			{
				switch (entityInfo->getIntAttribute("ring"))
				{
					case Logic::LogicalPosition::ANILLO_INFERIOR:
					{
						_pos._ring = Logic::LogicalPosition::ANILLO_INFERIOR;
						posicion.y=-50; // sustituir por una constante
						break;
					}
					case Logic::LogicalPosition::ANILLO_CENTRAL:
					{
						_pos._ring = Logic::LogicalPosition::ANILLO_CENTRAL;
						posicion.y=0; // sustituir por una constante
						break;
					}
					case Logic::LogicalPosition::ANILLO_SUPERIOR:
					{
						_pos._ring = Logic::LogicalPosition::ANILLO_SUPERIOR;
						posicion.y=50; // sustituir por una constante
						break;
					}
					default:
						{
						_pos._ring= Logic::LogicalPosition::ANILLO_CENTRAL;
						posicion.y=0;
						//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna el anillo central para que 
						//pese a todo no pete.
						}
			}

			}


			if(entityInfo->hasAttribute("base"))					
				_pos._base = entityInfo->getIntAttribute("base");

			if(entityInfo->hasAttribute("angularBox"))					
				_pos._angularBox = entityInfo->getFloatAttribute("angularBox");


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
						//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna el sentido izquierda 					
						}
			}		
			
			//aplicamos la transformaci�n, las coordenadas
			_transform.setTrans(posicion);

			}
		else
			{
			if(entityInfo->hasAttribute("position"))
				{
					Vector3 position = entityInfo->getVector3Attribute("position");
					_transform.setTrans(position);
				}
		}
		/* arreglamos la orientaci�n */
		Vector3 centro=Vector3(0,-125,0);
		Vector3 vectorCentroEntidad = -(centro-_transform.getTrans());
		vectorCentroEntidad.normalise();
		Vector3 actualDirection=Math::getDirection(this->getYaw());
		Vector3 directionPerp= Vector3::UNIT_Y.crossProduct(vectorCentroEntidad);
		directionPerp.normalise();
		Quaternion rotacionDestino=actualDirection.getRotationTo(directionPerp);

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
		// Por comodidad en el mapa escribimos los �ngulos en grados.
		if(entityInfo->hasAttribute("orientation"))
		{
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation"));
			Math::yaw(yaw,_transform); //revisar
		}

		if(entityInfo->hasAttribute("isPlayer"))
			_isPlayer = entityInfo->getBoolAttribute("isPlayer");
		

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
		// Si somos jugador, se lo decimos al servidor
		// y nos registramos para que nos informen
		// de los movimientos que debemos realizar
		if (isPlayer())
		{
			CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(this);
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
		// Si �ramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		if (isPlayer())
		{
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(0);
			CServer::getSingletonPtr()->setPlayer(0);
		}


		TComponentList::const_iterator it;

		// Desactivamos los componentes
		for( it = _components.begin(); it != _components.end(); ++it )
			(*it)->deactivate();

		_activated = false;

	} // deactivate

	//---------------------------------------------------------

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
		// Buscamos el componente hasta el final, por si aparec�a
		// m�s de una vez... (no tendr�a mucho sentido, pero por si
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

	const bool CEntity::emitMessage (const TMessage &message, IComponent* emitter)
	{
		// Interceptamos los mensajes que adem�s de al resto de los
		// componentes, interesan a la propia entidad.
		switch(message._type)
		{
		case Message::SET_TRANSFORM:
			_transform = message._transform;
		}

		TComponentList::const_iterator it;
		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		for( it = _components.begin(); it != _components.end(); ++it )
		{
			// Al emisor no se le envia el mensaje.
			if( emitter != (*it) )
				anyReceiver = (*it)->set(message) || anyReceiver;
		}
		return anyReceiver;

	} // emitMessage

	//---------------------------------------------------------

	void CEntity::setTransform(const Matrix4& transform) 
	{
		_transform = transform;

		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position) 
	{
		_transform.setTrans(position);

		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // setPosition

	//---------------------------------------------------------

	void CEntity::setOrientation(const Matrix3& orientation) 
	{
		_transform = orientation;
		
		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // setOrientation


	void CEntity::setOrientation(const Quaternion &quat)
	{
		_quat=quat;
		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._quat = _quat;
		message._transform = _transform;
		emitMessage(message);	
	}
	

	//---------------------------------------------------------

	Matrix3 CEntity::getOrientation() 
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
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // setYaw

	//---------------------------------------------------------

		void CEntity::setRoll(float roll) 
	{
		Math::setRoll(roll,_transform);

		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // setYaw

		void CEntity::setPitch(float pitch) 
	{
		Math::setPitch(pitch,_transform);

		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // setYaw



		void CEntity::setPitchYaw(float pitch,float yaw) 
	{
		Math::setPitchYaw(pitch,yaw,_transform);

		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // setYaw

	void CEntity::yaw(float yaw) 
	{
		Math::yaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // yaw

	void CEntity::roll(float roll) 
	{
		Math::roll(roll,_transform);

		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // roll

	void CEntity::pitch(float pitch)
	{
		Math::pitch(pitch,_transform);

		// Avisamos a los componentes del cambio.
		TMessage message;
		message._type = Message::SET_TRANSFORM;
		message._transform = _transform;
		emitMessage(message);

	} // pitch

	void CEntity::setDegree(const float &degree)
	{
		_pos._degrees=degree;
	}

	void CEntity::setRadio(const float &radio)
	{
		_pos._radio=radio;
	}

	void CEntity::setSense(const LogicalPosition::Sense &sense)
	{
		_pos._sense=sense;
	}
	
	void CEntity::setRing(const LogicalPosition::Ring &ring)
	{
		_pos._ring=ring;
	}
	
	


	/*
	bool CEntity::contactoAngular(CEntity* entidad)
	{
		if (this==entidad)
			return false;
		if (this->getBase()!=entidad->getBase()) 
			return false;
		if (this->getRing()!=entidad->getRing()) 
			return false;
		if (!this->_logicInput || !entidad->_logicInput)
			return false;
		
		float angleE1=this->getDegree();
		float angleE2=entidad->getDegree();
		float angularBoxE1=this->getAngularBox();
		float angularBoxE2=entidad->getAngularBox();
		if (angularBoxE2==0)
			return false;
		float logicalCenterDistance=abs(angleE1-angleE2);//distancia entre los centros de las entidades
		if (logicalCenterDistance>180) //
			logicalCenterDistance=360-logicalCenterDistance;

		float angularBoxAmount=angularBoxE1+angularBoxE2;
		//if (this->getType().compare("AnimatedEntity")==0)
		//	int i=0;
		if (logicalCenterDistance<=angularBoxAmount) //si la distancia de los centros es menor que la suma de los radios hay contacto
		{
			
			return true;
		
		}
			return false;
	}

		void CEntity::Contacto()
	{
		Logic::TMessage m;
		//_Vida--; si tiene vida se le disminuye, si es un proyectil no tiene vida

		//if (_Vida<=0){	
			//_entity->getMap()->getScene()->removeEntity(_entity->_gentity);
			if (this->getType().compare("Player")==0)
			{
				
				m._string="walkBack";
				m._type = Logic::Message::CONTROL;
				m._bool=_sentidoColision;
				this->emitMessage(m,this);				
				
				_hit++;
				m._string="luminoso";
				m._type = Logic::Message::SET_SHADER;
				this->emitMessage(m,this);
			}		
			if (this->getType().compare("AnimatedEntity")==0)
			{
				m._string="walkBack";
				m._type = Logic::Message::NPC_CONTROL;
				m._bool=_sentidoColision;
				this->emitMessage(m,this);			

				_hit++;
				m._string="luminoso";
				m._type = Logic::Message::SET_SHADER;
				_entity->emitMessage(m,this);
			}
	}*/
} // namespace Logic
