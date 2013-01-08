#include "LatentAction.h"

namespace AI 
{
	/**
	Método llamado cíclicamente por el responsable de la ejecución
	de la función latente.
	@return Devuelve el estado de la función latente: si ésta ha
	terminado su ejecución, si necesita que se siga invocando a
	la función Tick() cíclicamente o si, aún sin haber terminado,
	no necesita (de momento) la invocación a Tick().
	*/
	CLatentAction::LAStatus CLatentAction::tick() 
	{

		// ¿Hay que empezar la tarea?
		if (_status == READY) {
			_status = this->OnStart();
		}

		// Llamamos al Tick, si el OnStart no terminó
		// con la ejecución
		if ((_status == RUNNING) && !_finishing)
			_status = this->OnRun();

		// Si OnRun() terminó, llamamos al OnStop() y terminamos;
		// si estábamos terminando (se solicitó la terminación
		// de forma asíncrona), también.
		if ((_status == SUCCESS) || (_status == FAIL) || (_finishing)) {
			this->OnStop();
			_finishing = false;
		}
		return _status;
	} // tick

	/**
	Cancela la tarea que se está ejecutando; se entiende que este
	método es llamado cuando el comportamiento al que pertenece
	la tarea es anulado.
	*/
	void CLatentAction::abort() {
		_finishing = false;
		_status = this->OnAbort();
	}

	/**
	Reinicia la acción, que queda en un estado listo para ser ejecutada
	de nuevo (READY). Dependiendo del estado de la acción en el momento
	de llamar a reset (si éste es RUNNING o SUSPENDED) se considera que 
	la acción ha sido abortada y se llama a OnAbort.
	*/
	void CLatentAction::reset() {
		// Sólo hacemos algo si ya hemos empezado a ejecutar la acción
		if (_status != READY) {
			// Si estamos en ejecución (normal o suspendida) 
			// tenemos que llamar a onAbort (porque en realidad
			// abortamos la acción)
			if (_status == RUNNING || _status == SUSPENDED)
				this->OnAbort();
			// Dejamos el estado listo para volver a ejecutarla
			_status = READY;
			_finishing = false;
		}
	}


}// namespace AI 