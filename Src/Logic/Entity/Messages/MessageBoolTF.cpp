/**
@file MessageBoolTF.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::TMessage

@author Emilio Santalla
@date Enero, 2013
*/


#include "MessageBoolTF.h"
#include "Logic/Entity/Entity.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageBoolTF);

	//---------------------------------------------------------

	void CMessageBoolTF::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);

		data.write((void*) (&_bool),	sizeof(_bool));

		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				data.write((void*)(&_transform[i][j]), sizeof(_transform[i][j]));
			
	} // serialize
	
	//---------------------------------------------------------

	void CMessageBoolTF::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);

		data.read(&_bool,	sizeof(_bool));

		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				data.read(&_transform[i][j], sizeof(_transform[i][j]));
			
	} // deserialize

} // namespace Logic