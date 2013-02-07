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
	//CParticle::CParticle(const std::string &name, Vector3 position) //le paso un string y la posicion de la entidad
	CParticle::CParticle(const std::string &name, const float offset) //le paso un string y el offset de la pos del Particle
	{
		//Graphics::CServer::getSingletonPtr()->getActiveScene()->createParticle();
		//Desde el singletonPtr del Server grafico, puedo llamar al metodo getActiveScene para
		//conseguir la escena actual. Y desde la escena puedo llamar al método createParticle
		//para crear el ParticleSet desde la escena
		//le paso un string y la position
		//_bbSet = Graphics::CServer::getSingletonPtr()->getActiveScene()->createParticle(name,position);

		// Pablo 28-01-2013. Indico que la escena activa es map.txt (no dummy_scene)
		//Graphics::CServer::getSingletonPtr()->setScene("map.txt");

		//le paso un string
		//_bbSet = Graphics::CServer::getSingletonPtr()->getActiveScene()->createParticle(name); 
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//_particleSystem = _scen->createParticleSystem("smoke"+contador,"smokeParticles"); 

		//contador = 0;


	} // CParticle

	//--------------------------------------------------------

	void CParticle::deactivateParticle(const std::string &name) 
	{
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//_scen->deleteParticle(name);

	} // deactivateParticle

	//Las coordenadas se le dan al Particle.
	void CParticle::setCoordenadas(const float u0,const float v0,const float u1,const float v1){
		//_bbSet->getParticle(0)->setTexcoordRect(u0,v0,u1,v1);
	}

	//Las dimensiones se le dan al Particle
	void CParticle::setDimensions(const float width,const float height){
		//se usa la función de Ogre setDimensions sobre el Particle para darle unas dimensiones
		//_bbSet->getParticle(0)->setDimensions(width,height);

	}

	//aqui debería de colocarse encima de la entidad.
	/*void CParticle::setPosition(const Vector3 v){ //x,y,z
		
	}*/

	//El material se realiza sobre el ParticleSet
	void CParticle::setMaterial(const std::string &name){
		//_bbSet->setMaterialName(name); //se utiliza ogre para darle un material al ParticleSet
	}

	//--------------------------------------------------------


} // namespace Graphics

