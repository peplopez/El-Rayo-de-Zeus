//---------------------------------------------------------------------------
// StaticEntity.h
//---------------------------------------------------------------------------

/**
@file StaticEntity.h

Contiene la declaración de la clase que representa una entidad gráfica 
estática.

@see Graphics::CStaticEntity
@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_StaticEntity_H
#define __Graphics_StaticEntity_H

#include "Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class CScene;
}

namespace Graphics 
{
	/**
	Clase que representa una entidad gráfica estática. Especializa la clase
	Graphics::CEntity por lo que contiene una referencia a una entidad de 
	Ogre y al Nodo que la contiene. Añade la posibilidad de añadirse a una
	geometría estática. 
	<p>
	@remarks Una vez añadida la entidad a la geometría estática las modificaciones
	hechas en los atributos de la clase Graphics::CEntity no tendrán efecto en
	lo que se verá en la escena, así que se deben hacer todos los cambios antes
	de invocar a addToStaticGeometry. De hecho las estructuras creadas en Ogre
	antes de invocer a addToStaticGeometry sirven unicamente como referencia para 
	crear la geometría estática. Tras la creación, se mantienen las estructuras de 
	las entidades dinámicas de Ogre por si se requieren para cargar de nuevo en otra
	escena.
	<p>
	Oculta los detalles escabrosos del motor gráfico.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CStaticEntity : public CEntity
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CStaticEntity(const std::string &name, const std::string &mesh):
					CEntity(name,mesh) {}

		/**
		Destructor de la aplicación.
		*/
		virtual ~CStaticEntity() {}
	
	protected:

		/**
		Clase amiga. Solo la escena puede mandar a la entidad que pase a formar
		parte de la geometría estática.
		*/
		friend class CScene;

		/**
		Segundo paso de la carga de la entidad gráfica estática. Añade la 
		entidad a la geometría estática de la escena. En este paso, al añadir
		las entidades que contiene el nodo a la geometría estática, el nodo que 
		contiene la entidad se desacopla de la escena para que la entidad 
		no se pinte dos veces, una por geometría estática y otra por estar en 
		un nodo de la escena.
		<p>
		@remarks Una vez añadida la entidad a la geometría estática las modificaciones
		hechas en los atributos de la clase Graphics::CEntity no tendrán efecto en
		lo que se verá en la escena, así que se deben hacer todos los cambios antes
		de invocar a este método. Se mantiene sin embargo las estructuras de las 
		entidades dinámicas de Ogre por si se requieren para cargar de nuevo en otra
		escena.

		@return true si la geometría estática se pudo crear correctamente.
		*/
		bool addToStaticGeometry();

	}; // class CStaticEntity

} // namespace Graphics

#endif // __Graphics_StaticEntity_H
