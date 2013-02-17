/**
@file MessageShort.h

Contiene el tipo de datos de un mensaje int.

@see Logic::CMessageShort

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageShort_H
#define __Logic_MessageShort_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageShort : public CMessage
	{
		DEC_MFACTORY(CMessageInt);
	public:


		CMessageShort() : CMessage(GetAltTypeIdOf(CMessageShort)), _short(0) {}
			
		~CMessageShort(){};


		unsigned short getShort() const { return _short; }
		void setShort(const unsigned short valor) {_short=valor; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		intdel mensaje
		*/

		unsigned short _short;

	}; //class CmessageFloat

	REG_MFACTORY(CMessageShort);

} // namespace Logic

#endif // __Logic_Message_H
