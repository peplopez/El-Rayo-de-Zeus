	/**
@file LifeController.cpp

Contiene la declaración del componente que controla el movimiento 
angular de entidades.

@see Logic::CLifeController
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/
#include "LifeController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"


//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CLifeController);

	//CEntity *CLifeController::_BarraVida=0;

	bool CLifeController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// Pablo. 28-01-2013. Inicialización del atributo _life y _maxLife para aquellas
		// entidades que tienen el componente CLifeController
		if(entityInfo->hasAttribute("life"))
		{
			_maxLife = entityInfo->getFloatAttribute("life");
			_life = _maxLife;
		}

		if(entityInfo->hasAttribute("offsetLife"))
		{
			_offsetLife = entityInfo->getFloatAttribute("offsetLife");
		}

		if(entityInfo->hasAttribute("widthLife"))
		{
			_widthLife = entityInfo->getFloatAttribute("widthLife");
		}

		if(entityInfo->hasAttribute("heightLife"))
		{
			_heightLife = entityInfo->getFloatAttribute("heightLife");
		}


		//David LLanso. Tutoria.
		// crear el graphics::cbillboard y añadirle las dimensiones y ponerle las coordenadas
		//le paso un string y la posicion de la entidad.
		//_bb = new Graphics::CBillboard(entity->getName(),entity->getPosition());

		//le paso un string con el nombre de la entidad
		_bb = new Graphics::CBillboard(entity->getName(),_offsetLife); 
		
		_bb->setMaterial("lifeBar");
		//_bb->setDimensions(10,1);
		//Pablo. 01-02-2013- Ancho y alto de la barra de vida dirigido por datos
		_bb->setDimensions(_widthLife,_heightLife); 
		_bb->setCoordenadas(0.0f, 0.0f, 0.5f, 1.0f);
		//_bb->set

		return true;
	}

	bool CLifeController::activate()
	{
		if(_entity->getType().compare("BarraVida")==0)		
		{	
			//_entity->pitch(Math::PI);
				//_entity->setPitch(Math::PI/2);
		//	printf("Entidad nombre: %s",_BarraVida->getName());
		//	if (_BarraVida!=0){
				//_miBarra = CEntityFactory::getSingletonPtr()->instantiate(_entity);
				//_miBarra->setPosition(_entity->getPosition());
				//_miBarra->set
				//e->setPosition(Vector3(0,-126,0));
				//e->set
			//} 
		//	_entity->setOrientation(

		}		
			return true;
	}
		

	void CLifeController::deactivate()
	{
	}

	
		 bool CLifeController::accept(const TMessage &message)
		 {
			//return message._type == Message::CONTROL; 
			return message._type == Message::CONTACTO;
			//De momento, luego tendrá que aceptar de otras entidades NPC
			 return false;
		 }

		
		 void CLifeController::process(const TMessage &message)
		 {
			switch(message._type)
			{
			case Message::CONTACTO:
				if(!message._string.compare("updateLife"))
					updateLife(message._float);
			}

		 }

	 void CLifeController::tick(unsigned int msecs)
	 {
	 		IComponent::tick(msecs);
			//if (_miBarra!=0)
			//_miBarra->setPosition(_entity->getPosition());
			//if (_entity->getType().compare("BarraVida")==0)
			/*
			    _entity->setPosition(Logic::CServer::getSingletonPtr()->getPlayer()->getPosition());
				_entity->setPosition(Vector3(_entity->getPosition().x,_entity->getPosition().y+13,_entity->getPosition().z));
				
				_entity->setPitchYaw(Math::PI/2,Logic::CServer::getSingletonPtr()->getPlayer()->getYaw()-Math::PI/2);
				*/
				
				//_entity->get
			//	printf("player position: %d ",Logic::CServer::getSingletonPtr()->getPlayer()->getPosition().z);
				//CServer::getSingletonPtr()->get
			
			//_BarraVida->setPosition(_BarraVida->getPosition().);
		//Vector3 pos= _entity->getPosition();
	
		//_entity->setPosition(pos);
	 }


		void CLifeController::setLife(float life)
		{
			_life=life;
		}

		float CLifeController::getLife()
		{
			return _life;
		}

		void CLifeController::updateLife(float damage)
		{
				//float ratio = _energy / _maxEnergy;
				_life-=damage;

				float ratio = _life / _maxLife;
    
				if (ratio < 0.0f)
				{
					ratio = 0.0f;

					return;
				}
        
				float u1 = (1.0f - ratio) / 2.0f;
				float v1 = 0.0f;
				float u2 = 0.5f + (1.0f - ratio) / 2.0f;
				float v2 = 1.0f;
        
				_bb->setCoordenadas(u1, v1, u2, v2);
		}
	

} // namespace Logic