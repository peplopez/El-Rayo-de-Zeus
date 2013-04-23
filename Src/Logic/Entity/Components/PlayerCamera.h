/**
@file PLayerCamera.h

Contiene la declaraci�n del componente que controla la c�mara gr�fica
de una escena.

@see Logic::CCamera
@see Logic::IComponent

@author David Llans�
@date Septiembre, 2010
*/
#ifndef __Logic_PlayerCamera_H
#define __Logic_PlayerCamera_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"
// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CCamera;
	class CScene;
}

namespace Logic
{
	class CMessage;
}
//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de mover la c�mara gr�fica de una escena, para que 
	�sta se pueda reenderizar, siguiendo al jugador.
	<p>
	La c�mara se situar� a una distancia detr�s del jugador y a una altura del 
	suelo y enfocar� a un punto imaginario por delante del jugador que estar� 
	tambi�n a cierta altura del suelo. Todas estas caracter�sticas son configurables 
	desde la declaraci�n del mapa definiendo los nombres de atributo "distance",
	"height", "targetDistance" y "targetHeight".
	<p>
	El componente ir� recalculando en cada ciclo las posiciones de la c�mara y del
	punto de mira en funci�n de la posici�n y orientaci�n del jugador.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Septiembre, 2010
*/
	class CPlayerCamera : public IComponent
	{
		DEC_FACTORY(CPlayerCamera);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CPlayerCamera() : IComponent(GetAltTypeIdOf(CPlayerCamera)), _graphicsCamera(0), _distance(0), _height(0), _target(0), _targetDistance(0), _targetHeight(0),
				_currentPos(Vector3::ZERO), _cameraSpeed(0) {}
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la c�mara gr�fica y se leen atributos del mapa.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		void deactivate();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la c�mara siguiendo al jugador.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);
		
		/**
		M�todo que lleva la camara a una altura determinada. Se usar� en saltos en la propia base, teletransporte a otra base y podr�a usarse tambien para visualizar otra base.

		<p>
		Se encarga de mover la c�mara siguiendo al jugador.

		@param base short para la base
		@param ring LogicalPosition::Ring anillo
		*/
		void changeHeight(const unsigned short &base, const LogicalPosition::Ring &ring);

				/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool accept(const CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(CMessage *message);


	protected:
		
		/**
		C�mara gr�fica.
		*/
		Graphics::CCamera *_graphicsCamera;

		/**
		Entidad que se usa como objetivo
		*/
		CEntity *_target;

		Vector3 _currentPos;

		Vector3 _centerWorldPosition;

		/**
		Distancia de la c�mara respecto a la entidad objetivo. Es distancia
		sobre el plano XZ, la altura tiene su propio atributo.
		*/
		float _distance;
		
		/**
		altura de la c�mara respecto del suelo o plano XZ.
		*/
		float _height;

		/**
		Distancia del punto al que mirar� la c�mara respecto a la entidad 
		objetivo. Es distancia sobre el plano XZ, la altura tiene su propio 
		atributo.
		*/
		float _targetDistance;
		
		/**
		altura del punto al que mirar� la c�mara respecto del suelo o plano XZ.
		*/
		float _targetHeight;

		/**
		*/
		float _cameraSpeed;




	}; // class CCamera

	REG_FACTORY(CPlayerCamera);

} // namespace Logic

#endif // __Logic_PlayerCamera_H