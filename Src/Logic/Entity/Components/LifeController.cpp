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
#include "Graphics/Scene.h" //Pablo 03-02-2013
#include "Graphics/Server.h" //Pablo 04-02-2013

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"


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
		return true;
	}
		

	void CLifeController::deactivate()
	{
	}

	
		 bool CLifeController::accept(const CMessage *message)
		 {
			//return message._type == Message::CONTROL; 
			return message->getType() == Message::CONTACT;
			//De momento, luego tendrá que aceptar de otras entidades NPC
			 return false;
		 }

		
		 void CLifeController::process(CMessage *message)
		 {
			switch(message->getType())
			{
			case Message::CONTACT:
				CMessageFloat *maux = static_cast<CMessageFloat*>(message);
				if(message->getAction() == Message::UPDATE_LIFE)
					updateLife(maux->getFloat());
			}

		 }

	 void CLifeController::tick(unsigned int msecs)
	 {
	 		IComponent::tick(msecs);
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
				_life-=damage;

				float ratio = _life / _maxLife;
    
				if (ratio < 0.0f)
				{
					ratio = 0.0f;

					if(!_entity->isPlayer())
					{
						//Eliminacion del billboard de la entidad
						std::string name = _entity->getName();
						_bb->deactivateBillboard(name);

						//Eliminacion de la entidad a traves de la factoria de entidades

						//Esto da un error en ejecucion
						//CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
						//entityFactory->deleteEntity(_entity);

						//Eliminacion de la entidad a traves de la eliminacion de su sceneNode
						//SceneNode::removeAndDestroyAllChildren();
						//followed by
						//SceneManager::destroySceneNode( .. );
					

						//Se consigue la escena activa y se elimina el SceneNode con la entidad
						//Graphics::CScene* _cscene = Graphics::CServer::getSingletonPtr()->getActiveScene();
						//_cscene->deleteSceneNode(name);

						//Se elimina la entidad
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);

					}

					return;
				}
        
				float u1 = (1.0f - ratio) / 2.0f;
				float v1 = 0.0f;
				float u2 = 0.5f + (1.0f - ratio) / 2.0f;
				float v2 = 1.0f;
        
				_bb->setCoordenadas(u1, v1, u2, v2);
		}
	

} // namespace Logic