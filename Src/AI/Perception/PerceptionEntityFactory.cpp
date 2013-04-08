#include "PerceptionEntityFactory.h"

#include "SightSensor.h"
#include "PerceptionSignal.h"

namespace AI 
{

	/**
	 * Devuelve una entidad de percepción a partir de una cadena de texto que identifica su tipo.
	 *
	 * @param type Tipo de entidad de percepción. Por ahora sólo admite "enemy" y "player"
	 * @param radius Radio de la entidad de percepción
	 * @param userData Datos de usuario asociados a la entidad de percepción
	 * @param listener Listener que será notificado cuando la entidad perciba una señal
	 * @return CPerceptionEntity del tipo correspondiente al parámetro type
	 */
	CPerceptionEntity* CPerceptionEntityFactory::getPerceptionEntity(string type, float radius, void* userData, IPerceptionListener* listener)
	{
		if (type == "enemy") {
			return new CPerceptionEntityEnemy(userData, listener, radius);
		} else if (type == "player") {
			return new CPerceptionEntityPlayer(userData, listener, radius);
		} else {
			return NULL;
		}
	}

	/**
	 * Crea una entidad de percepción que representa a un enemigo.
	 *
	 * Esta clase sólo tiene un sensor de visión básico (AI::CSightSensor), inicializado
	 * con los siguientes parámetros:
	 * · active = true
	 * · threshold = 1.0
	 * · maxDistance = 200.0
	 * · alpha = PI / 4
	 *
	 * Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración externo.
	 *
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	CPerceptionEntityEnemy::CPerceptionEntityEnemy(void* userData, IPerceptionListener* listener, float radius) :
		CPerceptionEntity(userData, listener, radius)
	{
		// Esta clase sólo tiene un sensor de visión básico (AI::CSightSensor), inicializado
		// con los siguientes parámetros:
		// · active = true
		// · threshold = 1.0
		// · maxDistance = 200.0
		// · alpha = PI / 4
		//
		// Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración externo.

		CSightSensor * sight = new CSightSensor(this, true, 1.0f, 200.0f, Math::PI / 4.0f);
		this->addSensor(sight);
	}

	/**
	 * Clase de entidad de percepción que representa al jugador
	 *
	 * Esta clase sólo tiene una señal con los siguientes parámetros:
	 * · type = PERCEPTION_SIGHT (es decir, que la señal es de visibilidad)
	 * · intensity = 1.0
	 * · delay = 0.0 (instantánea)
	 * · isActive = true
	 * · keepAlive = true (la señal no se destruye después de un ciclo de percepción, sino que sigue activa)
	 *
	 * Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
	 *
	 *
	 * @author Gonzalo Flórez
	 * @date 11/04/2011
	 */
	CPerceptionEntityPlayer::CPerceptionEntityPlayer(void* userData, IPerceptionListener* listener, float radius) :
		CPerceptionEntity(userData, listener, radius)
	{
		// Esta clase sólo tiene una señal con los siguientes parámetros:
		// · type = PERCEPTION_SIGHT (es decir, que la señal es de visibilidad)
		// · intensity = 1.0
		// · delay = 0.0 (instantánea)
		// · isActive = true
		// · keepAlive = true (la señal no se destruye después de un ciclo de percepción, sino que sigue activa)
		//
		// Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.

		CPerceptionSignal* signal = new CPerceptionSignal(this, PERCEPTION_SIGHT, 1.0f, 0.0f, true, true);
		this->addSignal(signal);
	}


}
