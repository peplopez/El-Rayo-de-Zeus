/**
@file PhysicCharacter.h

Contiene la declaración del componente que se utiliza para representar jugadores y enemigos en
el mundo físico usando character controllers.

@see Logic::CPhysicCharacter
@see Logic::CPhysicEntity

@author Antonio Sánchez Ruiz-Granados
@date Octubre, 2010
*/
#ifndef __Logic_PhysicCharacter_H
#define __Logic_PhysicCharacter_H

#include "PhysicEntity.h"

// Los componentes se definen dentro del namespace Logica
namespace Logic 
{
	/**
	Componente que se utiliza para representar jugadores y enemigos en el mundo físico usando 
	character controllers.
	<p>
	Este componente recibe mensajes de tipo AVATAR_WALK para mover el controller.
	<p>
	La implementación de esta clase también gestiona las diferencias que existen entre el
	sistema de coordenadas local (SCL) lógico y físico. El SCL que usa la lógica para cada 
	entidad tiene su origen en la parte inferior de la entidad (los pies), mientras que 
	PhysX coloca por defecto el origen del SCL en medio de la entidad. En las entidades 
	físicas simples, podemos unificar los dos SCL definiendo una sencilla traslación en la 
	descripción del modelo, pero la descripción de los character controllers no ofrece esa
	posibilidad. La conclusión es que debemos realizar una traslación en el eje Y para 
	convertir entre coordenadas lógicas y coordenadas físicas.
	
    @ingroup logicGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Octubre, 2010
	*/
	class CPhysicCharacter : public CPhysicEntity
	{
		DEC_FACTORY(CPhysicCharacter);
	public:

		/**
		Constructor por defecto.
		*/
		CPhysicCharacter();

		/**
		Destructor (virtual).
		*/
		virtual ~CPhysicCharacter();

		/**
		Este componente sólo acepta mensajes de tipo AVATAR_WALK.
		*/
		virtual bool accept(const TMessage &message);
		
		/**
		Si se recibe un mensaje de tipo AVATAR_WALK, se almacena su vector de 
		desplazamiento para posteriormente mover el character controller en el tick.
		De este moto, si en un ciclo se reciben varios mensaje de tipo SET_TRANSFORM 
		sólo tendrá efecto el último que se reciba.
		*/
		virtual void process(const TMessage &message);

		/**
		Este método se invoca en cada ciclo de la simulación y hace lo siguiente:
		<ul>
		<li>Actualiza la posición de la entidad lógica usando la información que proporciona
		el motor de física.</li>
		
		<li>Mueve el character controller de acuerdo al último mensaje AVATAR_WALK recibido.</li>
		</ul>
		<p>
		Necesitamos sobreescribir el método de la clase padre ya que los character controllers
		no tienen orientación, sólo posición. Además debemos aplicar una traslación al vector 
		de posición que obtenemos de PhysX ya que la lógica y la física	usan distintos orígenes 
		para los sistemas de coordenadas locales a la entidad (en los pies / en el centro de la 
		entidad).
		*/
		virtual void tick(unsigned int msecs);

	protected:

		/**
		Debemos sobreescribir este método de la clase padre para aplicar una traslación
		al character controller una vez creado (conversión entre coordenadas lógicas y físicas).
		*/
		virtual IPhysicObj* createPhysicEntity(const Map::CEntity *entityInfo);

		/**
		Creamos un modelo para describir el character controller físico.
		 */
		virtual IPhysicModel* createPhysicModel(const Map::CEntity *entityInfo);

	private:
		/**
		Tranforma posiciones lógicas a posiciones físicas.
		 */
		Vector3 fromLogicToPhysics(const Vector3 &v) { return Vector3(v.x, v.y + _offsetY, v.z); };

		/**
		Transforma posiciones físicas a posiciones lógicas.
		*/
		Vector3 fromPhysicsToLogic(const Vector3 &v) { return Vector3(v.x, v.y - _offsetY, v.z); };

		/**
		Desplazamiento en el eje Y para transformar entre coordenadas lógicas y coordenadas
		físicas.
		 */
		float _offsetY;

		/**
		Vector de desplazamiento recibido en el último mensaje de tipo AVATAR_WALK. 
		*/
		Vector3 _movement;

		/**
		Indica si el character controller esta apoyado sobre una superficie o cayendo.
		*/
		bool _falling;

	}; // class CPhysicCharacter

	REG_FACTORY(CPhysicCharacter);

} // namespace Logic

#endif // __Logic_PhysicCharacter_H
