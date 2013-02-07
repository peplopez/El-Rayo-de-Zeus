/**
@file rtti.h

Contiene la declaración de la plantilla TAltTypeIdGenerator, que proporciona rtti
a los Componentes del proyecto.


@author AltDevBlog
@date Enero 2013
*/

#ifndef __Logic_rtti_H
#define __Logic_rtti_H


//#include <iostream>
#include "BaseSubsystems/city.h"


/**
Declaracion de tipo para identificador de clase alternativo
*/
typedef unsigned int altTypeId;

	
	
/**
Template que hace uso de typeid - tiempo constante - para devolver una cadena con el nombre de la clase
*/
template <typename T>
inline const char* GetTypeName()
{
    // aquí va un assert
	static const char* name = typeid(T).name();
	return name;
}


/**
Clase generadora de TypeID alternativo templatizada

Asumiendo que unsigned int es igual que el tipo puntero de la plataforma
garantiza devolver un id único para cada clase instanciada

El unico inconveniente es que es valor del ID no es constante en distintas ejecuciones
*/

template< typename T >
class TAltTypeIdGenerator
{
private:
	// constructor privado, clase no instanciable.
	TAltTypeIdGenerator()
	{}

public:

	// genera un typeID distinto  para cada clase que instancie este template - no constante entre ejecuciones
	static altTypeId GetAltTypeId2()
	{
		// En VS2010 sobre win32 este char ocupa exactamente 1byte
		static char iAddressOfThisIsTheTypeId;
		return reinterpret_cast< altTypeId >( &iAddressOfThisIsTheTypeId );
	}
	//genera un tipeid constante entre ejecuciones
	static altTypeId GetAltTypeId()
	{
		//Calcula la hash una única vez y la cachea para futuros usos
		static altTypeId type_id = BaseSubsystems::CityHash32( GetTypeName<T>(),strlen( GetTypeName<T>() ) );
		return type_id;
	}
};

#define GetAltTypeIdOf( TYPENAME )		( TAltTypeIdGenerator< TYPENAME >::GetAltTypeId() )

/** 
Funcion templatizada que se resuelve 
para devolver el altTypeID de una instancia
*/
template< typename T >
altTypeId GetAltTypeIdOfInstance( T instance )
{
	return TAltTypeIdGenerator< T >::GetAltTypeId();
}



#endif // __Logic_rtti_H