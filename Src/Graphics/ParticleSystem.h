//---------------------------------------------------------------------------
// ParticleSystem.h
//---------------------------------------------------------------------------

/**
@file ParticleSystem.h

Contiene la declaración de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_ParticleSystem_H
#define __Graphics_ParticleSystem_H

#include "Graphics\SceneElement.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{	
	class ParticleSystem;
};


namespace Graphics 
{
	/**
	Clase de ParticleSystem extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class CParticleSystem : public CSceneElement
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del ParticleSystem.
		*/
		CParticleSystem(const std::string &name, const std::string &templateName) : 
		  _name(name), _templateName(templateName){}

	protected:

		// CScene es la única que puede añadir o eliminar partículas de una 
		// escena y por tanto cargar o descargar sistemas de partículases.
		// Por otro lado cada sistema de partículas debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/** 
		ParticleSystem 
		*/
		Ogre::ParticleSystem* _particleSystem;


	private:

		/**
		Nombre del ParticleSystem.
		*/
		std::string _name;
		std::string _templateName; 

		
		/**
		Carga la sistema de partículas gráfica correspondiente al nombre _mesh. No hace 
		comprobaciónes de si la sistema de partículas está ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la sistema de partículas pudo crear los objetos necesarios en Ogre
		o si la sistema de partículas ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si la sistema de partículas está o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();

	}; // class CParticleSystem



} // namespace Graphics

#endif // __Graphics_ParticleSystem_H
