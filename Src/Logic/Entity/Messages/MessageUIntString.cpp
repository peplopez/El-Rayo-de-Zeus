/**
@file MessageUIntString.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::CMessageUIntString

@author Emilio Santalla Comellas
*/


#include "MessageUIntString.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageUIntString);

	void CMessageUIntString::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_uInt),	sizeof(_uInt));
		
		unsigned int size = _string.size();  
		data.write(&size,sizeof(size));		
		data.write((void*)_string.c_str(),size);
		
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageUIntString::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_uInt,  sizeof(_uInt));

		unsigned int size;
			data.read(&size, sizeof(size)); 		
		char* aux = new char[size];		
			data.read(aux, size);
			_string.assign(aux,size); 
			delete[] aux;
			
	} // deserialize

} // namespace Logic