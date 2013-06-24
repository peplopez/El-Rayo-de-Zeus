/**
@file MessageFloatUShort.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageFloatUShort

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageFloatUShort_H
#define __Logic_MessageFloatUShort_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageFloatUShort : public CMessage
	{
		DEC_MFACTORY(CMessageFloatUShort);
	public:


		CMessageFloatUShort() : CMessage(GetAltTypeIdOf(CMessageFloatUShort)), _float(0.0f), _ushort(0) {}
			
		~CMessageFloatUShort(){};


		float getFloat() const { return _float; }
		void setFloat(const float valor) {_float=valor; }

		unsigned short getUShort() const { return _ushort; }
		void setUShort(const unsigned short valor) {_ushort = valor; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		float del mensaje
		*/

		float _float;

		unsigned short _ushort;

	}; //class CmessageFloat

	REG_MFACTORY(CMessageFloatUShort);

} // namespace Logic

#endif // __Logic_Message_H
