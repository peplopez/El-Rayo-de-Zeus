/**
@file Death.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CDeath
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Death.h"
//#include "Application/BaseApplication.h"
//#include "Graphics/AnimatedEntity.h"
//#include "Logic/Entity/Entity.h"
//#include "Logic/Maps/EntityFactory.h"
// FRS Prohibido depender de Application en Logic (siempre que se pueda); además el gameover lo gestiona ya la FSM 

//#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Messages/Message.h"
//#include "Logic/Entity/Messages/MessageString.h"
//#include "Logic/Entity/Messages/MessageBoolString.h"
//#include "Logic/Entity/Messages/MessageAudio.h"

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"

namespace Logic 
{
	IMP_FACTORY(CDeath);
	
	//---------------------------------------------------------

	bool CDeath::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;
	} // spawn
	
	//---------------------------------------------------------

	bool CDeath::accept(const CMessage *message)
	{
		return	message->getType() == Message::DEAD;

	} // accept
	
	//---------------------------------------------------------

	// TODO separar en funciones / tipo
	void CDeath::process(CMessage *message)
	{
		// HACK FRS habrá que tener en cuenta todos los que tengan muerte animada...
		// (por parámetro en map.txt? animatedDeath = true o CDeathAnimated?)
			death(message);
	
	} // process


	//---------------------------------------------------------


	void CDeath::death (CMessage *message) {
		if(message->getType() == TMessageType::DEAD)
		{
		//CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);

		//init HUD Layout and functions
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("RespawnPlayer");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initRespawn");
		}
	} // death

	//---------------------------------------------------------



} // namespace Logic

