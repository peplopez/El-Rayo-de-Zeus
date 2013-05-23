//---------------------------------------------------------------------------
// AnimSet.h
//---------------------------------------------------------------------------

/**
@file AnimSet.h

Contiene la declaración de la clase que reprenta una correspondencia entre nombrés lógicos de animación y nombres físicos

@see Graphics::CAnimSet

@author Jose Luis López	
@date Mayo, 2013
*/

#ifndef __Graphics_AnimSet_H
#define __Graphics_AnimSet_H

#include <map>
#include <vector>
#include <string>
#include "Logic\Entity\Components\AnimatedGraphics.h"

namespace Graphics 
{
	
	/**
	Clase que representa una entidad gráfica con animaciones. Especializa la clase
	Graphics::CEntity por lo que contiene una referencia a una entidad de 
	Ogre y al Nodo que la contiene. Añade la posibilidad de activar una animación 
	y reproducirla.
	<p>
	Esta clase es bastante sencilla y solo controlamos una animación activa.
	Podría ser más sofisticada permitiendo interpolación de animaciones o avisando
	mediante observers cuando una animación termina de reproducirse.
	<p>
	Oculta los detalles escabrosos del motor gráfico.
	
	@ingroup graphicsGroup

	@author Jose Luis López	
	@date Mayo, 2013
	*/
	class CAnimSet
	{
	public:

		/**
		Constructor de la clase.

		@param entityInfo información del Map para leer la información acerca de la animación
		*/
		CAnimSet();



		/**
		Destructor de la clase.

		@param entityInfo información del Map para leer la información acerca de la animación
		*/
		~CAnimSet();

		/**
		Asigna un nombre de animación lógico a un string correcpondiente a la animación física.

		@param animEnum enumerado que estamos seteando
		@param realName nombre físico leido del map
		@return true si el metodo termina de forma satisfactoria
		*/
		bool addAnimation(const Logic::AnimationName animEnum, const std::string realName);
		
		/**
		Obtiene el nombre físico de la animación a partir de su nombre lógico.

		@param anim Nombre de la animación a activar.
		@return std::string devuelve el nombre físico de la animación
		*/
		std::string getAnimation(const Logic::AnimationName animEnum);

		/**
		Añade un evento de tiempo  leido desde el Map.

		@param animEnum enumerado que estamos seteando
		@param eventTime tiempo que queremos almacenar
		@return true si el metodo termina de forma satisfactoria
		*/
		bool addEventTime(const Logic::AnimationName animEnum, const Logic::Tracks, const float eventTime);

		/**
		Obtiene la cadena de eventos

		@param anim Nombre de la animación a la que acceder.
		@return std::list<float> devuelve la lista de eventos asociada a la animación.
		*/
		std::vector<std::pair<unsigned short,float>>* getEventChain(const Logic::AnimationName animEnum);
	
	private:

		/**
		Map de nombres de animaciones correspondientes a los enum Lógicos
		*/
		std::map<Logic::AnimationName,std::string> _animationSet; 
		


		/**
		Map que guarda una lista de eventos para una animación Lógica.
		*/
		std::map<Logic::AnimationName,std::vector<std::pair<unsigned short,float>>> _eventChain;

	}; // class CAnimSet

} // namespace Graphics

#endif // __Graphics_AnimSetEntity_H
