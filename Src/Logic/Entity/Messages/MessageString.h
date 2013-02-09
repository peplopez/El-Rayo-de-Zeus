/**
@file MessageString.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageString

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageString_H
#define __Logic_MessageString_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageString : public CMessage
	{
		DEC_MFACTORY(CMessageString);
	public:


		CMessageString() : CMessage(GetAltTypeIdOf(CMessageString)), _string("") {}	
		
		~CMessageString(){};



		std::string getString() const { return _string; }
		void setString(const std::string& valor) {_string=valor; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		string del mensaje
		*/
		std::string _string;



	}; //class CmessageBool

	REG_MFACTORY(CMessageString);

} // namespace Logic

#endif // __Logic_Message_H