/**
@file MessageBoolString.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.
@see Logic::CMessageBool

@author Emilio Santalla Comellas
*/


#include "MessageBoolString.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageBoolString);

	void CMessageBoolString::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);

		data.write((void*) (&_bool),	sizeof(_bool));
		
		unsigned int size = _string.size();  
		data.write(&size,sizeof(size));		
		data.write((void*)_string.c_str(),size);
		
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageBoolString::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);

		data.read(&_bool,	sizeof(_bool));

		unsigned int size;
			data.read(&size, sizeof(size)); 		
		char* aux = new char[size];		
			data.read(aux, size);
			_string.assign(aux,size); 
			delete[] aux;

		
			
	} // deserialize

} // namespace Logic