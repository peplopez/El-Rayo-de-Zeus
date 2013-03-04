/**
@file MessageUShort.h

Contiene el tipo de datos de un mensaje int.

@see Logic::CMessageUShort

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageUShort_H
#define __Logic_MessageUShort_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageUShort : public CMessage
	{
		DEC_MFACTORY(CMessageUShort);
	public:


		CMessageUShort() : CMessage(GetAltTypeIdOf(CMessageUShort)), _short(0) {}
			
		~CMessageUShort(){};

		unsigned short getUShort() const { return _short; }
		void setUShort(const unsigned short valor) {_short=valor; }

		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		intdel mensaje
		*/

		unsigned short _short;

	}; //class CmessageFloat

	REG_MFACTORY(CMessageUShort);

} // namespace Logic

#endif // __Logic_Message_H
