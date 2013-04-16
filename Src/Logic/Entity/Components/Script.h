/**
@file Script.h

Contiene la declaración del componente que contiene el control y la ejecución de los
scripts asociados a una entidad.

@author Pablo Terrado
@date Abril, 2013
*/

#ifndef __Logic_Script_H
#define __Logic_Script_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	/**
	Clase encargada de almacenar y controlar la ejecución de los scripts
	asociados a una entidad.

	@author Pablo Terrado
	@date Abril, 2013
	*/
	class CScript : public IComponent
	{
		DEC_FACTORY(CScript);
	public:

		/**
		Constructor por defecto.
		*/
		CScript() : IComponent(GetAltTypeIdOf(CScript)),_tickFunction(false), _activateFunction(false), _luaTickFunction(0) ,_luaActivateFunction(0) {}
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity).

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra así mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y ratón.
		*/
		void deactivate();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		MCameraControl.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool accept(const CMessage *message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(const CMessage *message);

	private:

		/**
		Indica si hay que llamar a una función de lua en el tick o no.
		*/
		bool _tickFunction;

		/**
		Indica si hay que llamar a una función de lua en el activate o no.
		*/
		bool _activateFunction;

		/**
		Nombre de la función de lua que se llamará en cada tick.
		*/
		const char *_luaTickFunction;

		/**
		Nombre de la función de lua que se llamará en el activate.
		*/
		const char *_luaActivateFunction;

	}; // class CScript

	REG_FACTORY(CScript);

} // namespace Logic

#endif // __Logic_Script_H