/**
@file MessageFactory.cpp

Contiene la implementación de la clase CMessageFactory, Singleton que almacena 
los punteros a función de funciones de creación componentes (CMessage).

@see Logic::MessageFactory
@see BaseSubsystems::CFactory
@see Logic::CMessage

@author David Llansó
@date Julio, 2010
*/
#include "MessageFactory.h"


#include <cassert>

namespace Logic {

	CMessageFactory* CMessageFactory::_instance = 0;

	//--------------------------------------------------------

	CMessageFactory::CMessageFactory() : CFactory()
	{
		_instance = this;

	} // CMessageFactory

	//--------------------------------------------------------

	CMessageFactory::~CMessageFactory()
	{
		_instance = 0;

	} // ~CMessageFactory
	
	//--------------------------------------------------------

	CMessageFactory* CMessageFactory::getSingletonPtr()
	{
		if(!_instance)
			new CMessageFactory();

		return _instance;

	} // Init

} // namespace Logic