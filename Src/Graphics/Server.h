//---------------------------------------------------------------------------
// Server.h
//---------------------------------------------------------------------------

/**
@file Server.h

Contiene la declaración de la clase principal de gráficos, la que permite crear
la ventana, etc.

@see Graphics::CServer

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_Server_H
#define __Graphics_Server_H

#include <assert.h>
#include <map>
#include <OgreCompositorManager.h>

// Predeclaración de clases para ahorrar tiempo de compilación

class IHHFXBase;

namespace Ogre 
{
	class Root;
	class RenderWindow;
	class Timer;
	class OverlayManager;
	class Viewport;
}
namespace Graphics 
{
	class CScene;
	class COverlay;
}

	


/**
Namespace con todo lo que se refiere a la parte gráfica. Las diferentes clases
se apoyan en Ogre.
(para más información ver @ref graphicsGroup).

@author David Llansó
@date Julio, 2010
*/
namespace Graphics 
{
	/**
	Clase central del subistema gráfico. Se encarga de gestionar todas las
	entidades que se pintan en la ventana de la aplicación.
	<p>
	Está implementado como un Singleton de inicialización explícita; se debe 
	llamar a su método Init(), para que la instancia esté creada. Al final de 
	la aplicación, habrá que llamar a Release().
	<p>
	Los detalles de dibujado están implementados haciendo uso de Ogre.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CServer 
	{
	protected:

		/**
		Tipo para la estructura que guarda las diferentes escenas
		*/
		typedef std::map<std::string,CScene*> TScenes;

	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor gráfico.
		*/
		static Graphics::CServer *getSingletonPtr() { assert(_instance && "Servidor Grafico no inicializado"); return _instance; }
		/**
		Inicializa el servidor gráfico. Dependiendo de la configuración (release
		o debug), utiliza pantalla completa o no.
		<p>
		Por conveniencia, el Init crea la ventana <b>minimizada</b>, por lo
		que la aplicación después deberá llamar a ShowWindow(). Se hace así
		para que la aplicación no muestre una ventana en negro mientras dura
		todo el proceso de carga. La ventana está creada y el usuario la verá,
		pero al menos la ve minimizada.

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor gráfico. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Función invocada en cada vuelta para actualización.

		@param secs Segundos desde que se reenderizó el último frame.
		*/
		void tick(float secs);

		/**
		Crea una nueva escena. Puede ser de un estado como el menú o un
		nivel del juego. Su equivalente en la lógica del juego en el caso
		de ser un nivel sería el mapa. la escena es un contenedor de todas
		las entidades gráficas que pueden ser reenderizadas por un viewport
		asociado a esa escena. 
		<p>
		@remarks El servidor gráfico SE HACE RESPONSABLE de
		la liberación de la escena al terminar la aplicación. No obstante
		se puede liberar antes mediante removeScene(). Si el motor gráfico 
		se destruye antes del objeto que creó la escena, este objeto dejará
		de tener la escena accesible.
		<p>
		@remarks En depuración se garantiza que no se creen escenas con el 
		mismo nombre.

		@param name Nombre que se le quiere dar a la escena. En depuración 
		se garantiza que no se creen escenas con el mismo nombre.
		@return Escena creada.
		*/
		CScene* createScene(const std::string& name);

		/**
		Recupera una escena previamente creada a partir de su nombre. 

		@param name Nombre de la escena que se quiere recuperar.
		@return La escena requerida.
		*/
		CScene* getScene(const std::string& name) {return (*_scenes.find(name)).second;}

		/**
		Devuelve la escena activa. 

		@return La escena activa.
		*/
		CScene* getVisibleScene() {return _visibleScene;}

		/**
		Elimina la escena pasada por parámetro de la tabla de escenas
		y la libera.

		@param scene Escena que se desea liberar
		*/
		void removeScene(CScene* scene);

		/**
		Elimina la escena de nombre especificado por parámetro de la 
		tabla de escenas y la libera.

		@param name Nombre de la escena que se quiere liberar.
		*/
		void removeScene(const std::string& name);

