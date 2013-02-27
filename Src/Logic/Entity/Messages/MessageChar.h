/**
@file MessageChar.h

Contiene el tipo de datos de un mensaje char.

@see Logic::CMessageChar

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageChar_H
#define __Logic_MessageChar_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageChar : public CMessage
	{
		DEC_MFACTORY(CMessageChar);
	public:


		CMessageChar() : CMessage(GetAltTypeIdOf(CMessageChar)), _char(0) {}
			
		~CMessageChar(){};

		char getChar() const { return _char; }
		void setChar(const char valor) {_char=valor; }

		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
			char del mensaje
		*/
		char _char;

	}; //class CmessageFloat

	REG_MFACTORY(CMessageChar);

} // namespace Logic

#endif // __Logic_Message_H
