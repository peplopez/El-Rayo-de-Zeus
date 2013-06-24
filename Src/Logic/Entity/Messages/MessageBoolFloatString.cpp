/**
@file CMessageBoolFloatString.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.
@see Logic::CMessageBoolFloatString

@author Jose Luis López Sánchez
*/


#include "MessageBoolFloatString.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageBoolFloatString);

	void CMessageBoolFloatString::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_bool),	sizeof(_bool));
		data.write((void*) (&_float),	sizeof(_float));
					
		unsigned int size = _string.size();  
		data.write(&size,sizeof(size));		
		data.write((void*)_string.c_str(),size);
	} // serialize
	
	//---------------------------------------------------------

	void CMessageBoolFloatString::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_bool,	sizeof(_bool));
		data.read(&_float,  sizeof(_float));
			
		unsigned int size;
			data.read(&size, sizeof(size)); 		
		char* aux = new char[size];		
			data.read(aux, size);
			_string.assign(aux,size); 
			delete[] aux;
	} // deserialize

} // namespace Logic