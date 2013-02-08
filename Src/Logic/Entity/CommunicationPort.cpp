/**
@file CommunicationPort.cpp

Contiene la implementación de la clase que se encarga del intercambio 
de los mensajes.

@see Logic::CCommunicationPort

@author David Llansó García
@date Julio, 2010
*/

#include "CommunicationPort.h"
#include "Logic/Entity/Messages/Message.h"

namespace Logic {

	CCommunicationPort::~CCommunicationPort()
	{
		_messages.clear();

	} // ~CCommunicationPort
	
	//---------------------------------------------------------

	bool CCommunicationPort::set(CMessage *message)
	{
		bool accepted = accept(message);
		if(accepted)
			message->grab();
			_messages.push_back(message);

		return accepted;

	} // set
	
	//---------------------------------------------------------

	void CCommunicationPort::processMessages()
	{
		TMessageList::const_iterator it = _messages.begin();
		for(; it != _messages.end(); it++)
		{
			process(*it);
			(*it)->release();
		}

		_messages.clear();

	} // processMessages

} // namespace Logic