		/**
		Inicializa la escena
		*/
		void activate(CScene* scene);

		/**
		Deinicializa la escena
		*/
		void deactivate(CScene* scene);

		/**
		*/
		void activatePlayerCam(CScene* scene);

		/**
		*/
		void activateBaseCam(CScene* scene);

		/**
		Devuelve al manager de Overlays. 

		@return La escena activa.
		*/
		Ogre::OverlayManager* getOverlayManager() {return _overlayManager;}			

	
		/**
		Metodo que devuelve el height de la pantalla. Esta tomado gracias a los overlays, puede que no sea fiable 100%

		@return devuelve el width de la pantalla
		*/
		int getScreenWidth();

		/**
		Metodo que devuelve el height de la pantalla. Esta tomado gracias a los overlays, puede que no sea fiable 100%

		@return devuelve el height de la pantalla
		*/
		int getScreenHeight();

		/**
		*/
		Ogre::Viewport* getViewport() { return _viewport; }


	protected:

		/**
		Instancia única de la aplicación.
		*/
		static CServer *_instance;

		/**
		Mapa de escenas. Se asocia una escena con su nombre.
		*/
		TScenes _scenes;

		/**
		Escena actual. Por simplificación asumimos que solo va a haber una
		escena activa al mismo tiempo. El cambio de escena activa se realiza
		a través de ésta clase.
		*/
		CScene* _visibleScene;

		/**
		Escena dummy que se crea automáticamente. Con ella permitimos que
		siempre haya una escena para el dibujado del GUI.
		*/
		CScene* _dummyScene;

		/**
		Punto de entrada al sistema Ogre.
		*/
		Ogre::Root *_root;

		/** 
		Ventana de renderizado 
		*/
		Ogre::RenderWindow *_renderWindow;
		
		/**
		Marco común de la ventana de renderizado para todas las escenas
		*/
		Ogre::Viewport *_viewport;

		/**
		Manager de los Overlays
		*/
		Ogre::OverlayManager* _overlayManager;

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor del server gráfico.
		*/
		virtual ~CServer();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();


	/*********************
		HELL HEAVEN FX
	********************/	

	private:		
		// DICCIONARIO "short hfx name" (Blast.hfx) => "long hfx name" (/HBO/Entities/Particles/Blast.hfx)
		typedef std::map<std::string, std::string> THFXNamesMap;
			THFXNamesMap _HFX_LONG_NAMES;

	public:
		IHHFXBase* getHHFXBase() const { 
			assert(_hhfxBase && "HHFX No ha sido inicializado correctamente"); 
			return _hhfxBase; 
		}	
		const std::string& CServer::getHFXLongName(const std::string& hfxShortName) { 
			assert( !_HFX_LONG_NAMES[ hfxShortName ].empty() && "Nombre de script HFX incorrecto o no incluido en el paquete precargado");
			return _HFX_LONG_NAMES[ hfxShortName ]; 
		}
		
	private:			

		IHHFXBase* _hhfxBase;
		void _initHHFX(CScene*);
		void _preloadHHFXTextures();	

	/*******************
		COMPOSITORS
	*******************/
	public:
		void compositorEnable(const std::string &name) {	assert(_viewport && "La escena no está activa");
			Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, name, true); }	
		void compositorDisable(const std::string &name)	{	assert(_viewport && "La escena no está activa");
			Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, name, false); }	
	
	private:
		void _compositorReenable(const std::string &name) {	compositorDisable(name); compositorEnable(name); }
		void _resetCompositors();
		void _compositorLoad();
		void _compositorAdd(const std::string &name) {	
			assert(_viewport && "La escena no está activa");
			Ogre::CompositorInstance* comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, name); 
			assert(comp && "Error al cargar compositor. Revisar que esta bien definido en los assets" );
		}

	}; // class CServer

} // namespace Graphics

#endif // __Graphics_Server_H
