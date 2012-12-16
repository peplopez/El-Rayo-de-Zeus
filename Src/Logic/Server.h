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

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CMap;
	class CEntity;
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
		Función que activa el mapa en curso.

		@return true si la inicialización fue correcta.
		*/
		bool activateMap();

		/**
		Función que desactiva el mapa en curso.
		*/
		void deactivateMap();

		/**
		Devuelve el mapa lógico del juego.

		@return Mapa con las entidades de juego.
		*/
		CMap *getMap() {return _map;}

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
		Carga un nuevo nivel a partir del nombre del mapa que se
		desea cargar. Si ya existía un mapa este se borra, solo
		permitimos un nivel activo.

		@param filename Fichero que se desea cargar.
		@return true si la carga fue correcta.
		*/
		bool loadLevel(const std::string &filename);

		/**
		Si hay un nivel cargado lo descarga  destruye.
		*/
		void unLoadLevel();

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
		CMap *_map;

		/**
		Entidad del jugador.
		*/
		CEntity *_player;

	private:
		/**
		Única instancia de la clase.
		*/
		static CServer* _instance;

	}; // class CServer

} // namespace Logic

#endif // __Logic_Server_H
