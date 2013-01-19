	/**
@file LifeController.cpp

Contiene la declaración del componente que controla el movimiento 
angular de entidades.

@see Logic::CLifeController
@see Logic::IComponent

@author José Luis López
@date Diciembre, 2012
*/
#include "Logic\Entity\Components\LifeController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic\Server.h"


//declaración de la clase
namespace Logic 
{
	IMP_FACTORY(CLifeController);

	CEntity *CLifeController::_BarraVida=0;

	bool CLifeController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

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
			//De momento, luego tendrá que aceptar de otras entidades NPC
			 return false;
		 }

		
		 void CLifeController::process(const TMessage &message)
		 {
		/*switch(message._type)
		{
		case Message::CONTROL:
			if(!message._string.compare("goUp"))
				goUp();
			else if(!message._string.compare("goDown"))
				goDown();
			else if(!message._string.compare("walkLeft"))
				walkLeft();
			else if(!message._string.compare("walkRight"))
				walkRight();
			else if(!message._string.compare("walkStop"))
				stopMovement();
			else if(!message._string.compare("turn"))
				turn(message._float);
		}*/

		 }

	 void CLifeController::tick(unsigned int msecs)
	 {
	 		IComponent::tick(msecs);
			//if (_miBarra!=0)
			//_miBarra->setPosition(_entity->getPosition());
			//if (_entity->getType().compare("BarraVida")==0)
			    _entity->setPosition(Logic::CServer::getSingletonPtr()->getPlayer()->getPosition());
				_entity->setPosition(Vector3(_entity->getPosition().x,_entity->getPosition().y+13,_entity->getPosition().z));
				
				_entity->setPitchYaw(Math::PI/2,Logic::CServer::getSingletonPtr()->getPlayer()->getYaw()-Math::PI/2);
				
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
	

} // namespace Logic