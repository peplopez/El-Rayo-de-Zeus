/**
@file MessageChar.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageChar

@author Emilio Santalla Comellas
*/


#include "MessageChar.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageChar);

	void CMessageChar::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_char),	sizeof(_char));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageChar::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_char,  sizeof(_char));
			
	} // deserialize

} // namespace Logic