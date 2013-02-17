/**
@file MessageFactory.h

Contiene la declaración de la clase CMessageFactory, Singleton que almacena 
los punteros a función de funciones de creación de mensajes (CMessage).

@see Logic::CMessageFactory
@see BaseSubsystems::CFactory
@see Logic::CMessage

@author Emilio Santalla
@date Julio, 2010
*/
#ifndef __Logic_MessageFactory_H
#define __Logic_MessageFactory_H

#include "BaseSubsystems/Factory.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CMessage;
}

// Declaración de la clase
namespace Logic
{
	/**
	Factoría de mensajes. Extiende la plantilla BaseSubsystems::CFactory
	para CMessage haciendo de ella un singleton por lo que solo habrá una 
	instancia de esta clase.
	<p>
	Es usada para almacenar mesanjes 
	en Logic::CMessageFactory donde todo el registro de componentes se realiza 
	utilizando macros del preprocesador declarados en Logic::CMessage.

	@ingroup logicGroup
	@ingroup mapGroup

	@author Emilio Santalla
	@date Enero, 2013
	*/
	class CMessageFactory : public BaseSubsystems::CFactory<CMessage*>
	{
	public:

		/**
		Devuelve la única instancia de la clase CMessageFactory.
		
		@return Única instancia de la clase CMessageFactory.
		*/
		static CMessageFactory* getSingletonPtr();

		/**
		Destructor.
		*/
		~CMessageFactory();

	protected:
		/**
		Constructor.
		*/
		CMessageFactory ();

	private:
		/**
		Única instancia de la clase.
		*/
		static CMessageFactory* _instance;

	}; // class CMessageFactory

} // namespace Logic

#endif // __Logic_ComponentFactory_H