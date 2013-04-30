/**
@file MessageBoolFloatString.h

Contiene el tipo de datos de un mensaje float con booleano y string

@see Logic::CMessageBoolFloatString

@author Jose Luis López Sánchez
*/

#ifndef __Logic_MessageBoolFloatString_H
#define __Logic_MessageBoolFloatString_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageBoolFloatString : public CMessage
	{
		DEC_MFACTORY(CMessageBoolFloat);
	public:

		/**
		Constructor básico
		*/
		CMessageBoolFloatString() :  CMessage(GetAltTypeIdOf(CMessageBoolFloatString)), _bool(false), _float(0.0f) {}
			

		~CMessageBoolFloatString() {};


		float getBool() const { return _bool; }
		void setBool(const bool valor) {_bool=valor; }


		float getFloat() const { return _float; }
		void setFloat(const float valor) {_float=valor; }

		std::string getString() const { return _string; }
		void setString(const std::string& valor) {_string=valor; }

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
		float del mensaje
		*/
		float _float;

		/**
		string del mensaje
		*/
		std::string _string;
	}; //class CmessageBoolFloat

	REG_MFACTORY(CMessageBoolFloatString);

} // namespace Logic

#endif // __Logic_Message_H