/**
@file MessageBool.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.
@see Logic::CMessageBool

@author Emilio Santalla Comellas
*/


#include "MessageBool.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageBool);

	void CMessageBool::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_bool),	sizeof(_bool));
		
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageBool::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_bool,	sizeof(_bool));
		
			
	} // deserialize

} // namespace Logic