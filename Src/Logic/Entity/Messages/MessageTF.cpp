/**
@file MessageTF.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageTF

@author Emilio Santalla
@date Enero, 2013
*/


#include "MessageTF.h"



namespace Logic 
{
	IMP_MFACTORY(CMessageTF);

	//---------------------------------------------------------

	void CMessageTF::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);

		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				data.write((void*)&_transform[i][j], sizeof(_transform[i][j]));
			
	} // serialize
	
	//---------------------------------------------------------

	void CMessageTF::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);

		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				data.read(&_transform[i][j], sizeof(_transform[i][j]));
			
	} // deserialize

} // namespace Logic