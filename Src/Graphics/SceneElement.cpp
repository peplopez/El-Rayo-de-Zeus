//---------------------------------------------------------------------------
// SceneElement.cpp
//---------------------------------------------------------------------------

/**
@file SceneElement.cpp

Contiene la implementacion de la clase base de cualquier elemento de escena.

@see Graphics::CSceneElement

@author FRS
@date Marzo, 2013
*/

#include "SceneElement.h"

#include "Graphics/Scene.h"

#include <assert.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreStaticGeometry.h>

namespace Graphics 
{


	CSceneElement::~CSceneElement() 
	{
		assert(!_scene && "¡¡Para destruir este elemento grafico no puede pertenecer a una escena!!");		
	} // ~CSceneElement
	
	//--------------------------------------------------------
		
	bool CSceneElement::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡El elemento grafico debe asociarse a una escena!!");
		
		if(_loaded) {	// Si el elemento ya había sido cargado... 
			if(_scene != scene)
				return false;	// ... por otro gestor de escena.
			else
				return true;	// ... por la misma escena
		
		} else { // Si no estaba cargado forzamos su carga.
			_scene = scene;
			return load();
		}

	} // attachToScene
	
	//--------------------------------------------------------
		
	bool CSceneElement::detachFromScene()
	{
		if(!_loaded) // Si el elemento no estaba cargado no se puede quitar de
			return false; // una escena, ya que no pertenecerá a ninguna.
		
		else { // Si el elemento se había cargado forzamos su descarga.
			assert(_scene && "¡¡El elemento grafico debe estar asociada a una escena!!");
			unload();
			_scene = 0;
			return true;
		}

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	void CSceneElement::unload()
	{
		if(_node)	{ // desacoplamos el elemento de su nodo	
			assert(_scene && "¡¡El elemento grafico debe estar asociada a una escena!!");
			_node->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_node); // y lo liberamos
			_node = 0;
		}// create a node to attach the effect
		
		_loaded = false;
	} // unload
	
	//--------------------------------------------------------

	bool CSceneElement::addToStaticGeometry()
	{
		if(_node){
			_scene->getStaticGeometry()->addSceneNode(_node);
			_node->detachAllObjects();
			return true;
		
		} else
			return false;

	} // addToStaticGeometry




	/********************
		GET's & SET's
	*******************/

	Ogre::SceneManager* CSceneElement::getSceneMgr() { 
		assert(_scene);
		return _scene->getSceneMgr();					
	}

	//---------------------- VISIBLE ----------------------------------
		
	void CSceneElement::setVisible(bool visible)
	{
		assert(_node && "El elemento no ha sido cargado en la escena");
		if(_node)
			_node->setVisible(visible); // estable visibilidad en cascada desde el nodo

	} // setVisible

		
	//------------------------TRANSFORM --------------------------------

	void CSceneElement::setTransform(const Matrix4 &transform)
	{
		assert( _node && "El elemento no ha sido cargado en la escena");
		if(_node) {
			_node->setPosition( transform.getTrans() );
			_node->setOrientation( transform.extractQuaternion() );
		}
	} // setTransform

	//------------------------------------------------------------------

	void CSceneElement::setPosition(const Vector3 &position)
	{
		assert(_node && "El elemento no ha sido cargado en la escena");
		if(_node)
			_node->setPosition(position);

	} // setPosition

	//------------------------------------------------------------------
	
	void CSceneElement::setOrientation(const Matrix3 &orientation)
	{
		assert(_node && "El elemento no ha sido cargado en la escena");
		if(_node)
			_node->setOrientation(orientation);
	} // setOrientation

	
	//-------------------------- SCALE ------------------------------
		

	const Vector3& CSceneElement::getScale()
	{
		assert(_node && "El elemento no ha sido cargado en la escena");
		if(_node)
			return _node->getScale();
		return Vector3::ZERO; //Pep, así elimino un warning
	}

	//------------------------------------------------------------------

	void CSceneElement::setScale(const Vector3 &scale)
	{
		assert(_node && "El elemento no ha sido cargado en la escena");
		if(_node)
			_scale = scale;
			_node->setScale(scale);
	} // setScale

	//------------------------------------------------------------------	



} // namespace Graphics
