//---------------------------------------------------------------------------
// Particle.h
//---------------------------------------------------------------------------

/**
@file Particle.h

Contiene la declaración de la clase que maneja el Particle.

@see Graphics::CParticle

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_Particle_H
#define __Graphics_Particle_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneNode;
	class ParticleSystem;

}
namespace Graphics 
{
	class CScene;
	class CServer;
}

namespace Graphics 
{
	/**
	Clase de Particle extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class CParticle
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del Particle.
		*/
		CParticle(const std::string &name, const std::string &templateName);

		/**
		Destructor de la partícula.
		*/
		~CParticle();
		

		Ogre::ParticleSystem* getParticleSystem() {return _particleSystem;}


	protected:
		/** 
		ParticleSystem _particleSystem(sistema de Particulas)
		*/
		Ogre::ParticleSystem* _particleSystem;

		/**
		Nombre del ParticleSystem.
		*/
		std::string _nameps;

		//friend int contador; //lo hago friend para que al crearse una particula, se incremente este contador.

	}; // class CParticle

} // namespace Graphics

#endif // __Graphics_Particle_H
