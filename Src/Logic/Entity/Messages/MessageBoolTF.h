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


		CMessageBoolTF() : CMessage(GetAltTypeIdOf(CMessageBoolTF)), _bool(false), _transform(Matrix4::ZERO) {}
		
		~CMessageBoolTF() {}


		float getBool() const { return _bool; }
		void setBool(const bool valor) {_bool=valor; }


		const Matrix4& getTransform() { return _transform; }
		void setTransform(const Matrix4 &transform) { _transform=transform; }


		void serialize(Net::CBuffer &data);
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