/**
@file MessageBool.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageBool

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageBool_H
#define __Logic_MessageBool_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageBool : public CMessage
	{
		DEC_MFACTORY(CMessageBool);
	public:


		CMessageBool() : CMessage(GetAltTypeIdOf(CMessageBool)), _bool(false) {}
			
		~CMessageBool() {}


		float getBool() const { return _bool; }
		void setBool(const bool valor) {_bool=valor; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:
		/**
		bool del mensaje
		*/
		bool _bool;



	}; //class CmessageBool

	REG_MFACTORY(CMessageBool);

} // namespace Logic

#endif // __Logic_Message_H