//---------------------------------------------------------------------------
// Overlay.h
//---------------------------------------------------------------------------

/**
@file Overlay.h

Contiene la declaración de la clase que maneja el Overlay.

@see Graphics::COverlay

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_Overlay_H
#define __Graphics_Overlay_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneManager;
	class OverlayManager;
	class OverlayContainer;
	class Overlay;

}
namespace Graphics 
{
	class CScene;
	class CServer;
}

namespace Graphics 
{
	/**
	Clase de Overlay extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class COverlay
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del Overlay.
		*/
		//COverlay(const std::string &name, Vector3 position); // le paso un string y la posicion de la entidad
		COverlay(const std::string &name, const float offset); // le paso un string

		/**
		Destructor de la aplicación.
		*/
		//virtual ~COverlay();
		

		//TUTORIA

		//Poner coordenadas
		//_lifeBar->setTexcoordRect(0.0f, 0.0f, 0.5f, 1.0f);
		void setCoordenadas(const float,const float,const float,const float);

		//Poner dimensiones
		//_lifeBar->setDimensions(40, 2);
		void setDimensions(const float,const float);

		//... activate, deactivate...

		void activateOverlay();

		void deactivateOverlay(const std::string &name);

		//void setPosition(const Vector3);

		void setMaterial(const std::string &name);

		Ogre::OverlaySet* getBBSet() {return _bbSet;}


	protected:
		/** 
		OverlaySet _bbSet(conjunto de Overlays)
		*/
		Ogre::OverlaySet* _bbSet;

		/**
		Nombre del Overlay.
		*/
		std::string _namebb;

	}; // class COverlay

} // namespace Graphics

#endif // __Graphics_Overlay_H