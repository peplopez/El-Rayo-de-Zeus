/**
 * @file Sensor.h
 *
 * En este fichero se define la clase que representa un sensor.
 * Los sensores representan la capacidad de percibir determinados tipos de estímulo
 * por parte de las entidades.
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */
#pragma once

#ifndef __AI_Sensor_H
#define __AI_Sensor_H

#include "BaseSubsystems/Math.h"

// #include "PerceptionSignal.h"
#include "PerceptionManager.h"

namespace AI 
{

	class CPerceptionSignal;

	/**
	 * Clase abstracta que representa un sensor.
	 * Los sensores representan la capacidad de percibir determinados tipos de estímulo
	 * por parte de las entidades. Los estímulos se representan mediante señales (AI::CPerceptionSignal).
	 *
	 * Toda entidad que tiene que percibir un tipo de señal debe tener al menos un sensor asociado.
	 *
	 * Cada sensor que herede de esta clase tendrá que implementar al menos el método getType (identifica el
	 * tipo de señal que puede percibir) y perceives. El método perceives será invocado por el gestor de
	 * percepción para cada señal. En este método es donde se incluye todo el código necesario para
	 * comprobar si un sensor detecta una señal determinada.
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	class CSensor
	{
	public:
		/**
		 * Constructor
		 *
		 * @param pEntity Entidad de percepción a la que está asociado el sensor
		 * @param active Valor booleano que indica si el sensor está activo
		 * @param threshold Valor mínimo de intensidad que debe tener una señal para activar el sensor
		 * @return return
		 */
		CSensor(CPerceptionEntity* pEntity, bool active, float threshold) : 
		  _pEntity(pEntity), _active(active), _threshold(threshold) {};
		/**
		 * Destructor
		 */
		virtual ~CSensor(void) {};
		/**
		 * Este método será implementado por cada sensor con las comprobaciones necesarias para averiguar
		 * si el sensor es capaz de percibir una señal determinada.
		 *
		 * Cada sensor necesitará realizar unas comprobaciones diferentes según su tipo y sus características.
		 *
		 * Este método va a ser llamada con mucha frecuencia para cada sensor, por lo que es importante que
		 * sea bastante eficiente. En general, lo que se hace es realizar en primer lugar las comprobaciones
		 * que consumen menor tiempo de proceso, dejando las más pesadas para el final. De esta manera
		 * podemos detectar que el sensor no puede percibir una señal antes de realizar las operaciones
		 * menos eficientes.
		 *
		 * Si el sensor no puede detectar la señal devuelve NULL. Si la detecta, devuelve una nueva instancia de
		 * AI::CNotification (declarado en PerceptionManager.h) con los detalles necesarios.
		 *
		 * Es importante destacar que el Sensor produce la notificación pero NO SE RESPONSABILIZA de destruirla.
		 *
		 * @param perceptible Señal cuya percepción queremos comprobar
		 * @param time Instante en el que se realiza la comprobación de percepción
		 * @return NULL si no se ha percibido la señal. Una instancia de CNotification en caso contrario.
		 */
		virtual CNotification* perceives(const CPerceptionSignal * perceptible, unsigned int time) = 0;

		/**
		 * Tipo de sensor. Se corresponde con un elemento del enumerado AI::EnmPerceptionType (definido en
		 * PerceptionManager.h).
		 *
		 * @return Tipo de sensor
		 */
		virtual EnmPerceptionType getType() = 0;

		/**
		 * Devuelve true si el sensor está activo, false en caso contrario
		 */
		bool isActive() { return _active; };
		/**
		 * Devuelve la entidad de percepción asociada al sensor
		 */
		CPerceptionEntity* getPerceptionEntity() { return _pEntity; };
		/**
		 * Devuelve el valor mínimo de intensidad de la señal para ser percibida por el sensor
		 */
		float getThreshold() const { return _threshold; };

	protected:
		/**
		 * Indica si el sensor está activo
		 */
		bool _active;
		/**
		 * Entidad de percepción asociada al sensor
		 */
		CPerceptionEntity* _pEntity;
		/**
		 * Valor mínimo de intensidad de la señal para ser percibida por el sensor
		 */
		float _threshold;

	};

} // namespace AI 

#endif __AI_Sensor_H
