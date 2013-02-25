//David Llanso Tutoria
/*

//Constructor
CParticle(name){

	_bbset = CServer::getSingletonPtr()->getActiveScene()->createParticle(name);
}

// Destructor 

*/
//metodos de setDimensions....activate, deactivate, setcoord...
//Dentro se utilizaria. _bbset->getParticle(0) para acceder al primer Particle del conjunto de Particles.

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
#include "Scene.h"
#include "Server.h"

//#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>
#include <OgreParticle.h>

namespace Graphics 
{
	//Constructor de la clase CParticle. Donde se crea un ParticleSet (de momento con 1 sólo Particle)
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


} // namespace Graphics

