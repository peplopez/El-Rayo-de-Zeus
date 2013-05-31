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
			
			//ESC - cuando se hace unload queremos destruir tambien todos los objetos atachados a los huesos de la entidad.
			Ogre::Entity::ChildObjectListIterator it =_entity->getAttachedObjectIterator();
				while (it.hasMoreElements())				
					getSceneMgr()->destroyEntity(
						static_cast<Ogre::Entity*>( it.getNext() )
					);
				
			_loaded = false;
			getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
			_boneObjectsTable.clear();
		}

	} // unload



	/***************
		ATTACH
	***************/


	void CEntity::attach(const std::string &toBone, const std::string &mesh, bool permanently) 
	{
		assert(_node && "La entidad no ha sido cargada en la escena");
		if(!_node) return;

		// PERMANENCIA: Algunos TAttachPoint son permanentes por definición; p.e FACE -> rasgos faciales estáticos
		permanently |= toBone == BONE_DICTIONARY[TAttachPoint::FACE];

		// AttachName = "parentName.bone.attachedMesh"
		std::string objectName = _name + '.';		
			objectName.append(		// FRS Si toBone tiene espacios (p.e "Bip01 LeftHand")
				toBone,				// nos quedamos con la ultima parte
				toBone.find_last_of(' ') == -1 ? 0 : toBone.find_last_of(' '), 
				toBone.length() - 1
			); 
			objectName.append(".");
			objectName.append( mesh, 0, mesh.find_last_of('.') ); // Suprimimos sufijo ".mesh"

		if( !getSceneMgr()->hasEntity(objectName) ) // No existe un objeto con el mismo nombre en la escena
		{	
			Ogre::Entity* newObject = getSceneMgr()->createEntity(objectName, mesh);			
				_entity->attachObjectToBone(toBone, newObject);	

			if(!permanently) { // FRS Apilamos solo attachables no permanentes, para poder gestionar después el detach
				TAttachedMeshes& boneObjects = _boneObjectsTable[toBone];
					if( !boneObjects.empty() )					// Si ya había objetos "atachados" a ese hueso
						boneObjects.top()->setVisible(false); // ocultamos el último attach que tenía el hueso	
					boneObjects.push(newObject); 
			}
		}

		// FRS Solo atachamos si no existe una entidad con el mismo objectName ya
		// Un objectName en uso indica que ya ha sido atachado ese objeto al mismo hueso;
		// ya está en la pila de AttachedMeshes de ese hueso y, por tanto, no se debería atachar de nuevo.

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
						attach(boneDeque->first, *attachedMeshName);			
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
