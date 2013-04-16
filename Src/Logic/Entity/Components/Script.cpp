/**
@file Script.cpp

Contiene la implementación del componente que contiene el control y la ejecución de los
scripts asociados a una entidad.

@see Logic::IComponent

@author Pablo Terrado
@date Abril, 2013
*/

#include "Script.h"
#include "Map/MapEntity.h"
#include "ScriptManager/Server.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageInt.h"
#include "Logic/Entity/Messages/MessageAudio.h"

/*
#include "Logic/Entity/Messages/SetAnimation.h"
#include "Logic/Entity/Messages/MoveSteering.h"
#include "Logic/Entity/Messages/Healed.h"
#include "Logic/Entity/Messages/CureEntity.h"
#include "Logic/Entity/Messages/ActivateSkill.h"
*/

#include <sstream>

namespace Logic
{
	IMP_FACTORY(CScript);

	//---------------------------------------------------------

	bool CScript::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if (entityInfo->hasAttribute("script"))
		{
			ScriptManager::CServer::getSingletonPtr()->loadExeScript(entityInfo->getStringAttribute("script").c_str());
		}


		if (entityInfo->hasAttribute("tickFunction"))
		{
			_tickFunction = true;
			_luaTickFunction = entityInfo->getStringAttribute("tickFunction").c_str();
		}
		else
		{
			_tickFunction = false;
		}


		if (entityInfo->hasAttribute("activateFunction"))
		{
			_activateFunction = true;
			_luaActivateFunction = entityInfo->getStringAttribute("activateFunction").c_str();
		}
		else
		{
			_activateFunction = false;
		}

		return true;

	} // spawn

	//---------------------------------------------------------

	bool CScript::activate()
	{
		if (_activateFunction)
		{
			std::stringstream script;
			script << _luaActivateFunction << "()";
			ScriptManager::CServer::getSingletonPtr()->executeScript(script.str().c_str());
		}

		return true;

	} // activate

	//---------------------------------------------------------

	void CScript::deactivate()
	{


	} // deactivate

	//---------------------------------------------------------

	bool CScript::accept(const CMessage *message)
	{
		return false;

	} // accept

	//---------------------------------------------------------

	void CScript::process(const CMessage *message)
	{


	} // process

	//---------------------------------------------------------

	void CScript::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (_tickFunction)
		{
			std::stringstream script;
			script << _luaTickFunction << "(" << msecs << ")";
			ScriptManager::CServer::getSingletonPtr()->executeScript(script.str().c_str());
		}

	} // tick

	//---------------------------------------------------------

} // namespace Logic