//---------------------------------------------------------------------------
// Clock.h
//---------------------------------------------------------------------------

/**
@file Clock.h

Contiene la declaración de un interfaz para un temporizador.

@see Application::IClock

@author David Llansó
@date Julio, 2010
*/

#ifndef __Application_Clock_H
#define __Application_Clock_H

namespace Application 
{
	/**
	Reloj de la aplicación, que sirve para controlar el tiempo de la
	aplicación.
	
	Los métodos getTime() y getLastFrameDuration() se utilizan para
	saber "la hora" y el número de milisegundos que ocupó el
	último frame.

	El método updateTime() es el único que hace avanzar la hora,
	y está pensado para ser llamado desde el bucle principal
	de la aplicación, una vez por vuelta.

	Eso significa que entre dos llamadas a updateTime() los
	valores devueltos por getTime() y getLastFrameDuration()
	son siempre los mismos.

	Hay que hacer notar que antes de la primera invocación
	a updateTime() los métodos getTime() y getLastFrameDuration()
	devuelven 0, y que tras una única invocación a updateTime()
	el getLastFrameDuration() devolverá un valor sin sentido.
	Por lo tanto la aplicación debería invocar una primera
	vez a updateTime() antes de comenzar el bucle principal, para
	evitar ese comportamiento anómalo del primer frame.

	La clase es en realidad abstracta, pues no implementa el
	método que tiene que ver con la obtención de la hora "física"
	(ya que es dependiente de la plataforma). Ese método virtual
	puro (que aparece en la parte protegida) es el que
	accede al hardware para obtener el tiempo real.

	Todas las unidades son en milisegundos.
	
	@ingroup applicationGroup

	@author David Llansó, Marco Antonio Gómez Martín
	@date Agosto, 2010
	*/
	class IClock
	{
	public:
		/** 
		Constructor de la clase 
		*/
		IClock() : _lastTime(0), _lastFrameDuration(0) {}

		/** 
		Destructor 
		*/
		virtual ~IClock() {}

		/**
		Hace avanzar la hora del reloj, para que
		getTime() y getLastFrameDuration() devuelvan
		los nuevos valores.
		*/
		void updateTime();

		/**
		Devuelve la hora en el momento de la última invocación
		a updateTime().

		@return Hora (en milisegundos) en la última invocación a
		updateTime(). No se debe asumir que la "hora 0" es la hora
		en la que se inició el temporizador, ni el tiempo de vida
		de la aplicación, ni siquiera del sistema. La hora debe
		tomarse siempre relativa a otra anterior.
		*/
		unsigned long getTime() const { return _lastTime; }

		/**
		Devuelve la duración del último frame, o lo que es lo mismo,
		el tiempo transcurrido entre las dos últimas invocaciones
		a updateTime().

		@return Duración del último frame. Si se ha invocado menos
		de dos veces a updateTime() el resultado es indeterminado.
		*/
		unsigned int getLastFrameDuration() const { return _lastFrameDuration; }

	protected:

		/**
		 Método que devuelve la "hora física" del sistema
		 en milisegundos. Cada invocación al método debería
		 devolver una hora distinta (si la precisión del
		 reloj hardware lo permite).

		 El "sistema de referencia" del valor devuelto puede
		 ser cualquiera, es decir, no se puede asumir que
		 una hora de 1000 milisegundos signifique que la
		 aplicación llega en ejecución 1 segundo, ni siquiera
		 que el reloj se creó hace 1 segundo.

		 @return Hora en milisegundos.
		 */
		virtual unsigned long getPhysicalTime() = 0;

		/**
		Última hora registrada.
		*/
		unsigned long _lastTime;

		/**
		Duración del último frame
		*/
		unsigned int _lastFrameDuration;

	}; // IClock

} // namespace Application

#endif //  __Application_Clock_H
