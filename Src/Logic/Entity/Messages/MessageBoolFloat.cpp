/**
@file MessageBoolFloat.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.
@see Logic::CMessageBoolFloat

@author Emilio Santalla Comellas
*/


#include "MessageBoolFloat.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageBoolFloat);

	void CMessageBoolFloat::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_bool),	sizeof(_bool));
		data.write((void*) (&_float),	sizeof(_float));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageBoolFloat::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_bool,	sizeof(_bool));
		data.read(&_float,  sizeof(_float));
			
	} // deserialize

} // namespace Logic