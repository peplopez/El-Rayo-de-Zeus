/**
@file MessageInt.h

Contiene el tipo de datos de un mensaje int.

@see Logic::CMessageUInt

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageUInt_H
#define __Logic_MessageUInt_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageUInt : public CMessage
	{
		DEC_MFACTORY(CMessageUInt);
	public:


		CMessageUInt() : CMessage(GetAltTypeIdOf(CMessageUInt)), _uInt(0) {}
			
		~CMessageUInt(){};

		unsigned int getUInt() const { return _uInt; }
		void setUInt(const unsigned int valor) {_uInt = valor; }
		
		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		usigned int del mensaje
		*/

		unsigned int _uInt;

	}; //class CmessageUInt

	REG_MFACTORY(CMessageUInt);

} // namespace Logic

#endif // __Logic_MessageUInt_H
