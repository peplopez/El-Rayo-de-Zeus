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
			
			reattachAllMeshes();

			_node->setScale(_scale);

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
			
			//ESC - cuando se hace unload queremos destruir tambien todos los objetos atachados a los huesos de la entidad.
			Ogre::Entity::ChildObjectListIterator it =_entity->getAttachedObjectIterator();
			while (it.hasMoreElements())
			{
				Ogre::Entity* entity = static_cast<Ogre::Entity*>(it.getNext());
				getSceneMgr()->destroyEntity(entity);
			}
			_loaded = false;
			getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
			_boneObjectsTable.clear();
		}

	} // unload



	/***************
		ATTACH
	***************/
	
	void CEntity::attach(const std::string &toBone, const std::string &mesh) 
	{
		reattach(toBone, mesh);
		_boneObjectsNameTable[toBone].push_back(mesh); 


	} // attach

	//--------------------------------------------------------	

	void CEntity::reattach(const std::string &toBone, const std::string &mesh) 
	{
		assert(_node && "La entidad no ha sido cargada en la escena");
		if(!_node) return;

		std::string objectName = _name + '.';
			objectName.append( mesh, 0, mesh.find_last_of('.') ); // Sufijo = meshName sin la extension (.mesh)

		// FRS De momento, no permite "atachar" el mismo mesh más de una vez en un único cuerpo.
		assert( !getSceneMgr()->hasEntity(objectName) && "Ya existe un objeto con el mismo nombre en la escena");
		
		TAttachedMeshes& boneObjects = _boneObjectsTable[toBone];
			if( !boneObjects.empty() )
				// Si ya había objetos "atachados" a ese hueso
				boneObjects.top()->setVisible(false); // ocultamos el último attach que tenía el hueso	

		Ogre::Entity* newObject = getSceneMgr()->createEntity(objectName, mesh);			
			_entity->attachObjectToBone(toBone, newObject);
			boneObjects.push(newObject); 

	} // reattach

	//--------------------------------------------------------
	
	void CEntity::reattachAllMeshes()
	{
		//se recorre el mapa de bicolas para volver a attachar todos los meshes
		TBoneObjectNamesTable::const_iterator boneDeque = _boneObjectsNameTable.begin();
		TBoneObjectNamesTable::const_iterator lastBoneDeque = _boneObjectsNameTable.end();
		for (; boneDeque != lastBoneDeque; ++boneDeque)
		{
			TAttachedMeshNames::const_iterator attachedMeshName = boneDeque->second.begin();
			TAttachedMeshNames::const_iterator lastAttachedMeshName = boneDeque->second.end();

			for (; attachedMeshName != lastAttachedMeshName; ++attachedMeshName)
			{
				reattach(boneDeque->first, *attachedMeshName);
			}


		}	
	} //reattachAllMeshes()

	//--------------------------------------------------------

	void CEntity::detach(const std::string &fromBone) 
	{
		assert(_node && "La entidad no ha sido cargada en la escena");

		TAttachedMeshes& boneObjects = _boneObjectsTable[fromBone];
			if( boneObjects.empty() ) 
				return;				// Ningún objeto "atachado" a ese hueso

		getSceneMgr()->destroyEntity(  boneObjects.top() ); // Eliminamos último attach
		
		_boneObjectsNameTable[fromBone].pop_back();
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
