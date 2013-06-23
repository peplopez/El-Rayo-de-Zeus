/**
@file MessageBoolString.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageBoolString

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageFloatString_H
#define __Logic_MessageFloatString_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageFloatString : public CMessage
	{
		DEC_MFACTORY(CMessageFloatString);
	public:


		CMessageFloatString() : CMessage(GetAltTypeIdOf(CMessageFloatString)), _float(0.0f), _string("") {}	
		~CMessageFloatString() {}


		bool getFloat() const { return _float; }
		void setFloat(const float value) {_float=value; }


		std::string getString() const { return _string; }
		void setString(const std::string& valor) {_string=valor; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:
		/**
		bool del mensaje
		*/
		float _float;

		/**
		string del mensaje
		*/
		std::string _string;



	}; //class CmessageBool

	REG_MFACTORY(CMessageFloatString);

} // namespace Logic

#endif // __Logic_FloatStringMessage_H