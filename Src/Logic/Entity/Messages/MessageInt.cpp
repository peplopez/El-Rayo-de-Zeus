/**
@file MessageInt.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageInt

@author Emilio Santalla Comellas
*/


#include "MessageInt.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageInt);

	void CMessageInt::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_int),	sizeof(_int));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageInt::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_int,  sizeof(_int));
			
	} // deserialize

} // namespace Logic