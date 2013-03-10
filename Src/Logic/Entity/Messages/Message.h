/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::CMessage

@author ESC
*/
#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>

#include "Net/buffer.h"
#include "BaseSubsystems/Rtti.h"
#include "MessageFactory.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	namespace Message 
	{
		enum TMessageType : unsigned char
		{
			UNASSIGNED 			= 0xFF,
			SET_TRANSFORM 		= 0x00,
			CONTROL				= 0x01,
			NPC_CONTROL 		= 0x02,
			DEAD 				= 0x03,
			SET_TRANSFORM_QUAT	= 0x04,
			SET_ANIMATION		= 0x05,
			STOP_ANIMATION		= 0x06,			 
			CONTACT				= 0x07,   
			SET_SHADER			= 0x08,			
			CAMERA				= 0x09,
			LIFE_MODIFIER		= 0x0A,
			AVATAR_MOVE			= 0x10,
			DAMAGED				= 0x11,			
			ANIMATION_FINISHED	= 0x12,
			COMBAT				= 0x13,
			CHANGE_PLANE		= 0x14,
			CHANGE_HEIGHT		= 0x15,
			HUD					= 0x16,		
			TRIGGER				= 0x17,
			COLLISION			= 0X18,
			SPACE_REQUEST		= 0x19, //para mandar un mensaje en el que preguntas si un punto del espacio está ocupado
			ANIMATION_MOMENT    = 0x20,
			REWIND_ANIMATION    = 0x21
		};

		enum TActionType : unsigned char
		{
			UNDEF				= 0xFF,//pep: esto es hexadecimal? no parece que llevemos un orden en la numeración hexadecimal
			SWITCH_ALTAR		= 0x01,
			STOP_SWITCH			= 0x02,
			GO_UP				= 0x0F,
			GO_DOWN				= 0x10,
			WALK_LEFT			= 0x11,
			WALK_RIGHT			= 0x12,
			WALK_STOP			= 0x13,
			WALK_BACK			= 0x14,
			JUMP				= 0x15,
			TURN				= 0x16,
			CHANGE_DIRECTION	= 0x17,
			CHANGE_BASE			= 0x18,
			CHANGE_RING			= 0x19,
			UPDATE_LIFE			= 0x20,
			LIGHT_ATTACK		= 0x21,
			HEAVY_ATTACK		= 0x22,
			DISPLAY_HUD			= 0x23,
			TRIGGER_ENTER		= 0x24,
			TRIGGER_EXIT		= 0x25,
			COVER				= 0x26
		};
	}

	/**
	Tipo copia para los mensajes. Por simplicidad.
	*/
	typedef Logic::Message::TMessageType TMessageType;

	/**
	Tipo copia para los mensajes. Por simplicidad.
	*/
	typedef Logic::Message::TActionType TActionType;

	/**
	Declaración de la clase
	
	@ingroup logicGroup
	@ingroup entityGroup

	@author Emilio Santalla
	@date Enero, 2013
	@ingroup grupoEntidad
	*/
	class CMessage
	{

	public:

		/**
		Constructor básico
		*/
		CMessage() : _classId(GetAltTypeIdOf(CMessage)),_refCount(0), _type(Message::UNASSIGNED), _action(Message::UNDEF) {}
			
		
		/**
		Constructor de la clase. 
		*/
		CMessage(altTypeId classId) :  _classId(classId), _refCount(0), _type(Message::UNASSIGNED), _action(Message::UNDEF) {}

		/**
		Destructor de la clase. El destructor se invoca cuando la cuenta
		de referencias vuelve a 0.
		*/
		virtual ~CMessage();

		/**
		Incrementa la cuenta de referencias. Ha de llamarse a este método cada 
		vez que se crea el mensaje en el heap, y cada vez que es aceptado por algún
		componente.
		*/			
		void grab() const;

		/**
		Decrementa la cuenta de referencias y libera el objeto cuando está llega a 0.
		*/		
		void release() const;

		/**
		Devuelve el tipo del Mensaje

		@return Tipo de mensaje.
		*/
		const TMessageType &getType() const { return _type; }

		/**
		Establece el tipo del mensaje.

		@param Tipo de mensaje.
		*/
		void setType(const TMessageType &type)  { _type=type; }


		/**
		Devuelve la accion del mensaje

		@return accion de mensaje.
		*/
		const TActionType &getAction() const { return _action; }


		/**
		Establece la accioón del mensaje.

		@param Tipo de mensaje.
		*/
		void setAction(const TActionType &action)  { _action=action; }

		/**
		Devuelve el classId del mensaje

		@return classId.
		*/
		const altTypeId &getClassId() const { return _classId; }


		/**
		Establece la accioón del mensaje.

		@param Tipo de mensaje.
		*/
		void setClassId(const altTypeId &classId)  { _classId=classId; }

		/**
		Serializa un mensaje. Genera un CBuffer a partir de 
		un mensaje.

		@param data CBuffer donde almacenar la deserialización.
		*/
		virtual void serialize (Net::CBuffer &data) const;

		/**
		Deserializa un mensaje. A partir de un CBuffer genera
		un nuevo mensaje.

		@param message Mensaje resultante de la deserialización.
		*/
		virtual void deserialize (Net::CBuffer &data);

		
		static CMessage* create();
		static bool regist();

	private:
		
		/**
		Tipo de la clase
		*/
		altTypeId _classId;

		/**
		Atributo para almacenar el conteo de referencias.
		No modifica la constancia lógica de la clase, por eso lo 
		declaro como mutable
		*/
		mutable unsigned char _refCount;

		/**
		Tipo del mensaje.
		*/
		TMessageType _type;

		/**
		Tipo del mensaje.
		*/
		TActionType _action;



	}; //class Cmessage

	static bool RegisteredFactory_CMessage = CMessage::regist();

/////////////////////////////////////////////////////////////
// Macros para la adición de los mensajes a la factoría // 
// de mensajes.                                         //
/////////////////////////////////////////////////////////////
	
/** 
Macro para la declaración de los métodos necesarios para que 
la factoria cree nuevas instancias del tipo de mensaje y 
para que el mensaje se registre en la factoría.
*/
#define DEC_MFACTORY(Class) \
public: \
	/** \
	Crea un componente de la clase en la que se declara. \
	*/ \
    static CMessage* create(); \
	/** \
	Registra el componente de la clase en la factoría. \
	*/ \
	static bool regist(); \

/** 
Macro para la implementación de los métodos necesarios para que
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factoría.
*/
#define IMP_MFACTORY(Class) \
CMessage* Class::create() \
{ \
	CMessage* res = new Class(); \
	return res; \
} \
bool Class::regist() \
{ \
	altTypeId name = TAltTypeIdGenerator<Class>::GetAltTypeId(); \
	if (!CMessageFactory::getSingletonPtr()->has(name)) \
	{ \
		CMessageFactory::getSingletonPtr()->add(Class::create, name); \
	} \
	return true; \
}

/** 
Macro que invoca al método que registra la clase en la factoría.
*/
#define REG_MFACTORY(Class) \
static bool RegisteredFactory_##Class = Class::regist();

} // namespace Logic

#endif // __Logic_Message_H


