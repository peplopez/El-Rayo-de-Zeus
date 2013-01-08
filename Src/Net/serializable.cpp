#include "serializable.h"


namespace Net{

	
	void Serializable::serializeString(CBuffer& buffer, std::string cad){
		
		int tam;
		std::string aux = cad;
		tam = aux.length();
		buffer.write(&tam,sizeof(tam));
		
		if (tam > 0){ 
			buffer.write(&aux[0], tam*sizeof(char));
			//printf("Se escribio %s \n",aux.c_str());
		}
		
		
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
}