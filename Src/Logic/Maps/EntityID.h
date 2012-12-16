/**
@file EntityId.h

Contiene la definición del tipo de identificación de entidades y una 
función para generación de nuevos IDs.
 
@see Logic::TEntityID
@see Logic::EntityID

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Logic_EntityID_H
#define __Logic_EntityID_H
//---------------------------------------------------------------------------

namespace Logic 
{
	/**
	Tipo que representa el identificador de una entidad. La gestión
	de entidades garantizará que no existen dos entidades con el mismo id.
	*/
	typedef unsigned int TEntityID;

	/** 
	Clase que gestiona la generación de nuevos identificadores. Solo
	puede solicitar nuevos identificadores la factoría de entidades.
	La funcion no recicla identificadores ni comprueba que nos quedemos 
	sin identificadores para asignar (aunque en la versión DEBUG se generan 
	asserts). Esto no debería ser un problema ya que trabajamos con todo el 
	rango de unsigned int.

	@ingroup logicGroup
	@ingroup mapGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class EntityID 
	{
	public:

		/** 
		Identificadores de entidad que indican que el identificador no ha sido 
		inicializado (este identificador NO se utilizará para entidades válidas), o 
		cual es el primer identificador válido para asignar. 
		*/
		enum {	UNASSIGNED = 0xFFFFFFFF,
				FIRST_ID = 0x00000000 };

	private:
		/**
		Próximo identificador a asignar (aun no asignado).
		*/
		static TEntityID _nextId;

		/**
		Clase amiga que puede solicitar nuevos identificadores.
		*/
		friend class CEntityFactory;

		/**
		Devuelve el siguiente identificador de entidad sin asignar. La funcion no 
		recicla identificadores ni comprueba que nos quedemos sin identificadores 
		para asignar (aunque en la versión DEBUG se generan asserts). Esto no 
		debería ser un problema ya que trabajamos con todo el rango de unsigned int.

		@return Nuevo identificador.
		*/
		static TEntityID NextID();

	}; // class EntityID

}
#endif // __Logic_EntityID_H
