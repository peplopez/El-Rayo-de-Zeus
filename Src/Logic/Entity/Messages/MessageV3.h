/**
@file MessageV3.h

Contiene el tipo de datos de un mensaje Vector3.

@see Logic::CMessageV3

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageV3_H
#define __Logic_MessageV3_H


#include "BaseSubsystems/Math.h"
#include "Net/buffer.h"
#include "Message.h"



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
	class CMessageV3 : public CMessage
	{
		DEC_MFACTORY(CMessageV3);

	public:


		CMessageV3() : CMessage(GetAltTypeIdOf(CMessageV3)), _vector3(Vector3::ZERO) {}
			

		~CMessageV3() {};


		const Vector3& getVector3() { return _vector3; }
		void setVector3(const Vector3 &vector3) { _vector3=vector3; }


		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);
	

	private:

		/**
		Vector3 del mensaje
		*/
		Vector3 _vector3;

	}; //class CmessageFloat

	REG_MFACTORY(CMessageV3);

} // namespace Logic

#endif // __Logic_Message_H
