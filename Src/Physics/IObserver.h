/**Clase abstracta de la que deben heredar todos los componentes físicos. 
	Proporciona un interfaz	común para recibir eventos desde el motor de física.
	@author ƒ®§
	@date Febrero, 2013
*/

#ifndef __Physics_IObserver_H
#define __Physics_IObserver_H


// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {
	
	class IObserver
	{
	public: 		
		/**Este método es invocado desde el motor de física cuando una entidad entra o sale de un
		trigger físico. Se notifica tanto al componente asociado al trigger como al componente
		asociado a la otra entidad.
		@param otherComponent Componente asociado al trigger o a la otra entidad, 
				dependiendode a quién se esté notificando.
		@param enter True si la entidad entra en el trigger y false si sale. 
		*/

		virtual ~IObserver() {};
		virtual void onTrigger(IObserver* other, bool enter) = 0;
		virtual void onCollision(IObserver* other) = 0;
	}; // class IObserver

} // namespace Physics


#endif // __Physics_IObserver_H