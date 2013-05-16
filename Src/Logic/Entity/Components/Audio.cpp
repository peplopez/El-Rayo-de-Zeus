/**
@file Audio.cpp

Contiene la implementación del componente que controla el sonido de una entidad.
 
@see Logic::CAudio
@see Logic::IComponent

@author Jose 
@date Marzo, 2013
*/

#include "Audio.h"

#include "../../../Audio/Server.h"

#include "../Messages/MessageAudio.h"

namespace Logic 
{
	IMP_FACTORY(CAudio);
	
	//---------------------------------------------------------
	
	bool CAudio::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		return true;

	} // spawn
	//---------------------------------------------------------

	/*bool CAudio::activate()
	{
		IComponent::activate();
		return true;

	} // activate*/
	//---------------------------------------------------------

	bool CAudio::accept(const CMessage *message)
	{
		return message->getType() == Message::AUDIO;
	} // accept
	//---------------------------------------------------------

	void CAudio::process(CMessage *message)
	{
		std::string ruta,id;
		Vector3 position;
		bool notIfPlay;
		bool localPlayer;

		switch(message->getType())
		{
		case Message::AUDIO:
			{
				//Recogemos los datos
				CMessageAudio *maux = static_cast<CMessageAudio*>(message);

				ruta=maux->getPath();
				id=maux->getId();
				position=maux->getPosition();
				notIfPlay=maux->getNotIfPlay();
				localPlayer=maux->getIsPlayer();

				//Le decimos al server de audio lo que queremos reproducir
				char *aux=new char[ruta.size()+1];
				aux[ruta.size()]=0;
				memcpy(aux,ruta.c_str(),ruta.size());
				//Si es local el sonido será stereo
				if(localPlayer)
					Audio::CServer::getSingletonPtr()->playSound(aux,id,notIfPlay);
				//En otro caso se trata de un sonido con posición 3D
				else
					Audio::CServer::getSingletonPtr()->playSound3D(aux,id,position,notIfPlay);
			}
			break;

		}

	} // process
	//----------------------------------------------------------

	void CAudio::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

	} // tick
	//----------------------------------------------------------

} // namespace Logic


