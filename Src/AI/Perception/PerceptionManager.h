/**
 * @file PerceptionManager.h
 *
 * Gestor de la percepción. En este fichero se declara la clase
 * encargada de la gestión de la percepción y algunas clases relacionadas.
 *
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionManager_H
#define __AI_PerceptionManager_H

#include <list>

#include "BaseSubsystems/Math.h"

namespace AI 
{

using namespace std;

class CPerceptionEntity;
class CSensor;

/**
 * Tipos de percepción
 */
enum EnmPerceptionType 
{
	/**
	 * Tipo desconocido
	 */
	PERCEPTION_UNKNOWN,
	/**
	 * Vista
	 */
	PERCEPTION_SIGHT,
	/**
	 * Oído
	 */
	PERCEPTION_HEAR
}; // enum EnmPerceptionType 

/**
 * Clase que representa una notificación que hace el gestor de
 * percepción a una entidad de percepción cuyo sensor ha percibido
 * una señal.
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */
class CNotification
{
public:
	/**
	 * Constructor que inicializa los parámetros. Existen dos atributos que serán excluyentes. entity se utilizará en los casos
	 * en los que la detección implique que se está percibiendo continuamente a la entidad que genera la señal (por ejemplo,
	 * en el caso de la visión). position se utiliza cuando se detecta una señal, pero no necesariamente a la entidad que la
	 * ha generado (por ejemplo, el ruido de una granada al estallar).
	 *
	 * @param time Tiempo de entrega. Instante de tiempo en que se tiene que realizar la notificación. No tiene porqué ser el mismo instante en que se crea, sino que puede ser más tarde.
	 * @param sensor Sensor que ha percibido la señal
	 * @param entity Entidad que se ha percibido
	 * @param position Posición de la señal que ha activado el sensor
	 */
	CNotification(unsigned long time, CSensor* sensor, CPerceptionEntity* perceivedEntity, Vector3 position = Vector3::ZERO) : 
		_time(time), _sensor(sensor), _perceivedEntity(perceivedEntity), _position(position) { 
		};
	/**
	 * Devuelve en qué instante se tiene que realizar la notificación
	 *
	 * @return Instante de tiempo en que se tiene que realizar la notificación
	 */
	unsigned long getTime() { return _time; };
	/**
	 * Devuelve el sensor que ha realizado la percepción
	 *
	 * @return Sensor que ha percibido la señal
	 */
	CSensor* getSensor() { return _sensor; };
	/**
	 * Devuelve la posición de la señal
	 *
	 * @return Posición de la señal que ha activado el sensor
	 */
	Vector3 getPosition() { return _position; };
	/**
	 * Devuelve la entidad que ha generado la señal percibida
	 *
	 * @return Entidad que se ha percibido
	 */
	CPerceptionEntity* getPerceivedEntity() { return _perceivedEntity; };


private:
	/**
	 * Instante en que se tiene que hacer la notificación
	 * (cuándo se ha percibido, no cuándo se ha generado la señal)
	 */
	long _time;
	/**
	 * Sensor a través del que se ha percibido
	 */
	CSensor* _sensor;
	/**
	 * Posición de la señal percibida
	 */
	Vector3 _position;
	/**
	 * Entidad que se ha percibido
	 */
	CPerceptionEntity* _perceivedEntity;
}; // class CNotification

/**
 * Clase auxiliar que compara dos notificaciones usando su tiempo de entrega.
 * Se utiliza para mantener ordenada la lista de notificaciones pendientes
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */
class CNotificationComparator 
{
public:
	bool operator() (CNotification* a, CNotification* b)
	{
		return ((a->getTime()) > (b->getTime()));
	};

}; // class CNotificationComparator 


/**
 * Clase gestora de la percepción. En esta clase se registran
 * las entidades interesadas en percibir/ser percibidas. La clase
 * se encarga de ordenar las comprobaciones necesarias cada tick
 * para que se lleve a cabo la percepción y de enviar las
 * notificaciones a los sensores correspondientes cuando han
 * percibido algo.
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */
class CPerceptionManager
{

public:
	/**
	 * Constructor por defecto
	 */
	CPerceptionManager(void) {};
	/**
	 * Destructor por defecto
	 */
	~CPerceptionManager(void) {};

	/**
	 * El método update se encarga de actualizar la información
	 * sobre los sensores de cada entidad de percepción.
	 *
	 *
	 * @param enclosing_method_arguments
	 * @return
	 */
	void update(unsigned int msecs);
	/**
	 * Registra una entidad de percepción en el gestor. Una vez que
	 * la entidad está registrada, se pueden realizar comprobaciones
	 * sobre sus sensores y señales y puede recibir notificaciones.
	 *
	 * @param entity Entidad de percepción que se va a registrar.
	 */
	void registerEntity(CPerceptionEntity* entity) { 
		cout << _entities.size() << endl;
		_entities.push_back(entity); 
	};
	/**
	 * Elimina una entidad de percepción de la lista de entidades gestionadas.
	 * Esto evita que se realicen las comprobaciones de percepción sobre sus
	 * sensores y señales y que reciba notificaciones.
	 *
	 * @param entity Entidad de percepción que se va a desregistrar
	 */
	void unregisterEntity(CPerceptionEntity* entity);

private:
	/**
	 * Lista de entidades de percepción gestionadas
	 */
	std::list<CPerceptionEntity*> _entities;
	/**
	 * Lista de notificaciones de percepción pendientes de enviar
	 */
	std::priority_queue<CNotification*, vector<CNotification*>, CNotificationComparator> _notifications;

	/**
	 * Comprueba los sensores de las entidades registradas
	 *
	 * @param time Instante de tiempo en que se realiza la comprobación
	 */
	void checkPerception(unsigned int time);
	/**
	 * Recorre la lista de notificaciones pendientes y envía las
	 * que hayan caducado
	 *
	 * @param time Instante de tiempo en que se realiza la comprobación
	 */
	void notifyEntities(unsigned int time);

}; // class CPerceptionManager

} // namespace AI 

#endif __AI_PerceptionManager_H
