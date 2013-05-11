/**
@file MessageBoolUShort.h

Contiene el tipo de datos de un mensaje unsigned short con booleano.

@see Logic::CMessageBoolUShort

@author Jose Luis López 
*/

#ifndef __Logic_MessageBoolUShort_H
#define __Logic_MessageBoolUShort_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageBoolUShort : public CMessage
	{
		DEC_MFACTORY(CMessageBoolUShort);
	public:


		CMessageBoolUShort() : CMessage(GetAltTypeIdOf(CMessageBoolUShort)), _short(0) {}
			
		~CMessageBoolUShort(){};
		
		float getBool() const { return _bool; }
		void setBool(const bool valor) {_bool=valor; }

		unsigned short getUShort() const { return _short; }
		void setUShort(const unsigned short valor) {_short=valor; }

		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		bool del mensaje
		*/
		bool _bool;

		/**
		UShort del mensaje
		*/

		unsigned short _short;

	}; //class CmessageBoolUShort

	REG_MFACTORY(CMessageBoolUShort);

} // namespace Logic

#endif // __Logic_Message_H
