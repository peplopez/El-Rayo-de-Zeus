/**
@file ComponentFactory.cpp

Contiene la implementación de la clase CComponentFactory, Singleton que almacena 
los punteros a función de funciones de creación componentes (IComponent).

@see Logic::CComponentFactory
@see BaseSubsystems::CFactory
@see Logic::CComponent

@author David Llansó
@date Julio, 2010
*/
#include "ComponentFactory.h"
#include "Logic/Entity/Component.h"

#include <cassert>

namespace Logic {

	CComponentFactory* CComponentFactory::_instance = 0;

	//--------------------------------------------------------

	CComponentFactory::CComponentFactory() : CFactory()
	{
		_instance = this;

	} // CComponentFactory

	//--------------------------------------------------------

	CComponentFactory::~CComponentFactory()
	{
		_instance = 0;

	} // ~CComponentFactory
	
	//--------------------------------------------------------

	CComponentFactory* CComponentFactory::getSingletonPtr()
	{
		if(!_instance)
			new CComponentFactory();

		return _instance;

	} // Init

} // namespace Logic
