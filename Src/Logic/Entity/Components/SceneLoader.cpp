/**
@file SceneLoader.cpp


 
@see Logic::CSceneLoader
@see Logic::IComponent

@author 
@date 
*/

#include "SceneLoader.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/DotSceneLoader/DotSceneLoader.h"
#include "Graphics/Scene.h"
#include "Logic/Maps/EntityFactory.h"


#include "Logic/Entity/Messages/Message.h"


namespace Logic 
{
	IMP_FACTORY(CSceneLoader);
	
	//---------------------------------------------------------

	CSceneLoader::~CSceneLoader() 
	{
		

	} // ~CDotSceneLoader

	//---------------------------------------------------------

	bool CSceneLoader::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getGraphicScene();
			assert(_scene && "Escena gráfica es NULL");

		assert(entityInfo->hasAttribute("sceneFile"));
			_sceneFile = entityInfo->getStringAttribute("sceneFile");

		assert(entityInfor->hasAttribute("skyXPresetNumber")
			_skyXPresetNumber = entityInfo->getIntAttribute("skyXPresetNumber");

		assert(entityInfor->hasAttribute("hydraXConfigFile")
			_hydraXConfigFile = entityInfo->getStringAttribute("hydraXConfigFile");

#ifdef _DEBUG

#else		
		Graphics::DotSceneLoader loader;
		loader.parseDotScene(_sceneFile, "General", _scene->getSceneMgr());
#endif
		return true;

	} // spawn
		
} // namespace Logic

