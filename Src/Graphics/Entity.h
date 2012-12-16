//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_Entity_H
#define __Graphics_Entity_H

#include "BaseSubsystems/Math.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Entity;
	class SceneNode;
}

namespace Graphics 
{
	class CScene;
}

namespace Graphics 
{
	/**
	Clase que representa una entidad gráfica. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gráfico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, ésta debe añadirse a una escenada de Ogre mediante el 
	método attachToScene(). Solo la clase CScene puede invocar los métodos 
	para añadir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al añadir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gráfica (posición, orientación, escala...) se deberán hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de éstas, que permitan
	modelos con distintas características. Por ejemplo, ésta clase
	no permite especificar la animación utilizada. Si queremos que podamos
	guardar un modelo gráfico, y poder cambiar la animación actual,
	se utilizará la clase derivada correspondiente.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CEntity 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CEntity(const std::string &name, const std::string &mesh);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CEntity();

		/**
		Cambia la posición y orientación de la entidad.

		@param transform Referencia a la matriz de transformación con la 
		que debe posicionarse y orientarse la entidad.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la posición de la entidad.

		@param position Nueva posición para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la orientación de la entidad.

		@param orientation Referencia a la matriz de rotación con la que debe 
		orientarse la entidad.
		*/
		void setOrientation(const Matrix3 &orientation);

		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad está puesta invisible, no debería dibujarse.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.
		 <p>
		 El método cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _entityNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		/**
		 Escala la entidad.

		 @param scale Valores de la escala en los diferentes ejes.
		 */
		void setScale(const Vector3 &scale);

		/**
		 Escala la entidad.

		 @param scale Valor de la escala para los 3 ejes.
		 */
		void setScale(float scale);

		/**
		 Devuelve el valor de la escala de la entidad.

		 @return Valores de la escala en los diferentes ejes.
		*/
		const Vector3 &getScale();

		/**
		Devuelve la entidad de Ogre.

		@return puntero a la entidad de Ogre.
		*/
		Ogre::Entity* getEntity() {return _entity;}

	protected:

		// CScene es la única que puede añadir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		Añade la entidad al SceneManager pasado por parámetro. Si la entidad
		no está cargada se fuerza su carga.

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
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);
		
		/**
		Nodo que contiene la entidad de Ogre.
		*/
		Ogre::SceneNode *_entityNode;

		/**
		Entidad de Ogre.
		*/
		Ogre::Entity *_entity;

		/**
		Quizás es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Nombre del modelo de la entidad.
		*/
		std::string _mesh;

		/**
		Indica si la entidad ha sido cargada en el motor gráfico.
		*/
		bool _loaded;

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
