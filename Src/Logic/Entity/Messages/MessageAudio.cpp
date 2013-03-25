#include "MessageAudio.h"

#include <string>

namespace Logic {

	IMP_MFACTORY(CMessageAudio);	

	void CMessageAudio::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		
		unsigned int size = _path.size();  
		data.write(&size,sizeof(size));		
		data.write((void*)_path.c_str(),size);

		size = _id.size();  
		data.write(&size,sizeof(size));		
		data.write((void*)_id.c_str(),size);

		data.write((void*)(&_position.x), sizeof(_position.x));
		data.write((void*)(&_position.y), sizeof(_position.y));
		data.write((void*)(&_position.z), sizeof(_position.z));
	
	}

	void CMessageAudio::deserialize(Net::CBuffer &data) {

		unsigned int size;
			data.read(&size, sizeof(size)); 		
		char* aux = new char[size];		
			data.read(aux, size);
			_path.assign(aux,size); 
			delete[] aux;

		unsigned int size1;
			data.read(&size1, sizeof(size1)); 		
		char* aux1 = new char[size1];		
			data.read(aux1, size1);
			_id.assign(aux1,size1); 
			delete[] aux1;

		data.read(&_position.x, sizeof(_position.x));
		data.read(&_position.y, sizeof(_position.y));
		data.read(&_position.z, sizeof(_position.z));
	}
};
