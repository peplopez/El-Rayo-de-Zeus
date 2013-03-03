/**
@file MessageUInt.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageUInt

@author Emilio Santalla Comellas
*/


#include "MessageUInt.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageUInt);

	void CMessageUInt::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_uInt),	sizeof(_uInt));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageUInt::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_uInt,  sizeof(_uInt));
			
	} // deserialize

} // namespace Logic