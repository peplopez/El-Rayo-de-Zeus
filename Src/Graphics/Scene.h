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

#include <BaseSubsystems/Math.h>
#include <OgreFrameListener.h>

#include <list>

// Predeclaración de clases para ahorrar tiempo de compilación
class IHHFXScene;
struct SContactReport;

namespace Ogre 
{
	class Root;
	class Viewport;
	class SceneManager;
	class StaticGeometry;	
};


namespace Graphics 
{
	class CServer;
	class CCamera;
	class CEntity;
	class CSceneElement;
	class CStaticEntity;
	class CBillboard;
	class CLight;
};

namespace Physics
{
	class CScene;
};

namespace Logic
{
	class CDotSceneLoader;
};

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
	class CScene  :  public Ogre::FrameListener
	{


	public:

		/**
		Devuelve la cámara de la escena.

		@return Puntero a la cámara de la escena.
		*/
		CCamera *getCamera() {return _camera;}

		CCamera *getBaseCamera() {return _baseCamera;}

		

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
		bool add(CEntity* entity);

		/**
		Elimina una entidad gráfica de la escena. 
		<p>
		@remarks Este método NO destruye la entidad, ésta solo deja de
		ser parte de la escena.

		@param entity Entidad gráfica que se quiere eliminar de la escena.
		*/
		void remove(CEntity* entity);
	
		/**
		*/
		bool add(CLight* light);

		/**
		*/
		void remove(CLight* light);


		//---------- GENERIC SCENE ELEMENTS (p.e. billboards)-----------

		bool add(CSceneElement* sceneElement);
		void remove(CSceneElement* sceneElement);

		// PARTICLES
		void createParticleSystem(const std::string& templateName, const std::string& parentEntity); // pos relativa a otra entidad
		void createParticleSystem(const std::string& templateName, const Vector3& position); // pos absoluta
		
		/**PeP
		Activar compositor, pensado para el de blanco y negro. También RadialBlur
		*/
	
		void activateCompositor(std::string name);

		/**PeP
		Desactivar compositor, pensado para el de blanco y negro sobre todo. También RadialBlur
		*/
		void deactivateCompositor(std::string name);


		
	protected:

		/**
		Clase amiga. Solo el servidor gráfico puede crear o liberar escenas, 
		activarlas o desactivarlas y actualizar su estado. Solo las entidades,, billboards y
		la cámara pueden acceder al gestor de la escena de Ogre.
		*/
		friend class CServer;	
		friend class CCamera;
		friend class CSceneElement;
		friend class CLight;
		friend class Logic::CDotSceneLoader; // HACK FRS Logic???
		friend class Physics::CScene; // FRS esto tiene que ser asin?

		/**
		Nombre de la escena.
		*/
		std::string _name;
		
		/**
		Punto de entrada al sistema Ogre.
		*/
		Ogre::Root *_root;

		/**
		Camara desde la que se verá la escena. Puede haber cámaras más
		sofisticadas y más tipos de cámaras desde el punto de vista lógico,
		ellas se encargarán de mover esta instancia.
		*/
		CCamera *_camera;

		/**
		*/
		CCamera *_baseCamera;

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

	


		/***************
		 SCENE ELEMENTS
		******************/
		
		/**
		Tipos para la lista de entidades.
		*/
		typedef std::list<CEntity*> TEntities;
	
		/**
		Tipos para la lista de luces.
		*/
		typedef std::list<CLight*> TLights;


		/**
		Lista de entidades dinámicas.
		*/
		TEntities _dynamicEntities; // FRS Necesario para ejecutarles el tick

		/**
		Lista de entidades estáticas.
		*/
		TEntities _staticEntities; // FRS Necesario para aglomerar geom estática
			
		
		/** 
		Lista de luces en la escena 
		*/
		TLights _lights;

		/**
		Geometría estática de la escena.
		*/
		Ogre::StaticGeometry *_staticGeometry;


		//---------------------------------------------------------------


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
		*/
		void activateBaseCam();

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
		Devuelve el gestor de la escena de Ogre
		@return Puntero al gestor de la escena de Ogre.
		*/
		Ogre::SceneManager *getSceneMgr() { return _sceneMgr; }

		/**
		Devuelve la geometría estática de la escena de Ogre
		@return Puntero a la geometría estática de la escena de Ogre.
		*/
		Ogre::StaticGeometry *getStaticGeometry() { return _staticGeometry; }
		



	/*********************
		HELL HEAVENS FX
	*********************/

	public:		

		// WARNING : if != 1.0, This scale must be taken into account when setting and getting arbitrary particle attributes !
		static const float HHFX_WORLD_SCALE;

		IHHFXScene*	getHHFXScene() const { assert(_hhfxScene); return _hhfxScene; }
		
		//	FRAME LISTENER 
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		
	private:	

		static const int _HHFX_UPDATE_TIME_MAX = 1000;
		Ogre::Real _hhfxTimeSinceUpdate;
		IHHFXScene *_hhfxScene;

		void _hhfxSceneInit();
		void _hhfxSceneDeinit();
		void _hhfxCompositorLoad();
		void _hhfxCompositorUnload();
		static bool _hhfxCollisionCheck(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport);

	}; // class CScene

} // namespace Graphics

#endif // __Graphics_Scene_H
