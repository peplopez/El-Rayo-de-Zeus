/**
@file MessageUIntString.h

Contiene el tipo de datos de un mensaje int.

@see Logic::CMessageUInt

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageUIntString_H
#define __Logic_MessageUIntString_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageUIntString : public CMessage
	{
		DEC_MFACTORY(CMessageUIntString);
	public:


		CMessageUIntString() : CMessage(GetAltTypeIdOf(CMessageUIntString)), _uInt(0), _string("") {}
			
		~CMessageUIntString(){};

		unsigned int getUInt() const { return _uInt; }
		void setUInt(const unsigned int valor) {_uInt = valor; }

		std::string getString() const { return _string; }
		void setString(const std::string& valor) {_string=valor; }
		
		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		usigned int del mensaje
		*/

		unsigned int _uInt;


		/**
		string del mensaje
		*/

		std::string _string;



	}; //class CmessageUIntString

	REG_MFACTORY(CMessageUIntString);

} // namespace Logic

#endif // __Logic_MessageUIntString_H
