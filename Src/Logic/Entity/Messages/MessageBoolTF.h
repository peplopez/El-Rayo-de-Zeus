/**
@file MessageBoolTF.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageBool

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageBoolTF_H
#define __Logic_MessageBoolTF_H


#include "Net/buffer.h"
#include "Message.h"
#include "BaseSubsystems/Math.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageBoolTF : public CMessage
	{
		DEC_MFACTORY(CMessageBoolTF);
	public:

		/**
		Constructor básico
		*/
		CMessageBoolTF() : CMessage(GetAltTypeIdOf(CMessageBoolTF)), _bool(false), _transform(Matrix4::ZERO) {}
			

		/**
		Destructor de la clase. El destructor se invoca cuando la cuenta
		de referencias vuelve a 0.
		*/
		~CMessageBoolTF() {};

		/**
		Devuelve el float del Mensaje

		@return float del mensaje.
		*/
		float getBool() const { return _bool; }

		/**
		Establece el float del mensaje

		@return float del mensaje.
		*/
		void setBool(const bool valor) {_bool=valor; }

		/**
		Devuelve el transform del Mensaje

		@return transform del mensaje.
		*/
		const Matrix4 &getTransform() const { return _transform; }

		/**
		Establece el transform del mensaje

		@param transform, transform del mensaje
		*/
		void setTransform(const Matrix4 &transform) { _transform=transform; }


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
		bool del mensaje
		*/
		bool _bool;

		/**
		Transform del mensaje
		*/
		Matrix4 _transform;



	}; //class CmessageBool

	REG_MFACTORY(CMessageBoolTF);

} // namespace Logic

#endif // __Logic_Message_H