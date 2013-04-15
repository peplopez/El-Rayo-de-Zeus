/**
 * @file SightSensor.h
 *
 * En este fichero se define un sensor de visión básico
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */
#pragma once

#ifndef __AI_SightSensor_H
#define __AI_SightSensor_H

#include "sensor.h"


namespace AI 
{

	/**
	 * Clase que implementa un sensor de visión básico. Este sensor comprueba si la señal
	 * se encuentra dentro de un cono de visión.
	 *
	 * El cono está definido por dos parámetros: la distancia y el ángulo. La distancia indica
	 * la separación máxima que puede existir entre el sensor y la señal para que ésta sea
	 * percibida. El ángulo da el sector circular dentro del que tiene que estar la señal
	 * para poder percibirla. El ángulo se da en relación a la dirección hacia la que apunta
	 * la entidad de percepción del sensor. Por ejemplo, si la entidad está mirando en la dirección
	 * dir y el ángulo es PI/3 (60º), el cono abarcaría desde dir - PI/3 hasta dir + PI/3. Los valores
	 * para el ángulo estarán en el intervalo [0, PI].
	 *
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	class CSightSensor : public CSensor
	{
	public:
		/**
		 * Constructor. Inicializa los parámetros básicos
		 *
		 * @param pEntity Entidad a la que pertenece el sensor
		 * @param active Indica si el sensor está activo (puede percibir) o no
		 * @param threshold Intensidad mínima de la señal para que el sensor pueda percibir
		 * @param maxDistance Distancia máxima a la que puede percibirse
		 * @param alpha Ángulo del cono.
		 * @return return
		 */
		CSightSensor(CPerceptionEntity* pEntity, bool active, float threshold, float maxDistance, float alpha)
			: CSensor(pEntity, active, threshold), _maxDistance(maxDistance), _alpha(alpha) {} ;
		/**
		 * Destructor
		 */
		~CSightSensor(void);

		/**
		 * Realiza todas las comprobaciones necesarias para averiguar si la señal recibida
		 * se encuentra dentro del cono de visión.
		 *
		 * Las comprobaciones se realizan de más "barata" a más "cara"
		 * 1. Comprueba si el sensor y la señal están activos
		 * 2. Comprueba si el tipo de la señal se corresponde con el tipo que percibe el sensor.
		 * 3. Comprueba si la intensidad de la señal está por encima del threshold del sensor.
		 * 4. Comprueba la distancia entre la señal y el sensor. Se realiza en 2 pasos:
		 * 4.1. Primero se comprueba si la distancia en cada dimensión es mayor que la distancia máxima.
		 * 4.2. A continuación se comprueba si la distancia euclídea es mayor que la distancia máxima.
		 * 5. Comprobamos si la señal se encuentra dentro del ángulo de amplitud del cono de visión.
		 * 6. Comprobamos si no existe ningún objeto físico entre el sensor y la señal. Para eso usamos un rayo físico.
		 *
		 * En el momento en que alguna de estas comprobaciones falle se detiene el proceso y se devuelve
		 * NULL,  lo que significa que el sensor no es capaz de detectar la señal. Por otro lado, si todas se
		 * superan con éxito se devuelve una notificación.
		 *
		 * @param perceptible Señal cuya percepción queremos comprobar
		 * @param time Instante en el que se realiza la comprobación de percepción
		 * @return NULL si no se ha percibido la señal. Una instancia de CNotification en caso contrario.
		 */
		virtual CNotification* perceives(const CPerceptionSignal * perceptible, unsigned int time);
		/**
		 * Devuelve el tipo de sensor. En este caso un sensor de visión.
		 */
		virtual EnmPerceptionType getType() { return PERCEPTION_SIGHT; };

	private:
		/**
		 * Distancia máxima del cono
		 */
		float _maxDistance;
		/**
		 * Amplitud máxima del cono
		 */
		float _alpha;
	
	}; // class CSightSensor

} // namespace AI 

#endif __AI_SightSensor_H
