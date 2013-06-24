/**
@file MessageFloat.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageFloatUShort

@author Emilio Santalla Comellas
*/


#include "MessageFloatUShort.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageFloatUShort);

	void CMessageFloatUShort::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_float),	sizeof(_float));
		data.write((void*) (&_ushort),	sizeof(_ushort));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageFloatUShort::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_float,  sizeof(_float));
		data.read(&_ushort,  sizeof(_ushort));
			
	} // deserialize

} // namespace Logic