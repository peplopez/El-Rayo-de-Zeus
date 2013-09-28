/**
@file Server.h

Contiene la declaración de la clase CServer, Singleton que se encarga de
la gestión de la lógica del juego.

@see Logic::CServer

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_Server_H
#define __Logic_Server_H

#include <string>
#include "Logic\Entity\LogicalPosition.h"


// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CMap;
	class CEntity;
	class CGameNetMsgManager;	
}

//PT
namespace CEGUI
{
	class EventArgs;
	class Window;
	class WindowManager;
	class ProgressBar;
}
//PT
namespace Graphics
{
	class Server;
}

/**
Namespace que engloba la lógica del juego. Engloba desde el mapa lógico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para más información ver @ref LogicGroup).

@author David Llansó
@date Agosto, 2010
*/
namespace Logic
{
	/**
	Servidor de la lógica que se se encarga de la creación del mundo, su 
	destrucción y actualización a lo largo de la partida. Se encarga de 
	inicializar el parseador de mapas y la factoría de entidades para cargar
	y gestionar el mapa de la partida. 

	@ingroup logicGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CServer
	{

	protected:

		typedef std::list<std::string> TMapNameList;

		typedef std::vector<std::string> TMapNameVector;
		
		typedef std::map<std::string, CMap*> TMaps;	

		typedef std::list<CEntity*> TEntityList;
		
		typedef std::map<int, TEntityList> TMapEntityLists;


	public:

		/**
		Devuelve la única instancia de la clase CServer.
		
		@return Única instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Llamará al método tick() del mapa.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Carga un nuevo nivel a partir del nombre del mapa que se
		desea cargar. Si ya existía un mapa este se borra, solo
		permitimos un nivel activo.

		@param filename Fichero que se desea cargar.
		@return true si la carga fue correcta.
		*/
		bool loadMap(const std::string &filename);

		/**
		Si hay un nivel cargado lo descarga  destruye.
		*/
		void unLoadMap(const std::string &filename);

		/**
		Función que activa el mapa en curso.

		@return true si la inicialización fue correcta.
		*/
		bool activateMap(const std::string &filename);

		/**
		Función que desactiva el mapa en curso.
		*/
		void deactivateMap(const std::string &filename);
		

		/**
		*/
		bool loadWorld(TMapNameList &mapList);

		/**
		*/
		void unLoadWorld();

		/**
		*/
		bool activateAllMaps();

		/**
		*/
		void deactivateAllMaps();

		/**
		Para inicializar las estructuras que contienen las posiciones de los anillos
		*/
		virtual bool setRingPositions();

		/**
		Para obtener un Vector3 con la posición del anillo
		*/
		virtual Vector3 getRingPosition(LogicalPosition::Ring ring);

		/**
		Para obtener un float con el radio de un anillo pasado por parámetro
		*/		
		virtual float getRingRadio(LogicalPosition::Ring ring);

		/**
		Devuelve el mapa lógico del juego.

		@return Mapa con las entidades de juego.
		*/
		CMap *getMap(const std::string mapName);

		/**
		Devuelve la entidad del jugador.

		@return Entidad del jugador.
		*/
		CEntity *getPlayer() {return _player;}

		/**
		Establece la entidad del jugador.

		@param player Entidad del jugador.
		*/
		void setPlayer(CEntity *player) {_player = player;}

		/**
		*/
		void deferredMoveEntity(CEntity *entity,const int targetMap);

		/**
		*/
		void moveDefferedEntities();

		/**
		*/
		void activateBaseCam(const int targetMap);
		
		/**
		*/
		void activatePlayerCam();

		/**
		*/
		void compositorEnable(const std::string &name);
		void compositorDisable(const std::string &name);

		std::ostringstream  getBasestring(int base);
	protected:
		/**
		Constructor.
		*/
		CServer ();

		/**
		Destructor.
		*/
		~CServer();

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

		/**
		Mapa donde se encuentran todas las entidades lógicas.
		*/
		//CMap *_map;

		/**
		*/
		TMaps _maps;

		/**
		*/
		TMapNameVector _mapNames;

		/**
		*/
		TMapEntityLists _entitiesToMove;

		/**
		Entidad del jugador.
		*/
		CEntity *_player;

		/**
		Gestor de los mensajes que llegan por la red
		*/
		Logic::CGameNetMsgManager* _gameNetMsgManager;


	private:
		/**
		Única instancia de la clase.
		*/
		static CServer* _instance;

		bool _worldIsLoaded;

	}; // class CServer

} // namespace Logic

#endif // __Logic_Server_H
