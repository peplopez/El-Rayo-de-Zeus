/**
@file LUA_EntityFunctions.h

LUA


@author Pablo Terrado
@date Abril, 2013
*/

#ifndef __ScriptManager_LUA_EntityFunctions_H
#define __ScriptManager_LUA_EntityFunctions_H

//PT
// Predeclaración de clases para ahorrar tiempo de compilación
namespace BaseSubsystems 
{
  class CServer;
  class CMath;
}

namespace Logic
{
	class CEntity;
	class CMap;
	class CEntityFactory;
	class CServer;
}

namespace Application
{
	class CGaleonApplication;
}
// Fin Predeclaración de clases para ahorrar tiempo de compilación


namespace ScriptManager
{

	void deleteEntity(unsigned int entityID);
	bool isPlayer (unsigned int entityID);
	unsigned int getEntityID (std::string entityName);
	std::string getName (unsigned int entityID);
	void gameOver ();
	

} // namespace ScriptManager

#endif // __ScriptManager_LUA_EntityFunctions_H