//---------------------------------------------------------------------------
// Billboard.h
//---------------------------------------------------------------------------

/**
@file Billboard.h

Contiene la declaración de la clase que maneja el billboard.

@see Graphics::CBillboard

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_Billboard_H
#define __Graphics_Billboard_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h" // Pablo. al cambiar el CBillboard(string) por CBillboard(entity)

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneNode;
	class BillboardSet;
	class Billboard;

}
namespace Graphics 
{
	class CScene;
	class CServer;
}

namespace Graphics 
{
	/**
	Clase de billboard extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class CBillboard
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del billboard.
		*/
		CBillboard() {} // ctor por defecto
		CBillboard(const std::string &name, const float position, const float width, const float height); // le paso un string y la posicion

		/**
		Destructor de la aplicación.
		*/
		//virtual ~CBillboard();
		

		//TUTORIA

		//Poner coordenadas
		//_lifeBar->setTexcoordRect(0.0f, 0.0f, 0.5f, 1.0f);
		void setCoordenadas(const float,const float,const float,const float);

		//Poner dimensiones
		//_lifeBar->setDimensions(40, 2);
		void setDimensions(const float,const float);

		//... activate, deactivate...

		void activateBillboard();

		void deactivateBillboard(const std::string &name);

		//void setPosition(const Vector3);

		void setMaterial(const std::string &name);

		Ogre::BillboardSet* getBBSet() {return _bbSet;}


	protected:
		/** 
		BillboardSet _bbSet(conjunto de Billboards)
		*/
		Ogre::BillboardSet* _bbSet;

		/**
		Nombre del billboard.
		*/
		std::string _namebb;

	}; // class CBillboard

} // namespace Graphics

#endif // __Graphics_Billboard_H
