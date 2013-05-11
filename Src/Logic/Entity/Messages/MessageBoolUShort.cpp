/**
@file MessageBoolUShort.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageBoolUShort

@author Jose Luis López
*/


#include "MessageBoolUShort.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageBoolUShort);

	void CMessageBoolUShort::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_bool),	sizeof(_bool));
		data.write((void*) (&_short),	sizeof(_short));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageBoolUShort::deserialize(Net::CBuffer &data)
	{
		CMessage::deserialize(data);
		data.read(&_bool,	sizeof(_bool));
		data.read(&_short,  sizeof(_short));
			
	} // deserialize

} // namespace Logic