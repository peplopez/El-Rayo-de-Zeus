/**
@file CommunicationPort.cpp

Contiene la implementación de la clase que se encarga del intercambio 
de los mensajes.

@see Logic::CCommunicationPort

@author David Llansó García
@date Julio, 2010
*/

#include "CommunicationPort.h"

namespace Logic {

	CCommunicationPort::~CCommunicationPort()
	{
		_messages.clear();

	} // ~CCommunicationPort
	
	//---------------------------------------------------------

	bool CCommunicationPort::set(const TMessage &message)
	{
		bool accepted = accept(message);
		if(accepted)
			_messages.push_back(message);

		return accepted;

	} // set
	
	//---------------------------------------------------------

	void CCommunicationPort::processMessages()
	{
		TMessageList::const_iterator it = _messages.begin();
		for(; it != _messages.end(); it++)
			process(*it);

		_messages.clear();

	} // processMessages

} // namespace Logic
