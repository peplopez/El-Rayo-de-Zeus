//---------------------------------------------------------------------------
// ParticleSystem.cpp
//---------------------------------------------------------------------------

/**
@file ParticleSystem.cpp

Contiene la implementación de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author Pablo Terrado
@date Enero, 2013
*/

#include "ParticleSystem.h"

#include <assert.h>

//#include <OgreParticle.h>
#include <OgreParticleSystem.h>
//#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{	
	
	//PT. Creacion de una particula
	bool CParticleSystem::load()
	{
		try{
			_particleSystem =  // Creamos nuestro sistema de partículas 
				getSceneMgr()->createParticleSystem(_name, _templateName);

			// POS. RELATIVA (particulas hijas de otra entidad gráfica)
			if( getSceneMgr()->hasSceneNode( _name + "_node") ) 
				_node = getSceneMgr()->getSceneNode( _name + "_node")
							->createChildSceneNode(_name + "_particleSystem");

			// POS. ABSOLUTA
			else 
				_node = getSceneMgr()->getRootSceneNode()
							->createChildSceneNode(_name + "_particleSystem" );	

			_node->attachObject( _particleSystem ); // Atachamos particulas al nodo
			_loaded = true;

		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;		

	} // load

	//-------------------------------------------------------------

	void CParticleSystem::unload() {

		CSceneElement::unload();

		// TODO

	//	Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//_scen->deleteParticle(name);

		// Desvinculamos el sistema de partículas del nodo
		/*
		sceneNode->detachObject(pssmoke);
 
		// Destruimos el nodo
		_sceneMgr->destroySceneNode(sceneNode);
 
		// Destruimos el sistema de partículas
		_sceneMgr->destroyParticleSystem(pssmoke);
		*/

	} // unload



} // namespace Graphics

