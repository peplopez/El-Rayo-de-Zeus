//---------------------------------------------------------------------------
// Billboard.cpp
//---------------------------------------------------------------------------

/**
@file Billboard.cpp

Contiene la implementación de la clase que maneja el billboard.

@see Graphics::CBillboard

@author Pablo Terrado
@date Enero, 2013
*/

#include "Billboard.h"

#include "Graphics/Scene.h"

#include <assert.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

namespace Graphics 
{
	
	CBillboard::~CBillboard() 
	{
		assert(!_scene && "¡¡Para destruir un billboard éste no puede pertenecer a una escena!!");
	} // ~CEntity

	//--------------------------------------------------------

	//Constructor de la clase CBillboard. Donde se crea un BillboardSet (de momento con 1 sólo Billboard)
	bool CBillboard::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡El billboard debe asociarse a una escena!!");
		
		// Si el billboard está cargado por otro gestor de escena.
		if(_loaded) {			
			if(_scene != scene)
				return false;
			else
				return true; // Si ya estaba cargado en la escena se devuelve cierto.
		
		} else { // Si no está cargado forzamos su carga.		
			_scene = scene;
			return load();
		}
	} // attachToScene
	
	//--------------------------------------------------------
		
		
	bool CBillboard::deattachFromScene()
	{
		// Si  el billboard no está cargado no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!_loaded)
			return false;		
		else { // Si está cargado forzamos su descarga.
			assert(_scene && "¡¡El billboard debe estar asociada a una escena!!");
			unload();
			_scene = 0;
			return true;
		}
	} // deattachFromScene
	
	//--------------------------------------------------------
		
	bool CBillboard::load()
	{
		try{
			// PT: Necesario un set como nodo del billboard
			// Como solo vamos a insertar un billboard, el poolSize = 1
			_bbSet =  _scene->getSceneMgr()->createBillboardSet(_parentName + "_billboard", 1); 
				_bbSet->setMaterialName(_material); // material por defecto	
				Ogre::Billboard* billboard = _bbSet->createBillboard( _relativePos );			
					billboard->setDimensions(_width, _height);

			assert( _scene->getSceneMgr()->hasSceneNode( _parentName + "_node") );
			_graphicalNode = _scene->getSceneMgr()->getSceneNode( _parentName + "_node")
								->createChildSceneNode(_parentName + "_billboard");
				_graphicalNode->attachObject(_bbSet);	

			_loaded = true;
			return true;

		} catch(std::exception e){
			return false;
		}
		
	} // load
	
	//--------------------------------------------------------
		
	void CBillboard::unload()
	{
		if(_graphicalNode)	{
			_graphicalNode->detachAllObjects(); // desacoplamos la entidad de su nodo
			_scene->getSceneMgr()->destroySceneNode( _graphicalNode );
			_graphicalNode = 0;
			// PT: si la escena tiene un billboardset con el nombre de
			// la entidad de referencia  + "_billboard" se elimina ese billboardset
		}
		if(_bbSet)	{ //se elimina ese billboardset
			_scene->getSceneMgr()->destroyBillboardSet( _parentName + "_billboard");
			_bbSet = 0;
		}

	} // load

	//--------------------------------------------------------
		
	
	/**********************
		GET's & SET's
	***********************/

	//Las coordenadas se le dan al Billboard.
	void CBillboard::setTextureCoords(const float u0,const float v0,const float u1,const float v1){
		assert(_loaded && "Imprescindible haber cargado el billboard en escena primero");
		_bbSet->getBillboard(0)->setTexcoordRect(u0,v0,u1,v1);
	}

	//Las dimensiones se le dan al Billboard
	void CBillboard::setDimensions(const float width,const float height){
		assert(_loaded && "Imprescindible haber cargado el billboard en escena primero");
		_bbSet->getBillboard(0)->setDimensions(width,height);		//se usa la función de Ogre setDimensions sobre el billboard para darle unas dimensiones
	}

	//El material se realiza sobre el BillboardSet
	void CBillboard::setMaterial(const std::string &name){
		assert(_loaded && "Imprescindible haber cargado el billboard en escena primero");
		_bbSet->setMaterialName(name); //se utiliza ogre para darle un material al billboardSet
	}

	//--------------------------------------------------------


} // namespace Graphics

