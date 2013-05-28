/**
@file DotSceneLoader.cpp


 
@see Logic::CDotSceneLoader
@see Logic::IComponent

@author 
@date 
*/

#include "DotSceneLoader.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/DotSceneLoader/DotSceneLoader.h"
#include "Graphics/Scene.h"
#include "Logic/Maps/EntityFactory.h"


#include "Logic/Entity/Messages/Message.h"


namespace Logic 
{
	IMP_FACTORY(CDotSceneLoader);
	
	//---------------------------------------------------------

	CDotSceneLoader::~CDotSceneLoader() 
	{
		

	} // ~CDotSceneLoader

	//---------------------------------------------------------

	bool CDotSceneLoader::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getGraphicScene();
			assert(_scene && "Escena gráfica es NULL");

		assert(entityInfo->hasAttribute("sceneFile"));
			_sceneFile = entityInfo->getStringAttribute("sceneFile");
#ifdef _DEBUG

#else		
		Graphics::DotSceneLoader loader;
		loader.parseDotScene(_sceneFile, "General", _scene->getSceneMgr());
#endif
		return true;

	} // spawn
		
} // namespace Logic

