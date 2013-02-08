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

		/**
		Constructor básico
		*/
		CMessageFloat() : CMessage(GetAltTypeIdOf(CMessageFloat)), _float(0.0f) {}
			

		/**
		Destructor de la clase. El destructor se invoca cuando la cuenta
		de referencias vuelve a 0.
		*/
		~CMessageFloat(){};

		/**
		Devuelve el float del Mensaje

		@return float del mensaje.
		*/
		float getFloat() const { return _float; }

		/**
		Establece el float del mensaje

		@return float del mensaje.
		*/
		void setFloat(const float valor) {_float=valor; }


		/**
		Serializa un mensaje. Genera un CBuffer a partir de 
		un mensaje.

		@param data CBuffer donde almacenar la deserialización.
		*/
		void serialize(Net::CBuffer &data);

		/**
		Deserializa un mensaje. A partir de un CBuffer genera
		un nuevo mensaje.

		@param message Mensaje resultante de la deserialización.
		*/
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
