/**
@file MessageInt.h

Contiene el tipo de datos de un mensaje int.

@see Logic::CMessageInt

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageInt_H
#define __Logic_MessageInt_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageInt : public CMessage
	{
		DEC_MFACTORY(CMessageInt);
	public:


		CMessageInt() : CMessage(GetAltTypeIdOf(CMessageInt)), _int(0) {}
			
		~CMessageInt(){};


		int getInt() const { return _int; }
		void setInt(const int valor) {_int=valor; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		intdel mensaje
		*/

		int _int;

	}; //class CmessageFloat

	REG_MFACTORY(CMessageInt);

} // namespace Logic

#endif // __Logic_Message_H
