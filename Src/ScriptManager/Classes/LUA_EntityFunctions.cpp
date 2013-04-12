/**
@file LUA_EntityFunctions.cpp

LUA

@author Pablo Terrado
@date Abril, 2013
*/

#include <cassert>
#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"

#include "Application/GaleonApplication.h"

namespace ScriptManager
{

	void deleteEntity(unsigned int entityID)
	{
		//Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(entityID));
	}

	//---------------------------------------------------------

	bool isPlayer(unsigned int entityID)
	{
		//Logic::CEntity* entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(entityID);
		//if (entity != NULL)
		//{
		//	return !entity->isPlayer();
		//}

		return false;
	}

	//---------------------------------------------------------

	unsigned int getEntityID (std::string entityName)
	{
		//Logic::CEntity* entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(entityName);
		//if (entity != NULL)
		//{
		//	return entity->getEntityID();
		//}

		return 0;
	}

	//---------------------------------------------------------

	std::string getName (unsigned int entityID)
	{
		//Logic::CEntity* entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(entityID);
		//if (entity != NULL)
		//{
		//	return entity->getName();
		//}
		
		return "";
	}

	//---------------------------------------------------------
		
	void gameOver ()
	{
		//Application::CGaleonApplication::getSingletonPtr()->setState("gameOver");
	}


} // namespace ScriptManager