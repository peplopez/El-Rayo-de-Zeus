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
		CBillboard(const std::string &parentName, const Vector3& relativePos, 
			const float width, const float height, const std::string material) :
			_parentName(parentName), _relativePos(relativePos), 
			_width(width), _height(height), _material(material),
			_loaded(false), _scene(0){}

		~CBillboard();

		/*****************
			GET's & SET'S
		*******************/

		//Poner coordenadas
		void setTextureCoords(const float u0,const float v0,const float u1,const float v1);

		//Poner dimensiones
		void setDimensions(const float,const float);

		void setMaterial(const std::string &name);


	protected:

		// CScene es la única que puede añadir o eliminar billboards de sí misma.	
		// Por otro lado cada billboard debe pertenecer a una escena. 
		friend class CScene;

		/**
		Añade el billboard al CScene pasado por parámetro. 
		Si el billboard no está cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere añadir
		la entidad.
		@return true si la entidad se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y eliminó de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();


	private:

		std::string _parentName;
		Vector3 _relativePos;
		float _width;
		float _height;
		std::string _material;

		/** 
		BillboardSet _bbSet(conjunto de Billboards)
		*/
		Ogre::BillboardSet* _bbSet;

		/**Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;

		/**	Nodo que contiene la entidad de Ogre.*/
		Ogre::SceneNode *_graphicalNode;

		/**	Indica si el billboard ha sido cargado en el motor gráfico.	*/
		bool _loaded;		
		
		/**
		Carga la entidad gráfica correspondiente al nombre _mesh. No hace 
		comprobaciónes de si la entidad está ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si la entidad está o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();

	}; // class CBillboard

} // namespace Graphics

#endif // __Graphics_Billboard_H
