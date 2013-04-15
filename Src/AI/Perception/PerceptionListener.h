/**
 * @file PerceptionListener.h
 *
 * Definición de la interfaz PerceptionListener, que implementarán aquellas
 * clases que deseen ser notificadas cuando se active un sensor de una entidad
 * de percepción
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionListener_H
#define __AI_PerceptionListener_H



namespace AI
{
	class CNotification;

	/**
	 * Interfaz que implementarán aquellas
	 * clases que deseen ser notificadas cuando se active un sensor de una entidad
	 * de percepción
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	class IPerceptionListener
	{
	public:
		/**
		 * Método invocado por el gestor de percepción cuando recibe una notificación de un
		 * sensor de la entidad de percepción.
		 *
		 * @param notification Notificación recibida
		 */
		virtual void notificationPerceived(CNotification* notification) = 0;
		/**
		 * Destructor
		 */
		virtual ~IPerceptionListener(void) {};
	};

} // namespace AI

#endif __AI_PerceptionListener_H
