/**
@file Server.h

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __AI_Server_H
#define __AI_Server_H

namespace AI {


/**
Servidor de IA. De momento sólo se encarga de mantener el
grafo con los waypoints y de centralizar las llamadas a A*.

La idea es que en un futuro también se ocupe de gestionar la 
percepción.
*/
class CServer
{
	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor de IA.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor de IA. Esta operación sólo es necesario realizarla
		una vez durante la inicialización de la aplicación. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor de IA. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Función invocada en cada vuelta para actualización.

		@param secs Segundos desde que se reenderizó el último frame.
		@return Valor booleano indicando si todo fue bien.
		*/
		//PT ¿porque se usa un float en vez de un unsigned int, y porque secs y no msecs?
		//si al final este metodo tick en la AI esta declarado pero no implementado
		//bool tick(float secs);
		bool tick (unsigned int msecs);

	private:
		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();
		/**
		Instancia única de la clase.
		*/
		static CServer *_instance;

}; // class CServer

} // namespace AI
#endif