/**
@file Server.h

Contiene la declaración de la clase CServer, Singleton que se encarga de
la gestión del audio en el juego.

@see Audio::CServer

@author Jose 
@date Marzo, 2013
*/
#ifndef __Audio_Server_H
#define __Audio_Server_H

#include "fmod.hpp"
#include "fmod_errors.h"
#include "BaseSubsystems/Math.h"

#include <iostream>
#include <string>
#include <map>

using namespace FMOD;

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CEntity;
}

// Declaración de la clase
namespace Audio
{
	/**
	Servidor del módulo Audio que se encarga de la gestión del audio del juego. Está 
	implementado como un singlenton de inicialización explícita. Sirve 
	para comunicar a FMOD los eventos de sonido que tenga que reproducir.

	@author Jose
	@date Marzo, 2013
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
		Se encarga de cargar un sonido (no 3D) y reproducirlo en modo normal.
		*/
		void playSound(char* rutaSonido, const std::string& id);

		/**
		Se encarga de cargar un sonido (no 3D) y reproducirlo en modo loop.
		*/
		void playLoopSound(char* rutaSonido, const std::string& id);

		/**
		Se encarga de cargar un sonido 3D y reproducirlo en modo normal.
		*/
		void playSound3D(char* rutaSonido, const std::string& id, Vector3 position);

		/**
		Se encarga de cargar un sonido 3D y reproducirlo en modo loop.
		*/
		void playLoopSound3D(char* rutaSonido, const std::string& id, Vector3 position);

		/**
		Se encarga de parar un sonido introduciendo su nombre como parámetro.

		@param id El identificador del sonido
		*/
		void stopSound(const std::string& id);

		/**
		Se encarga de parar todos los sonidos
		*/
		void stopAllSounds();

		/**
		Se encarga de cargar un sonido (no 3D) y reproducirlo en modo normal.
		*/
		void playStreamingSound(char* rutaSonido, const std::string& id);

		/**
		Se encarga de cargar un sonido (no 3D) y reproducirlo en modo loop.
		*/
		void playStreamingLoopSound(char* rutaSonido, const std::string& id);

		/**
		Establece el componente del jugador con el que preguntaremos la posición para actualizar la posición de escucha.

		@param controlledAvatar Componente al que le preguntaremos la posición de la entidad.
		*/
		void setSoundAvatar(Logic::CEntity *controlledAvatar) 
										{_soundAvatar = controlledAvatar;};

	protected:

		/**
		Constructor.
		*/
		CServer();

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
		Gestiona los errores de inicialización del sistema o carga/reproduccion de sonidos.
		*/
		void ERRCHECK(FMOD_RESULT result);

		/**
		Guarda las asociaciones de nombreSonido/Canal
		*/
		typedef std::map<std::string, Channel *> SoundChannelMap;
		SoundChannelMap _soundChannel;

	private:
		/**
		Única instancia de la clase.
		*/
		static CServer* _instance;

		/**
		Variable sistema de fmod.
		*/
		System* _system; // reminiscencias de C

		/**
		Factores doppler y rolloff del sistema
		*/
		float _doppler; 
		float _rolloff;

		/**
		Player(Protagonista) del audio.
		*/
		Logic::CEntity* _soundAvatar;
		short _playerHeight;

	}; // class CServer

} // namespace Audio

#endif // __Audio_Server_H
