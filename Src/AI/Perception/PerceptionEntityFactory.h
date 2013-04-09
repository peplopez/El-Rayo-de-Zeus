/**
 * @file PerceptionEntityFactory.h
 *
 * En este fichero definimos una factoría para crear los distintos tipos de entidades de percepción. En este caso
 * sólo hay dos: "enemy" y "player". "enemy" tiene un sensor de visión básico (CSightSensor) y "player" tiene
 * una señal de visión para ser percibido por el enemigo.
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionEntityFactory_H
#define __AI_PerceptionEntityFactory_H

#include "PerceptionEntity.h"

using namespace std;

namespace AI
{
	/**
	 * Clase factoría para crear las entidades de percepción.
	 *
	 * Tiene un método estático, getPerceptionEntity, que recibe una cadena que identifica
	 * la entidad de percepción y algunos parámetros, y devuelve una nueva entidad de percepción
	 * del tipo correspondiente.
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	class CPerceptionEntityFactory
	{
	public:
		/**
		 * Devuelve una entidad de percepción a partir de una cadena de texto que identifica su tipo.
		 *
		 * @param type Tipo de entidad de percepción. Por ahora sólo admite "enemy" y "player"
		 * @param radius Radio de la entidad de percepción
		 * @param userData Datos de usuario asociados a la entidad de percepción
		 * @param listener Listener que será notificado cuando la entidad perciba una señal
		 * @return CPerceptionEntity del tipo correspondiente al parámetro type
		 */
		static CPerceptionEntity* getPerceptionEntity(string type, float radius, void* userData, IPerceptionListener* listener);

	};  // class CPerceptionEntityFactory

	/**
	 * Clase de entidad de percepción que representa a un enemigo
	 *
	 * Esta clase sólo tiene un sensor de visión básico (AI::CSightSensor), inicializado
	 * con los siguientes parámetros:
	 * · active = true
	 * · threshold = 1.0
	 * · maxDistance = 200.0
	 * · alpha = PI / 4
	 *
	 * Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
	 *
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	class CPerceptionEntityEnemy :
		public AI::CPerceptionEntity
	{

	public:
		CPerceptionEntityEnemy(void* userData, IPerceptionListener* listener, float radius);
		~CPerceptionEntityEnemy(void) {};

	}; // class CPerceptionEntityEnemy

	/**
	 * Clase de entidad de percepción que representa al jugador
	 *
	 * Esta clase sólo tiene una señal con los siguientes parámetros:
	 * · type = PERCEPTION_SIGHT (es decir, que la señal es de visibilidad)
	 * · intensity = 1.0
	 * · isActive = true
	 * · keepAlive = true (la señal no se destruye después de un ciclo de percepción, sino que sigue activa)
	 *
	 * Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
	 *
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	class CPerceptionEntityPlayer :
		public AI::CPerceptionEntity
	{
	public:
		CPerceptionEntityPlayer(void* userData, IPerceptionListener* listener, float radius);
		~CPerceptionEntityPlayer(void) {};
	}; //class CPerceptionEntityPlayer

} //namespace AI

#endif __AI_PerceptionEntityFactory_H
