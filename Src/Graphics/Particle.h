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
#include "Logic\Entity\Entity.h" // Pablo. al cambiar el CParticle(string) por CParticle(entity)

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneNode;
	class ParticleSystem;
	class Particle;

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
		//CParticle(const std::string &name, Vector3 position); // le paso un string y la posicion de la entidad
		CParticle(const std::string &name, const float offset); // le paso un string

		/**
		Destructor de la aplicación.
		*/
		//virtual ~CParticle();
		

		//TUTORIA

		//Poner coordenadas
		//_lifeBar->setTexcoordRect(0.0f, 0.0f, 0.5f, 1.0f);
		void setCoordenadas(const float,const float,const float,const float);

		//Poner dimensiones
		//_lifeBar->setDimensions(40, 2);
		void setDimensions(const float,const float);

		//... activate, deactivate...

		void activateParticle();

		void deactivateParticle(const std::string &name);

		//void setPosition(const Vector3);

		void setMaterial(const std::string &name);

		Ogre::ParticleSystem* getParticleSystem() {return _particleSystem;}


	protected:
		/** 
		ParticleSystem _bbSet(conjunto de Particles)
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
