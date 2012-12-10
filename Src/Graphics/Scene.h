//---------------------------------------------------------------------------
// Scene.h
//---------------------------------------------------------------------------

/**
@file Scene.h

Contiene la declaración de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_Scene_H
#define __Graphics_Scene_H

#include <list>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Root;
	class Viewport;
	class SceneManager;
	class StaticGeometry;
	class Light;
}
namespace Graphics 
{
	class CServer;
	class CCamera;
	class CEntity;
	class CStaticEntity;
}

namespace Graphics 
{
	/**
	Clase que controla todos los elementos de una escena. Puede ser de 
	un estado como el menú o un nivel del juego. Su equivalente en la 
	lógica del juego en el caso de ser un nivel sería el mapa. la escena 
	es un contenedor de todas las entidades gráficas que pueden ser 
	reenderizadas por un viewport asociado a esa escena. Se asume que una
	escena solo va a ser reenderizada desde un punto de vista por lo que
	por defecto se crea una única cámara y cuando se activa la escena se
	crea un único viewport mostrado en la totalidad de la ventana de 
	reenderizado. En función del tipo de juego puede quererse hacer más de
	un reenderizado de la misma escena a la vez y mostrarlo en diferentes 
	viewports en la ventana. Por ejemplo en un juego de coches para mostrar
	la imagen de los retrovisores del coche.
	<p>
	Trás crear la escena se deben añadir las diferentes entidades y 
	entidades estáticas. Al añadir las entidades a la escena se fuerza su
	carga. Las entidades estáticas se almacenan en una estructura. Una vez 
	añadidas todas las entidades se debe crear la geometría estática ya 
	que hasta que no se invoque a buildStaticGeometry() las entidades no
	pasarán a formar parte de la geometría estática. Al activar la escena
	se fuerza la creación de la geometría estática si no se había creado.
	<p>
	@remarks Una vez activada la escena las modificaciones realizadas en
	los valores de las entidades estáticas no tendrán efecto.
	<p>
	@remarks Solo el servidor gráfico (Graphics::CServer) puede crear o 
	liberar escenas.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CScene 
	{
	public:

		/**
		Devuelve la cámara de la escena.

		@return Puntero a la cámara de la escena.
		*/
		CCamera *getCamera() {return _camera;}

		/**
		Devuelve el nombre de la escena.

		@return Nombre de la escena.
		*/
		const std::string& getName() {return _name;}

		/**
		Añade una entidad gráfica a la escena.
		<p>
		@remarks La escena NO se hace responsable de destruir la
		entidad.

		@param entity Entidad gráfica que se quiere añadir a la escena.
		@return Cierto si la entidad se añadió y cargó correctamente.
		*/
		bool addEntity(CEntity* entity);

		/**
		Añade una entidad gráfica estática a la escena. No sirve
		addEntity porque estas entidades deben ser almacenadas en otra
		lista para cuando se cree la geometría estática de la escena poder 
		recuperarlas y cargarlas.
		<p>
		@remarks La escena NO se hace responsable de destruir la
		entidad.

		@param entity Entidad gráfica que se quiere añadir a la escena.
		@return Cierto si la entidad se añadió y cargó correctamente.
		*/
		bool addStaticEntity(CStaticEntity* entity);

		/**
		Elimina una entidad gráfica de la escena. 
		<p>
		@remarks Este método NO destrulle la entidad, ésta solo deja de
		ser parte de la escena.

		@param entity Entidad gráfica que se quiere eliminar de la escena.
		*/
		void removeEntity(CEntity* entity);

		/**
		Elimina una entidad gráfica estática de la escena. 
		<p>
		@remarks Este método NO destrulle la entidad, ésta solo deja de
		ser parte de la escena.

		@param entity Entidad gráfica estática que se quiere eliminar de 
		la escena.
		*/
		void removeStaticEntity(CStaticEntity* entity);

	protected:

		/**
		Clase amiga. Solo el servidor gráfico puede crear o liberar escenas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**
		Constructor de la clase.
		*/
		CScene(const std::string& name);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CScene();

		/**
		Despierta la escena y crea un viewport que ocupa toda la
		pantalla.
		*/
		void activate();

		/**
		Duerme la escena y destruye su viewport para que no se siga 
		reenderizando.
		*/
		void deactivate();
		
		/**
		Actualiza el estado de la escena cada ciclo. Llama a su vez a 
		todas las entidades.
		
		@param secs Número de segundos transcurridos desde la última 
		llamada.
		*/
		void tick(float secs);

		/**
		Añade las entidades estáticas a la geometría estática del nivel
		y la construlle. Si la geometría estática ya ha sido construida
		no se vuelve a construir.
		<p>
		@remarks Una vez construida la geometría estática no se pueden 
		modificar los valores de las entidades estáticas.
		*/
		void buildStaticGeometry();

		/**
		Clase amiga. Solo las entidades y la cámara pueden acceder al 
		gestor de la escena de Ogre.
		*/
		friend class CEntity;
		friend class CCamera;

		/**
		Devuelve el gestor de la escena de Ogre

		@return Puntero al gestor de la escena de Ogre.
		*/
		Ogre::SceneManager *getSceneMgr() {return _sceneMgr;}


		/**
		Clase amiga. Solo las entidades pueden acceder al gestor de la
		escena de Ogre.
		*/
		friend class CStaticEntity;

		/**
		Devuelve la geometría estática de la escena de Ogre

		@return Puntero a la geometría estática de la escena de Ogre.
		*/
		Ogre::StaticGeometry *getStaticGeometry() {return _staticGeometry;}
		
		/**
		Nombre de la escena.
		*/
		std::string _name;
		
		/**
		Punto de entrada al sistema Ogre.
		*/
		Ogre::Root *_root;

		/** 
		Marco en la ventana de reenderizado donde se pinta lo captado por
		una cámara. Solo puede haber una cámara asociada a un viewport,
		sin embargo una ventana de reenderizado puede tener diferentes
		viewports al mismo tiempo.
		*/
		Ogre::Viewport *_viewport;
		
		/**
		Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		Ogre::SceneManager *_sceneMgr;
		
		/**
		Luz direccional que se crea por defecto en la escena. Gráficamente
		mejora la escena bastante respecto a tener solo luz ambiente ya que
		se ven mejor los volúmenes de las entidades.
		*/
		Ogre::Light *_directionalLight;

		/**
		Camara desde la que se verá la escena. Puede haber cámaras más
		sofisticadas y más tipos de cámaras desde el punto de vista lógico,
		ellas se encargarán de mover esta instancia.
		*/
		CCamera *_camera;

		/**
		Tipos para la lista de entidades.
		*/
		typedef std::list<CStaticEntity*> TStaticEntityList;

		/**
		Tipos para la lista de entidades.
		*/
		typedef std::list<CEntity*> TEntityList;

		/**
		Lista de entidades estáticas.
		*/
		TStaticEntityList _staticEntities;

		/**
		Lista de entidades dinámicas.
		*/
		TEntityList _dynamicEntities;
		
		/**
		Geometría estática de la escena.
		*/
		Ogre::StaticGeometry *_staticGeometry;

	}; // class CScene

} // namespace Graphics

#endif // __Graphics_Scene_H
