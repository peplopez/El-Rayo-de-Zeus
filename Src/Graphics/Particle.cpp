//---------------------------------------------------------------------------
// Particle.cpp
//---------------------------------------------------------------------------

/**
@file Particle.cpp

Contiene la implementación de la clase que maneja el Particle.

@see Graphics::CParticle

@author Pablo Terrado
@date Enero, 2013
*/

#include "Particle.h"

#include "Graphics/Scene.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>
#include <OgreParticle.h>
#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{

	CParticle::~CParticle() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");		
	} // ~CEntity

	//--------------------------------------------------------

	//Constructor de la clase CParticle.
	CParticle::CParticle(const std::string &name, const std::string &templateName)
	{
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		_scen->createParticula(name,templateName); 
	} // CParticle

	//--------------------------------------------------------

	/*void CParticle::deactivateParticle(const std::string &name) 
	{
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//_scen->deleteParticle(name);

	} // deactivateParticle
	*/


	//--------------------------------------------------------


	//PT. Creacion de una particula
	bool CParticle::load(const std::string &name1, const std::string &name2)
	{

	char numstr[21]; // enough to hold all numbers up to 64-bits
	sprintf(numstr, "%d", counterParticles);

	// Creamos nuestro sistema de partículas :)
	Ogre::ParticleSystem *pssmoke;
	pssmoke = _sceneMgr->createParticleSystem(name1+numstr, name2);

	// Creamos un nodo y atachamos la particula pssmoke a ese scenenode
	Ogre::SceneNode* sceneNode = _sceneMgr->createSceneNode(name1+"_particleSystemNode_"+numstr);
	sceneNode->attachObject(pssmoke);

	if(_sceneMgr->hasSceneNode(name1+"_node"))
	{
		_sceneMgr->getSceneNode(name1+"_node")->addChild(sceneNode);
	}

	// Desvinculamos el sistema de partículas del nodo
	/*
	sceneNode->detachObject(pssmoke);
 
	// Destruimos el nodo
	_sceneMgr->destroySceneNode(sceneNode);
 
	// Destruimos el sistema de partículas
	_sceneMgr->destroyParticleSystem(pssmoke);
	*/

	counterParticles++;

	return pssmoke;

	}//createParticula


} // namespace Graphics

