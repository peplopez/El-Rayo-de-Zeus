/**
@file MessageFloat.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageFloat

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageFloat_H
#define __Logic_MessageFloat_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageFloat : public CMessage
	{
		DEC_MFACTORY(CMessageFloat);
	public:


		CMessageFloat() : CMessage(GetAltTypeIdOf(CMessageFloat)), _float(0.0f) {}
			
		~CMessageFloat(){};


		float getFloat() const { return _float; }
		void setFloat(const float valor) {_float=valor; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		float del mensaje
		*/

		float _float;

	}; //class CmessageFloat

	REG_MFACTORY(CMessageFloat);

} // namespace Logic

#endif // __Logic_Message_H
