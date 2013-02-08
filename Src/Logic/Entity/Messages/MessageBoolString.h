/**
@file MessageBoolString.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageBoolString

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageBoolString_H
#define __Logic_MessageBoolString_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageBoolString : public CMessage
	{
		DEC_MFACTORY(CMessageBoolString);
	public:


		CMessageBoolString() : CMessage(GetAltTypeIdOf(CMessageBoolString)), _bool(false), _string("") {}	
		~CMessageBoolString() {};


		float getBool() const { return _bool; }
		void setBool(const bool valor) {_bool=valor; }


		std::string getString() const { return _string; }

		void setString(const std::string& valor) {_string=valor; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:
		/**
		bool del mensaje
		*/
		bool _bool;

		/**
		string del mensaje
		*/
		std::string _string;



	}; //class CmessageBool

	REG_MFACTORY(CMessageBoolString);

} // namespace Logic

#endif // __Logic_Message_H