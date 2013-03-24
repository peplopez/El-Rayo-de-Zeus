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

namespace Graphics 
{
	class CScene;
};

namespace Ogre 
{
	class SceneNode;
	class ParticleSystem;
};


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

		// CScene es la única que puede añadir o eliminar partículas de una 
		// escena y por tanto cargar o descargar sistemas de partículases.
		// Por otro lado cada sistema de partículas debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/** 
		ParticleSystem _particleSystem(sistema de Particulas)
		*/
		Ogre::ParticleSystem* _particleSystem;

		/**		
		Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;

			/**
		Nodo que contiene la sistema de partículas de Ogre.
		*/
		Ogre::SceneNode *_entityNode;
		


		/**
		Añade la sistema de partículas al SceneManager pasado por parámetro. Si la sistema de partículas
		no está cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere añadir
		la sistema de partículas.
		@return true si la sistema de partículas se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una sistema de partículas de la escena en la que se encuentra cargada.

		@return true si la sistema de partículas se descargo y eliminó de la escena
		correctamente. Si la sistema de partículas no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
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
		
		/**
		Actualiza el estado de la sistema de partículas cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		void tick(float secs);


	private:

		/**
		Nombre del ParticleSystem.
		*/
		std::string _name;

		/**
		Indica si las partículas han sido cargada en el motor gráfico.
		*/
		bool _loaded;

	}; // class CParticle

} // namespace Graphics

#endif // __Graphics_Particle_H
