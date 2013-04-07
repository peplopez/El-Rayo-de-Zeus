//---------------------------------------------------------------------------
// SceneElement.h
//---------------------------------------------------------------------------

/**
@file SceneElement.h

Contiene la declaración de la clase base de cualquier elemento de escena.

@see Graphics::CSceneElement

@author FRS
@date Marzo, 2013
*/

#ifndef __Graphics_SceneElement_H
#define __Graphics_SceneElement_H

#include "BaseSubsystems/Math.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneNode;
	class SceneManager;
};

namespace Graphics 
{
	class CScene;
};

namespace Graphics 
{
	/**
	Contiene la declaración de la clase base de cualquier elemento de escena.

	
	@ingroup graphicsGroup

	@author FRS
	@date Marzo, 2013
	*/
	class CSceneElement 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CSceneElement::CSceneElement() : _node(0), _scene(0), _loaded(false) {} 

		/**
		Destructor de la aplicación.
		*/
		virtual ~CSceneElement();

		/**
		Segundo paso de la carga de un elemento gráfico estática. Añade el
		elemento a la geometría estática de la escena. En este paso, al añadir
		el elemento que contiene el nodo a la geometría estática, el nodo que 
		contiene el elemento se desacopla de la escena para que la entidad 
		no se pinte dos veces, una por geometría estática y otra por estar en 
		un nodo de la escena.
		<p>
		@remarks Una vez añadido el elemento a la geometría estática las modificaciones
		hechas en sus atributos no tendrán efecto en
		lo que se verá en la escena, así que se deben hacer todos los cambios antes
		de invocar a este método. Se mantiene sin embargo las estructuras de las 
		entidades dinámicas de Ogre por si se requieren para cargar de nuevo en otra
		escena.

		@return true si la geometría estática se pudo crear correctamente.
		*/
		bool CSceneElement::addToStaticGeometry(); 
		// FRS Movido a esta clase base porque es el único friend de CScene
		// y, por tanto, el único con acceso a _scene->getStaticGeometry()->addSceneNode(_node)
		// Además, a la geometría estática se le pasa un nodo de escena genérico,
		// el cual puede contener cualquier tipo de objeto.


		/******************
			GET's & SET's
		********************/
	
		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si el elemento debe dibujarse o no,
		 es decir, no está está dentro del campo de visión de la cámara o no.

		 @return Cierto si el elemento es visible (está activo para ser reenderizado).*/
		virtual bool isVisible() const = 0; // FRS virtual porque el nodo no ofrece getter de visibilidad

		/**
		 Establece la propiedad visible del elemento.
		 La propiedad indica si el elemento debe dibujarse o no,
		 es decir, no está relacionada con si está
		 dentro del campo de visión de la cámara o no.
		 <p>
		 El método cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _graphicalNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible); 

		/**
		Cambia la posición y orientación del elemento gráfico.
		@param transform Referencia a la matriz de transformación con la 
		que debe posicionarse y orientarse el elemento.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la posición del elemento.
		@param position Nueva posición para el nodo de escena contenedor del elemento.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la orientación del elemento.
		@param orientation Referencia a la matriz de rotación con la que debe orientarse el elemento.
		*/
		void setOrientation(const Matrix3 &orientation);

		// TODO FRS Getters de los 3 de arriba en caso de demanda

		// UNDONE FRS Analizar usos y si es necesario devolver ref
		///**
		// Devuelve el valor de la escala del elemento gráfico
		// @return Valores de la escala en los diferentes ejes.
		//*/
		//const Vector3 &getScale() const;

		/**
		 Escala el elemento
		 @param scale Valores de la escala en los diferentes ejes.
		 */
		void setScale(const Vector3 &scale);

		/**
		 Escala el elemento
		 @param scale Valor de la escala para los 3 ejes.
		 */
		void setScale(const float scale){ setScale( scale * Vector3::UNIT_SCALE ); } // setScale
		


	protected:

		// CScene es la única que puede añadir o eliminar elementos de una 
		// escena y por tanto cargarlos o descargarlos.
		// Por otro lado cada elemento debe pertenecer a una escena. 
		// Solo permitimos a la escena actualizar el estado.
		friend class CScene;
	
		/**	
		Controla todos los elementos Ogre de una escena.
		*/
		CScene *_scene;

		/**
		Nodo que contiene el elemento de escena
		*/
		Ogre::SceneNode *_node;

		/**
		Indica si el elemento ha sido cargada en el motor gráfico.
		*/
		bool _loaded;


		/**
		Añade el elemento al SceneManager pasado por parámetro. 
		Si el elemento no está cargado se fuerza su carga.

		@param scene escena de Ogre a la que se quiere añadir el elemento.
		@return true si el elemento se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga un elemento de la escena en la que se encuentra cargado.

		@return true si el elemento se descargo y eliminó de la escena
		correctamente. Si el elemento no estaba cargado se devuelve false.
		*/
		bool detachFromScene();
		
		/**
		Carga el elemento gráfico. No hace comprobaciónes de si ya estaba cargado
		o de si pertenece a otra escena. Esto se debe hacer de manera externa.

		@return true si el elemento pudo crear los objetos necesarios en Ogre
		o si ya estaba cargado.
		*/
		virtual bool load() = 0; 

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si el elemento está o no cargado o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		virtual void unload();
		
		/**
		Actualiza el estado del elemento en cada ciclo. 

		@param secs Número de mili segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float msecs) {}

		Ogre::SceneManager* CSceneElement::getSceneMgr();

	}; // class CSceneElement

} // namespace Graphics

#endif // __Graphics_SceneElement_H
