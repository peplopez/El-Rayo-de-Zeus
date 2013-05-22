//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

/**
@file Server.cpp

Contiene la implementación de la clase principal de audio, llevará el control de los sonidos en el juego.

@see Audio::CServer

@author Jose 
@date Marzo, 2013
*/

#include "Server.h"
#include "Logic/Entity/Entity.h"

#include <cassert>
#include <iostream>

namespace Audio
{
	CServer *CServer::_instance = 0;

	CServer::CServer()
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");
		_volume=0.5f;
		_doppler=1.0f;
		_rolloff=1.0f;
		_soundAvatar=NULL;
		_playerHeight=8;
		_isMute=false;
		_instance = this;
		_minimumExecuteTime=100;
		_timeToExecute=0;
	} // CServer

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CServer

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		FMOD_RESULT result;
		//Creamos
		result = System_Create(&_system);
		ERRCHECK(result);
			
		//Iniciamos
		result = _system->init(32, FMOD_INIT_3D_RIGHTHANDED, 0);
		ERRCHECK(result);
		
		//Configuración 3D, el parámetro central es el factor de distancia (FMOD trabaja en metros/segundos)
		_system->set3DSettings(_doppler,1.0f,_rolloff);

		//Cargamos la banda sonora del juego
		playLoopSound("media/audio/playa_1.mp3", "theme");

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		stopAllSounds();
		_soundAvatar=NULL;
		_system->release();

	} // close

	//--------------------------------------------------------

	void CServer::tick(unsigned int msecs) 
	{
		_timeToExecute+=msecs;
		//Si ha pasado el tiempo mínimo para actualizarnos lo hacemos

		//El tick se ejecuta ahora mismo cada 0.001secs (controlado en el nivel superior)
		//Si hay un player con el que actualizarnos, seteamos la nueva posición
		if(_timeToExecute>=_minimumExecuteTime)
		{
				//Reseteamos el tiempo
				_timeToExecute=0;

			if(_soundAvatar){
				Vector3 positionAvatar=_soundAvatar->getPosition();
				Vector3 directionAvatar=Math::getDirection(_soundAvatar->getOrientation());
				directionAvatar.normalise();

				FMOD_VECTOR
					listenerPos = {positionAvatar.x,positionAvatar.y+_playerHeight,positionAvatar.z}, // posición del listener
					listenerVel = {0.0f,0.0f,0.0f}, // velocidad
					up = {0.0f,1.0f,0.0f},          // vector up: hacia la “coronilla”
					at = {directionAvatar.x,directionAvatar.y,directionAvatar.z};          // vector at: hacia donde se mira

				// se coloca el listener
				_system->set3DListenerAttributes(0,&listenerPos, &listenerVel,
										 &at, &up);
			}

		//Actualizamos el sistema
		_system->update();
		}
	} // tick
	//--------------------------------------------------------

	// función para dar salida de error y terminar aplicación
	void CServer::ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK){
			std::cout << "FMOD error! " << result << std::endl <<FMOD_ErrorString(result);
			exit(-1);
		}
	}//ERRCHECK
	//--------------------------------------------------------

	void CServer::playSound(char* rutaSonido, const std::string& id, bool notIfPlay){
		//Si queremos que suene solamente si no esta sonando ya
		if(notIfPlay){
			bool isPlaying;
			_soundChannel[id]->isPlaying(&isPlaying);
			if(isPlaying){
				return;
			}
		}
		
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result = _system->createSound(
		rutaSonido, // path del archivo de sonido
		FMOD_DEFAULT, // flags
		0, // información adicional (nada en este caso)
		& sound); // devolución del handle al buffer
		ERRCHECK(result);

		//Reproducción en canal
		Channel *canal;
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		result = canal->setVolume(_volume);
		ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;

		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;

	}//playSound
	//--------------------------------------------------------

	void CServer::playLoopSound(char* rutaSonido, const std::string& id){
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result = _system->createSound(
		rutaSonido, // path del archivo de sonido
		FMOD_DEFAULT | FMOD_LOOP_NORMAL, // flags
		0, // información adicional (nada en este caso)
		& sound); // devolución del handle al buffer
		ERRCHECK(result);
		//sound->setLoopCount(-1); // sonido a loop asi le iba a david
			
		//Reproducción en canal
		Channel *canal;
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		
		result = canal->setVolume(_volume);
		ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;


		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;

	}//playLoopSound
	//--------------------------------------------------------

	void CServer::playSound3D(char* rutaSonido, const std::string& id,Vector3 position, bool notIfPlay){
		
		//Si queremos que suena solamente si no esta sonando ya
		if(notIfPlay){
			bool isPlaying;
			_soundChannel[id]->isPlaying(&isPlaying);
			if(isPlaying){
				return;
			}
		}
		
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result = _system->createSound(
		rutaSonido, // path del archivo de sonido
		FMOD_3D, // flags
		0, // información adicional (nada en este caso)
		& sound); // devolución del handle al buffer
		ERRCHECK(result);

		//Reproducción en canal
		Channel *canal;
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		
		result = canal->setVolume(_volume);
		ERRCHECK(result);

		FMOD_VECTOR
			pos={position.x,position.y,position.z}, // posición
			vel={0,0,0};  // velocidad (para el doppler)
			canal->set3DAttributes(& pos, & vel);
			ERRCHECK(result);

		/*int can;
		canal->getIndex(&can);
		std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;*/

		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;
	}//playSound3D
	//--------------------------------------------------------

	void CServer::playLoopSound3D(char* rutaSonido, const std::string& id, Vector3 position){
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result = _system->createSound(
		rutaSonido, // path del archivo de sonido
		FMOD_3D | FMOD_LOOP_NORMAL, // flags
		0, // información adicional (nada en este caso)
		& sound); // devolución del handle al buffer
		ERRCHECK(result);
		//sound->setLoopCount(-1); // sonido a loop asi le iba a david
			
		//Reproducción en canal
		Channel *canal;
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		
		result = canal->setVolume(_volume);
		ERRCHECK(result);

		FMOD_VECTOR
			pos={position.x,position.y,position.z}, // posición
			vel={0,0,0};  // velocidad (para el doppler)
			canal->set3DAttributes(& pos, & vel);
			ERRCHECK(result);

		//Distancia a la que empieza a atenuarse y a la cual ya no se atenua mas respectivamente
		result = canal->set3DMinMaxDistance(50.0f,10000.0f);
		ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		//std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		//std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;


		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;
	}//playLoopSound3D
	//--------------------------------------------------------

	void CServer::stopSound(const std::string& id){
		SoundChannelMap::const_iterator itMap = _soundChannel.find(id);

		if (itMap != _soundChannel.end()){
			itMap->second->stop();
			_soundChannel.erase(id);
		}

			
	}//stopSound
	//--------------------------------------------------------


	void CServer::stopAllSounds(){
		Channel *canal;
		//Recorremos los 32 canales y paramos su reproduccion
		for(int i=0;i<32;i++){
				_system->getChannel(i,&canal);
				canal->stop();
			}
		//Limpiamos el mapa idSonido-canal
		_soundChannel.clear();
	}//stopAllSounds
	//--------------------------------------------------------

	void CServer::playStreamingSound(char* rutaSonido, const std::string& id){
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result = _system->createStream(
		rutaSonido, // path del archivo de sonido
		FMOD_DEFAULT, // flags
		0, // información adicional (nada en este caso)
		& sound); // devolución del handle al buffer
		ERRCHECK(result);

		//Reproducción en canal
		Channel *canal;
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		
		result = canal->setVolume(_volume);
		ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		//std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		//std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;

		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;

	}//playSound
	//--------------------------------------------------------

	void CServer::playStreamingLoopSound(char* rutaSonido, const std::string& id){
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result = _system->createStream(
		rutaSonido, // path del archivo de sonido
		FMOD_DEFAULT | FMOD_LOOP_NORMAL, // flags
		0, // información adicional (nada en este caso)
		& sound); // devolución del handle al buffer
		ERRCHECK(result);
		//sound->setLoopCount(-1); // sonido a loop asi le iba a david
			
		//Reproducción en canal
		Channel *canal;
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		result = canal->setVolume(_volume);
		ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		//std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		//std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;


		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;

	}//playLoopSound
	//--------------------------------------------------------


} // namespace Audio