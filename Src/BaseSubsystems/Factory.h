/**
@file Factory.h

Contiene la declaración de la plantilla CFactory, factoría que 
almacena los punteros a función de funciones de creación de clases 
tel tipo parametrizado.

@see BaseSubsystems::CFactory

@author David Llansó
@date Julio, 2010
*/
#ifndef __BaseSubsystems_Factory_H
#define __BaseSubsystems_Factory_H

#include <string>
#include <map>
#include <exception>

#include "Rtti.h"
#include "City.h"



// Declaración de la clase
namespace BaseSubsystems
{

	/**
	Factoría parametrizada de objetos. Permite registrar punteros a función de 
	creación de cualquier tipo de clase. Dichas funciones no pueden tener 
	parámetros de entrada y como salida deven devolver un objeto de tipo T.
	<p>
	Como ejemplo esta factoría es usada para almacenar componentes en el singleton
	Logic::CComponentFactory donde todo el registro de componentes se realiza 
	utilizando macros del preprocesador.

	@author David Llansó
	@date Julio, 2010
	@ingroup baseSubsystemsGroup
	 */
	template <class T>
	class CFactory
	{
	protected:
		
		/** 
		Puntero a función.  
		*/
		typedef T (*FunctionPointer)();

	public:
		
		/**
		Constructor.
		*/
		CFactory () {}

		/**
		Destructor.
		*/
		virtual ~CFactory();

		/**
		Añade una nueva función a la tabla de la factoría.

		@param funcion Función de creación del objeto.
		@param name Nombre con el que nos referiremos a la función para
		la futura creación del objeto.
		*/
		void add(FunctionPointer function, const std::string& name);

		void add(FunctionPointer function, altTypeId name);

		/**
		Si una función ya está contenida en la tabla.

		@param name Nombre de la clase del objeto que se quiere saber si 
		se puede crear.
		@return true si se encuentra en la tabla.
		*/
		bool has(const std::string& name) const;

		bool has(const altTypeId name) const;
		/**
		Crea una nueva instancia de la clase requerida.

		@param name Nombre de la clase del objeto que se quiere crear.
		@return Nueva instancia de la clase. 0 si no está en la tabla.
		*/
		T create(const std::string& name) const;

		T create(altTypeId) const;

	protected:

		typedef std::pair <altTypeId,FunctionPointer> TStringFunctionPointerPair;
		typedef std::map<altTypeId,FunctionPointer> TFunctionPointerMap;

		/** 
		Tabla con los punteros a función.
		*/
		TFunctionPointerMap _table;

	}; // class CFactory
	
	//--------------------------------------------------------

	template <class T> 
	inline CFactory<T>::~CFactory()
	{
		TFunctionPointerMap::const_iterator it = _table.begin();
		TFunctionPointerMap::const_iterator it2;
		
		while( it != _table.end() )
		{
			it2 = it;
			it++;
			_table.erase(it2);
		}

	} // ~CFactory

	//--------------------------------------------------------

	template <class T> 
	inline void CFactory<T>::add(FunctionPointer function, const std::string& name)
	{
		const char * c = name.c_str();
		altTypeId typeId = BaseSubsystems::CityHash32(c,strlen(c));
		TStringFunctionPointerPair element(typeId,function);
		_table.insert(element);

	} // add
	//--------------------------------------------------------

	template <class T> 
	inline void CFactory<T>::add(FunctionPointer function, altTypeId name)
	{

		TStringFunctionPointerPair element(name,function);
		_table.insert(element);

	} // add

	//--------------------------------------------------------

	template <class T> 
	inline bool CFactory<T>::has(const std::string& name) const
	{
		const char * c = name.c_str();
		altTypeId typeId = BaseSubsystems::CityHash32(c,strlen(c));
		return _table.count(typeId) > 0;

	} // has

	//--------------------------------------------------------

	template <class T> 
	inline bool CFactory<T>::has(altTypeId name) const
	{

		return _table.count(name) > 0;

	} // has

	//--------------------------------------------------------

	template <class T> 
	inline T CFactory<T>::create(const std::string& name) const
	{
		const char * c = name.c_str();
		altTypeId typeId = BaseSubsystems::CityHash32(c,strlen(c));
		if(has(name))
		{
			TFunctionPointerMap::const_iterator it;
			it = _table.find(typeId);
			if( it != _table.end() )
				return it->second();
		}
		throw new std::exception("No existe la función de creación que se solicitó.");

	} // create

	//--------------------------------------------------------
	template <class T> 
	inline T CFactory<T>::create(altTypeId name) const
	{
		if(has(name))
		{
			TFunctionPointerMap::const_iterator it;
			it = _table.find(name);
			if( it != _table.end() )
				return it->second();
		}
		throw new std::exception("No existe la función de creación que se solicitó.");

	} // create

} // namespace BaseSubsystems

#endif // __BaseSubsystems_Factory_H
