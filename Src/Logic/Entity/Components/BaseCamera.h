/**
@file Camera.h

Contiene la declaración del componente que controla la cámara gráfica
de una escena.

@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Septiembre, 2010
*/
#ifndef __Logic_BaseCamera_H
#define __Logic_BaseCamera_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"
// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CCamera;
	class CScene;
}

namespace Logic
{
	class CMessage;
}
//declaración de la clase
namespace Logic 
{
/**
*/
	class CBaseCamera : public IComponent
	{
		DEC_FACTORY(CBaseCamera);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CBaseCamera() : IComponent(GetAltTypeIdOf(CBaseCamera)), _graphicsCamera(0), _currentPos(Vector3::ZERO), _degree(0), _radius(0), 
			_height(0), _angularSpeed(0) {}
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la cámara gráfica y se leen atributos del mapa.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		void deactivate();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la cámara siguiendo al jugador.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);
		
		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool accept(const CMessage *message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(CMessage *message);


	protected:
		
		/**
		Cámara gráfica.
		*/
		Graphics::CCamera *_graphicsCamera;
		
		/**
		*/
		Vector3 _currentPos;		
	
		/**
		*/
		float _degree;
		/**
		*/
		float _radius;

		/**
		*/
		float _height;

		/**
		*/
		float _angularSpeed;



	}; // class CBaseCamera

	REG_FACTORY(CBaseCamera);

} // namespace Logic

#endif // __Logic_Camera_H
