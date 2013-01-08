/**
@file Manager.h

Contiene la declaración de la clase CManager, Singleton que se encarga de
la gestión de la red del juego.

@see Net::CManager

@author David Llansó
@date Diciembre, 2010
*/
#ifndef __Net_Manager_H
#define __Net_Manager_H

#include <vector>

// Predeclaracion de clases
namespace Net {
	class CBuffer;
	class CServidor;
	class CCliente;
	class CConexion;
	class CFactoriaRed;
	class CFactoriaRedEnet;
	class CPaquete;
};

/**
Namespace que engloba lo relacionado con la parte de red.
(para más información ver @ref NetGroup).

@author David Llansó
@date Diciembre, 2010
*/
namespace Net
{
	enum NetMessageType {
		COMMAND,
		START_GAME,
		END_GAME,
		LOAD_MAP,
		MAP_LOADED,
		ENTITY_MSG
	};



	/**
	Gestor de la red. Sirve como interfaz para que el resto de los
	proyectos interactúen con la red y no tengan que preocuparse de 
	la tecnología específica usada para su implementación.
	<p>
	Esta clase debe ser específicamente inicializada o configurada
	como cliente <em>o</em> como servidor. Sin embargo, desde fuera
	<em>puede usarse sin conocer su configuración</em> y así invocar 
	métodos como CManager::tick() para que compruebe si se ha recibido 
	algún mensaje o invocar a CManager::send() cuando se quiera enviar 
	un mensaje al otro extremo de la conexión.

	@ingroup NetGroup

	@author David Llansó
	@date Diciembre, 2010
	*/
	class CManager
	{
	public:

		/**
		Interfaz que deben implementar las clases que quieran ser 
		notificadas de eventos de red.
		*/
		class IObserver{
		public:
			virtual void dataPacketReceived(Net::CPaquete* packet)=0;
			virtual void connexionPacketReceived(Net::CPaquete* packet)=0;
			virtual void disconnexionPacketReceived(Net::CPaquete* packet)=0;
		};

		/**
		Devuelve la única instancia de la clase CManager.
		
		@return Única instancia de la clase CManager.
		*/
		static CManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CManager. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Será aquí donde consultaremos si han llegado mensajes y de ser así 
		los procesaremos o delegaremos su proceso al módulo correspondiente.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Función que sirve para enviar datos al otro lado de la conexión.
		Si se está en modo cliente los datos se enviarán al servidor
		mientras que si se encuentra en modo servidor la información se
		enviará a todos los clientes registrados.

		@param data Datos a enviar.
		*/
		void send(void* data, size_t longdata);

		void activateAsServer(int port, int clients = 16, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

		void activateAsClient(unsigned int maxConnections = 1, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

		void connectTo(char* address, int port, int channels = 1, unsigned int timeout = 5000);

		void disconnect();

		void deactivateNetwork();


		void addObserver(IObserver*);

		void removeObserver(IObserver*);

	protected:
		/**
		Constructor.
		*/
		CManager ();

		/**
		Destructor.
		*/
		~CManager();

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

		void getPackets(std::vector<Net::CPaquete*>& _paquetes);

	private:
		/**
		Única instancia de la clase.
		*/
		static CManager* _instance;

		/**
		Factoría de objetos de red
		*/
		Net::CFactoriaRed* _factoriaRed;

		/**
			 Servidor de red
		*/
		Net::CServidor* _servidorRed;

		/**
			Cliente de red
		*/
		Net::CCliente* _clienteRed;

		/**
			Conexion cliente de red. Es decir, el servidor visto desde el cliente.
			En un juego avanzado habría más de una si estamos en p2p.
		*/
		Net::CConexion* _conexion;

		std::vector<IObserver*> _observers;

		std::vector<Net::CPaquete*> _paquetes;

		}; // class CManager

} // namespace Net

#endif // __Net_Manager_H
