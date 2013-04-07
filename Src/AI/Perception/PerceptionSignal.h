/**
 * @file PerceptionSignal.h
 *
 * En este fichero se define la clase que representa una señal de percepción.
 * Una señal representa de manera abstracta a todo aquello que puede ser percibido
 * por un sensor.
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionSignal_H
#define __AI_PerceptionSignal_H

#include "PerceptionManager.h"

namespace AI
{

	/**
	 * Clase que representa las señales de percepción.
	 * Una señal representa de manera abstracta a todo aquello que puede ser percibido
	 * por un sensor.
	 *
	 * Las propiedades de la señal son las que marcan las diferencias entre los tipos de señales.
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	class CPerceptionSignal
	{
	public:
		/**
		 * Constructor
		 *
		 * @param pEntity Entidad de percepción a la que pertenece la señal
		 * @param type Tipo de la señal. Los tipos están definidos en el enumerado AI::EnmPerceptionType en PerceptionManager.h
		 * @param intensity Intensidad de la señal
		 * @param isActive Booleano que indica si la señal está activa
		 * @param keepAlive Booleano que indica si la señal se debe mantener activa una vez que se ha procesado
		 */
		CPerceptionSignal(CPerceptionEntity* pEntity, int type, float intensity = 0.0f, float delay = 0.0f, 
			bool isActive = true, bool keepAlive = true) : _pEntity(pEntity), _active(isActive), 
			_keepAlive(keepAlive), _type(type), _intensity(intensity), _delay(delay) {};
		/**
		 * Destructor
		 */
		~CPerceptionSignal(void) {};
		/**
		 * Devuelve true si la señal está activa en este momento
		 */
		bool isActive() const { return _active;};
		/**
		 * Devuelve true si la señal debe mantenerse activa
		 */
		bool keepAlive() const { return _keepAlive; };
		/**
		 * Devuelve la intensidad de la señal
		 */
		float getIntensity() const { return _intensity; };
		/**
		 * Devuelve el tipo de la señal
		 */
		int getType() const { return _type; };
		/**
		 * Devuelve la entidad de percepción que ha lanzado la señal
		 */
		CPerceptionEntity* getPerceptionEntity() const { return _pEntity; }
		/**
		 * Devuelve el retardo de la señal
		 */
		float getDelay() { return _delay; };

	private:
		/**
		 * Entidad a la que pertenece la señal
		 */
		CPerceptionEntity* _pEntity;
		/**
		 * Indica si la señal está activa (puede ser percibida)
		 */
		bool _active;
		/**
		 * Indica si la señal se mantiene activa para siempre o
		 * si debe ser eliminada después de ser evaluada una vez
		 */
		bool _keepAlive;
		/**
		 * Tipo de la señal. Es un elemento del enumerado AI::EnmPerceptionType,
		 * definido en PerceptionManager.h
		 */
		int _type;
		/**
		 * Intensidad de la señal. Los sensores la utilizan para comprobar
		 * si la señal es lo bastante fuerte para que la detecten.
		 */
		float _intensity;
		/**
		 Retardo de la señal. Es la inversa de la velocidad a la que se 
		 desplaza la señal. Es decir, es el tiempo que tardaría la señal en 
		 recorrer 1 unidad de distancia.
		 Al usar el retardo en lugar de la velocidad podemos crear señales de
		 transmisión instantánea, dándole un retardo de 0 (la otra opción sería 
		 darle una velocidad infinita)
		 */
		float _delay;

	}; // class CPerceptionSignal

} // namespace AI

#endif __AI_PerceptionSignal_H
