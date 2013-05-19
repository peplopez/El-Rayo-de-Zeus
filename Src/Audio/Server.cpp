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

#include <cassert>
#include <iostream>


#include "Logic\Entity\Entity.h"


namespace Audio
{
	CServer *CServer::_instance = 0;

	CServer::CServer()
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");
		_doppler=0.00000001f;
		_rolloff=0.00000001f;
		_soundAvatar=NULL;
		_playerHeight=8;
		_instance = this;

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
		result = _system->init(100, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);
		
		//Configuración 3D, el parámetro central es el factor de distancia (FMOD trabaja en metros/segundos)
		_system->set3DSettings(_doppler,1.0,_rolloff);

		//Cargamos la banda sonora del juego
		playLoopSound("media/audio/playa_1.mp3", "theme");

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		_system->release();

	} // close

	//--------------------------------------------------------

	void CServer::tick(unsigned int secs) 
	{
		//El tick se ejecuta ahora mismo cada 0.001secs (controlado en el nivel superior)
		//Si hay un player con el que actualizarnos, seteamos la nueva posición
		if(_soundAvatar){
			Vector3 positionAvatar=_soundAvatar->getPosition();
			Vector3 directionAvatar=Math::getDirection(_soundAvatar->getOrientation());

			FMOD_VECTOR
				listenerPos = {positionAvatar.x,positionAvatar.y+_playerHeight,positionAvatar.z}, // posición del listener
				listenerVel = {0,0,0}, // velocidad
				up = {0,1,0},          // vector up: hacia la “coronilla”
				at = {directionAvatar.x,directionAvatar.y,directionAvatar.z};          // vector at: hacia donde se mira

			// se coloca el listener
			_system->set3DListenerAttributes(0,&listenerPos, &listenerVel,
                                     &up, &at);
		}


		//Actualizamos el sistema
		_system->update();
	} // tick
	//--------------------------------------------------------

	// función para dar salida de error y terminar aplicación
	void CServer::ERRCHECK(FMOD_RESULT result){
		if (result != FMOD_OK){
		std::cout << "FMOD error! " << result << std::endl <<FMOD_ErrorString(result);
		exit(-1);
		}
	}//ERRCHECK
	//--------------------------------------------------------

	void CServer::playSound(char* rutaSonido, const std::string& id){
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
		float volume=0.7f; // valor entre 0 y 1
		result = canal->setVolume(volume);
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
		float volume=0.7f; // valor entre 0 y 1
		result = canal->setVolume(volume);
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

	void CServer::playSound3D(char* rutaSonido, const std::string& id, Vector3 position){
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
		float volume=0.7f; // valor entre 0 y 1
		result = canal->setVolume(volume);
		ERRCHECK(result);

		FMOD_VECTOR
			pos={position.x,position.y,position.z}, // posición
			vel={0,0,0};  // velocidad (para el doppler)
			canal->set3DAttributes(& pos, & vel);
			ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;

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
		float volume=0.7f; // valor entre 0 y 1
		result = canal->setVolume(volume);
		ERRCHECK(result);

		FMOD_VECTOR
			pos={position.x,position.y,position.z}, // posición
			vel={0,0,0};  // velocidad (para el doppler)
			canal->set3DAttributes(& pos, & vel);
			ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;


		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;
	}//playLoopSound3D
	//--------------------------------------------------------

	void CServer::stopSound(const std::string& id){
		SoundChannelMap::const_iterator itMap = _soundChannel.begin();
		SoundChannelMap::const_iterator itErase;
		bool mapErase = false;
		for(; itMap != _soundChannel.end() && !mapErase; ++itMap) {
			if(itMap->first.compare(id)==0) {
				itMap->second->stop();
				itErase=itMap;
				mapErase=true;
			}
		}
		_soundChannel.erase(itErase);
			
	}//stopSound
	//--------------------------------------------------------


	void CServer::stopAllSounds(){
		SoundChannelMap::const_iterator itMap = _soundChannel.begin();
		bool mapErase = false;
		for(; itMap != _soundChannel.end(); ++itMap) {
				itMap->second->stop();
		}
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
		float volume=0.7f; // valor entre 0 y 1
		result = canal->setVolume(volume);
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
		float volume=0.7f; // valor entre 0 y 1
		result = canal->setVolume(volume);
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


} // namespace Audio