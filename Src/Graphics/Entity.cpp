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
	CEntity::TBoneDictionary CEntity::BONE_DICTIONARY = CEntity::initBoneDictionary();
		
	bool CEntity::load()
	{
		try{
			_entity = getSceneMgr()->createEntity(_name, _mesh);		
		
			_node = getSceneMgr()->getRootSceneNode()
					->createChildSceneNode(_name + "_node");		
				_node->attachObject(_entity);		

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

		// UNDONE FRS Necesario?
			//_entity->detachAllObjectsFromBone(); // TODO Necesario hacer detach y destroy en arbol?
			//getSceneMgr()->destroyEntity( "weapon" ); FRS este destroy se ejecuta cuando cualquier entidad muere...
		//
			getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
		}

	} // unload



	/***************
		ATTACH
	***************/
	
	void CEntity::attach(const std::string &toBone, const std::string &mesh) 
	{
		assert(_node && "La entidad no ha sido cargada en la escena");
		if(!_node) return;

		std::string objectName = _name + '.';
			objectName.append( mesh, 0, mesh.find_last_of('.') ); // Sufijo = meshName sin la extension (.mesh)

		// FRS De momento, no permite "atachar" el mismo mesh más de una vez en un único cuerpo.
		assert( !getSceneMgr()->hasEntity(objectName) && "Ya existe un objeto con el mismo nombre en la escena");
		
		TAttachedMeshes boneObjects = _boneObjectsTable[toBone];
			if( !boneObjects.empty() )				// Si ya había objetos "atachados" a ese hueso
				boneObjects.top()->setVisible(false); // ocultamos el último attach que tenía el hueso	

		Ogre::Entity* newObject = getSceneMgr()->createEntity(objectName, mesh);			
			_entity->attachObjectToBone(toBone, newObject);
			boneObjects.push(newObject);

	} // attach

	//--------------------------------------------------------	

	void CEntity::detach(const std::string &fromBone) 
	{
		assert(_node && "La entidad no ha sido cargada en la escena");

		TAttachedMeshes boneObjects = _boneObjectsTable[fromBone];
			if( boneObjects.empty() ) 
				return;				// Ningún objeto "atachado" a ese hueso

		getSceneMgr()->destroyEntity(  boneObjects.top() ); // Eliminamos último attach

		boneObjects.pop();						// lo desapilamos
		boneObjects.top()->setVisible(true);	//  y activamos el siguiente apilado
	} // detach



	
	/********************
		GET's & SET's
	*******************/

		
	bool CEntity::isVisible() const
	{
		assert(_node && "La entidad no ha sido cargada en la escena");
		return _entity->isVisible();
	} // getPosition
	
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
