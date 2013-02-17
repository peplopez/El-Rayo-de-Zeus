/**
@file MessageTF.h

Contiene el tipo de datos de un mensaje transform.

@see Logic::CMessageTF

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageTF_H
#define __Logic_MessageTF_H


#include "Net/buffer.h"
#include "Message.h"
#include "BaseSubsystems/Math.h"



// Predeclaracion de clases
namespace Logic {
	class CEntity;
};

// Declaración de la clase
namespace Logic
{	
	/**
	Contiene el tipo de datos de un mensaje transform
		
	*/
	class CMessageTF : public CMessage
	{
		DEC_MFACTORY(CMessageTF);

	public:


		CMessageTF() :  CMessage(GetAltTypeIdOf(CMessageTF)), _transform(Matrix4::ZERO) {}
	
		~CMessageTF(){}


		const Matrix4 &getTransform() { return _transform; }
		void setTransform(const Matrix4 &transform) { _transform=transform; }

		
		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		Transform del mensaje
		*/
		Matrix4 _transform;

	}; //class CmessageTF

	REG_MFACTORY(CMessageTF);

} // namespace Logic

#endif // __Logic_Message_H
