#include "serializable.h"


namespace Net{

	
	void Serializable::serializeString(CBuffer& buffer, std::string s){
		
		unsigned int length = s.length();
			buffer.write(&length, sizeof(length) );	
			if (length > 0)
				buffer.write((void*) s.c_str(), length);
	}
	

	std::string Serializable::deserializeString(CBuffer& buffer){
	
		int tam;
		buffer.read(&tam, sizeof(tam));
		
		if (tam > 0){
		
			std::string cad(tam,'0');
			buffer.read(&cad[0],tam*sizeof(char));
			//printf("se leyo %s \n",cad.c_str());
			
			return cad;
		}else{
			
			return "";
		
		}
	
	}

	// TODO meter aquí el resto de serializes?
}