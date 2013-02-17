/**
@file MessageV3.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageV3

@author Emilio Santalla
@date Enero, 2013
*/


#include "MessageV3.h"


namespace Logic 
{

	IMP_MFACTORY(CMessageV3);

	void CMessageV3::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);

		data.write((void*)(&_vector3.x), sizeof(_vector3.x));
		data.write((void*)(&_vector3.y), sizeof(_vector3.y));
		data.write((void*)(&_vector3.z), sizeof(_vector3.z));

			
	} // serialize
	
	//---------------------------------------------------------

	void CMessageV3::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);

		data.read(&_vector3.x, sizeof(_vector3.x));
		data.read(&_vector3.y, sizeof(_vector3.y));
		data.read(&_vector3.z, sizeof(_vector3.z));
			
	} // deserialize

} // namespace Logic