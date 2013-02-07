#include "serializable.h"


namespace Net{

	
	void Serializable::serializeString(CBuffer& buffer, std::string s){
		
		unsigned int length = s.length();
			buffer.write(&length, sizeof(length) );	
			if (length > 0)
				buffer.write((void*) s.c_str(), length);
	}
	

	std::string Serializable::deserializeString(CBuffer& buffer){
	
		unsigned int length;
			buffer.read(&length, sizeof(length)); // Leemos longitud		
			
				if (length > 0){
					std::string s(length,'0');
					buffer.read( &s[0], length);					
					return s;			
				} else
					return "";
	}

	// TODO meter aquí el resto de serializes?
}