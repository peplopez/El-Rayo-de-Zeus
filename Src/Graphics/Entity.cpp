//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
		
	bool CEntity::load()
	{
		try{
			_entity = getSceneMgr()->createEntity(_name, _mesh);		
		
			_node = getSceneMgr()->getRootSceneNode()
					->createChildSceneNode(_name + "_node");		
				_node->attachObject(_entity);		

			// HACK Emily: cutre - para attach del arma en el Player
			if(!_name.compare("Mono")) { // que es como llamamos al player en el mapa		
				Ogre::Entity *weapon = getSceneMgr()->createEntity("weapon", "Katana.mesh");
					_entity->attachObjectToBone("Bip01 R Hand",weapon);
			}

			_loaded = true;

		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;
	} // load
	
	//--------------------------------------------------------
		
	void CEntity::unload()
	{
		CSceneElement::unload();
		
		if(_entity){

		// HACK Emily 2ª parte
			_entity->detachAllObjectsFromBone(); // TODO Necesario hacer detach y destroy en arbol?
			getSceneMgr()->destroyEntity( "weapon" );
		//
			getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
		}

	} // load

	
	
	/********************
		GET's & SET's
	*******************/

		
	bool CEntity::isVisible() const
	{
		assert(_node && "La entidad no ha sido cargada en la escena");
		return _entity->isVisible();
	} // getPosition
	
	////--------------------------------------------------------

	// UNDONE FRS En principio no sería legal sacar ogre de graphics

	//Ogre::SceneNode* CEntity::getEntityNode()
	//{
	//	assert(_node && "La entidad no ha sido cargada en la escena");
	//	return _node;
	//}

	//--------------------------------------------------------

	void CEntity::setMaterial(const std::string &materialName) 
	{
		assert(_node && "La entidad no ha sido cargada en la escena");
		if(_node)
			_entity->setMaterialName(materialName);	
	} // setMaterial

	//--------------------------------------------------------
	void CEntity::setSubEntityMaterial(const std::string &materialName, unsigned int subEntityIndex) 
	{
		assert(_node && "La entidad no ha sido cargada");
		if(_node)
			_entity->getSubEntity(subEntityIndex)->setMaterialName(materialName);
	} // setSubEntityMaterial
	
	


} // namespace Graphics
