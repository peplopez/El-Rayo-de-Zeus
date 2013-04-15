/**
 * @file PerceptionEntity.h
 *
 * En este fichero se encuentra la declaración de la clase CPerceptionEntity,
 * que representa a las entidades que pueden percibir o ser percibidas (o
 * ambas cosas) dentro del gestor de percepción.
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionEntity_H
#define __AI_PerceptionEntity_H

#include <list>
#include "PerceptionListener.h"
#include "BaseSubsystems/Math.h"



namespace AI {

class CSensor;
class CPerceptionSignal;
class CNotification;

/**
 * La clase CPerceptionEntity representa a las entidades que pueden percibir o
 * ser percibidas (o ambas cosas) dentro del gestor de percepción.
 * Cada entidad de percepción tiene una lista de sensores, que le da la capacidad
 * de percibir, y de señales, que permiten a otras entidades percibirla.
 *
 * En cada tick de IA el gestor recorrerá los sensores de la entidad y comprobará si
 * alguno de ellos se activa. Por otra parte, recorrerá las señales asociadas a la
 * entidad comprobando también si activan alguno de los sensores de las demás entidades.
 *
 * El campo userData permite asociar a la entidad de percepción con cualquier dato
 * que pueda ser útil durante el proceso de percepción (en general nos interesará
 * asociarla a la entidad lógica).
 *
 * La entidad de percepción tiene una matriz de transformación que será utilizada
 * por los sensores que así lo requieran para obtener datos como la posición o la
 * orientación. Esta matriz tiene que ser actualizada de manera explícita (en
 * general, por la entidad lógica a la que esté asociada la entidad de percepción).
 *
 *
 *
 * @author Gonzalo Flórez
 * @date 11/04/2011
 */
class CPerceptionEntity
{
public:
	/**
	 * Constructor
	 *
	 * @param userData Datos de usuario asociados a la entidad de percepción
	 * @param listener Clase que será notificada cuando la entidad de percepción reciba una notificación
	 * @param radius Radio de la entidad de percepción
	 */
	CPerceptionEntity(void* userData, IPerceptionListener* listener, float radius) : _userData(userData), _listener(listener), _radius(radius) {};
	/**
	 * Destructor
	 * El destructor se responsabiliza de eliminar el contenido de las listas de sensores y de señales de la entidad
	 */
	~CPerceptionEntity(void);

	/**
	 * Devuelve los datos de usuario asociados a la entidad de percepción
	 */
	void* getUserData() {return _userData;};
	// TODO Esto tendría que devolver un const y que sea referencia
	/**
	 * Devuelve la lista de sensores de la entidad
	 */
	const std::list<CSensor*>& getSensors() { return _sensors; };
	// TODO Esto tendría que devolver un const y que sea referencia
	/**
	 * Devuelve la lista de señales de la entidad
	 */
	const std::list<CPerceptionSignal*>& getSignals() { return _signals; };
	/**
	 * Añade un sensor a la entidad
	 */
	void addSensor(CSensor* sensor) { _sensors.push_back(sensor); };
	/**
	 * Añade una señal a la entidad
	 */
	void addSignal(CPerceptionSignal* signal) { _signals.push_back(signal); };
	/**
	 * Elimina un sensor de la entidad
	 */
	void removeSensor(CSensor* sensor);
	/**
	 * Elimina una señal de la entidad
	 */
	void removeSignal(CPerceptionSignal* signal);
	/**
	 * Envía una notificación a su listener
	 */
	void sendNotification(CNotification* notification) { _listener->notificationPerceived(notification); };
	/**
	 * Actualiza la matriz de transformación de la entidad de percepción
	 */
	void setTransform(const Matrix4& transform) { _transform = transform; };
	/**
	 * Devuelve la matriz de transformación de la entidad de percepción
	 */
	Matrix4& getTransform() { return _transform; };
	/**
	 * Devuelve el radio de la entidad de percepción
	 */
	float getRadius() { return _radius; };

private:
	/**
	 * Datos de usuario
	 */
	void* _userData;
	/**
	 * Lista de sensores
	 */
	std::list<CSensor*>  _sensors;
	/**
	 * Lista de señales
	 */
	std::list<CPerceptionSignal*>  _signals;
	/**
	 * Clase que será notificada cuando alguno de los sensores se active
	 */
	IPerceptionListener* _listener;
	/**
	 * Matriz de transformación
	 */
	Matrix4 _transform;
	/**
	 * Radio de la entidad de percepción
	 */
	float _radius;
	
}; // class CPerceptionEntity

} // namespace AI 

#endif __AI_PerceptionEntity_H
