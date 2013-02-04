/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::TMessage

@author David Llansó García
*/
#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>

#include "BaseSubsystems/Math.h"

#include "Net/buffer.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};

// Declaración de la clase
namespace Logic
{
	/**
	Namespace para los tipos de mensajes posibles.
	*/
	namespace Message
	{
		enum TMessageType
		{
			UNASSIGNED          = 0xFFFFFFFF,
			SET_TRANSFORM       = 0x00000000,
			SET_ANIMATION       = 0x00000001,
			STOP_ANIMATION      = 0x00000002,
			CONTROL	            = 0x00000003,
			AVATAR_WALK         = 0x00000004,
			TOUCHED             = 0x00000005,
			UNTOUCHED           = 0x00000006,
			SWITCH              = 0x00000007,
			DAMAGED             = 0x00000008,
			DEAD				= 0x00000009,
			MOVE_TO             = 0x0000000A,
			ROUTE_TO            = 0x0000000B,
			FINISHED_ROUTE      = 0x0000000C,
			FAILED_ROUTE        = 0x0000000D,
			ANIMATION_FINISHED  = 0x0000000E		
		};
	}

	/**
	Tipo copia para los mensajes. Por simplicidad.
	*/
	typedef Message::TMessageType TMessageType;

	/**
	Contiene el tipo de datos de un mensaje. Tiene una serie de
	atributos genéricos que se interpretarán en función del tipo 
	de mensaje.
	<p>
	@remarks <b>¡¡ESTO NO ES ESCALABLE!!</b> En tu proyecto esto 
	debería ser cambiado.
	Lo suyo sería tener una clase CMesage base y luego clases que
	especializasen a ésta con los atributos necesarios para el 
	mensaje concreto en vez de tener una serie de atributos
	genéricos como es esta estructura que deben de ser interpretados
	externamente en función del tipo de mensaje.
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó García
	@date Julio, 2010
    @ingroup grupoEntidad
	*/
	struct TMessage
	{
		/**
		Tipo del mensaje.
		*/
		TMessageType _type;

		/**
		Atributo para almacenar una matriz de transformación.
		*/
		Matrix4 _transform;
		
		/**
		Atributo para almacenar un valor int.
		*/
		int _int;

		/**
		Atributo para almacenar un valor float.
		*/
		float _float;
		
		/**
		Atributo para almacenar un valor booleano.
		*/
		bool _bool;
		
		/**
		Atributo para almacenar un string.
		*/
		std::string _string;

		/**
		Atributo para almacenar un vector.
		*/
		Vector3 _vector3;

		/**
		Atributo para almacenar una entidad.
		*/
		CEntity *_entity;

		TMessage() : _type(Message::UNASSIGNED), _int(0), _float(0.0f), _bool(false),
			_string(""),_vector3(Vector3::ZERO),_entity(0),_transform(Matrix4::ZERO)
			{}


	}; 

	namespace Message
	{
		/**
		Serializa un mensaje. Genera un CBuffer a partir de 
		un mensaje.

		@param message Mensaje a serializar.
		@param data CBuffer donde almacenar la deserialización.
		*/
		void Serialize(const TMessage& message, Net::CBuffer &data);

		/**
		Deserializa un mensaje. A partir de un CBuffer genera
		un nuevo mensaje.

		@param data CBuffer a deserializar.
		@param message Mensaje resultante de la deserialización.
		*/
		void Deserialize(Net::CBuffer &data, TMessage& message);
	}

} // namespace Logic

#endif // __Logic_Message_H
