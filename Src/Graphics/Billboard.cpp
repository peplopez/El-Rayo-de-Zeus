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

#include <assert.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

namespace Graphics 
{		
	bool CBillboard::load()
	{
		assert( getSceneMgr()->hasSceneNode( _parentName + "_node") );

		try{
			// PT: Necesario un set como nodo del billboard
			// Como solo vamos a insertar un billboard, el poolSize = 1
			_bbSet =  getSceneMgr()->createBillboardSet(_parentName + "_billboard", 1); 
				_bbSet->setMaterialName(_material); // material por defecto	

			Ogre::Billboard* billboard = _bbSet->createBillboard( _relativePos );			
				billboard->setDimensions(_width, _height);
			
			_node = getSceneMgr()->getSceneNode( _parentName + "_node")
								->createChildSceneNode(_parentName + "_nodeBillboard");
				_node->attachObject(_bbSet);

			setTextureCoords(_u0, _v0, _u1, _v1);

			_loaded = true;

		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;		
	} // load
	
	//--------------------------------------------------------
		
	void CBillboard::unload()
	{
		CSceneElement::unload();

		// PT: si la escena tiene un billboardset con el nombre de
		// la entidad de referencia  + "_billboard" se elimina ese billboardset		
		if(_bbSet)	{ //se elimina ese billboardset
			getSceneMgr()->destroyBillboardSet( _parentName + "_billboard");
			_bbSet = 0;
		}

	} // unload




	
	/**********************
		GET's & SET's
	***********************/

	bool CBillboard::isVisible() const {
		return _bbSet->isVisible();
	}

	//Las coordenadas se le dan al Billboard.
	void CBillboard::setTextureCoords(const float u0,const float v0,const float u1,const float v1){
		assert(_loaded && "Imprescindible haber cargado el billboard en escena primero");
		_u0 = u0; _v0 = v0; _u1 = u1; _v1 = v1;
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

