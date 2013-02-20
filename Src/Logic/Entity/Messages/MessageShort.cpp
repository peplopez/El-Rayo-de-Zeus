/**
@file MessageShort.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageShort

@author Emilio Santalla Comellas
*/


#include "MessageShort.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageShort);

	void CMessageShort::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_short),	sizeof(_short));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageShort::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_short,  sizeof(_short));
			
	} // deserialize

} // namespace Logic