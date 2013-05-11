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
#include <string>

namespace Graphics 
{
	enum AnimationName : unsigned int 
	{	
		IDLE,				
		RUN,
		DEATH,	
		JUMP,
		DAMAGE,
		ACTIVATE_ALTAR,	
		COVER_WITH_WEAPON,
		COVER_WITH_SHIELD,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		COMBO1,
		COMBO2,
		COMBO3
	};

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
		bool addAnimation(const Graphics::AnimationName animEnum, const std::string realName);
		
		/**
		Obtiene el nombre físico de la animación a partir de su nombre lógico.

		@param anim Nombre de la animación a activar.
		@return true si la animación solicitada fue correctamente desactivada.
		*/
		std::string getAnimation(const Graphics::AnimationName animEnum);
	
	private:

		std::map<Graphics::AnimationName,std::string> _animationSet; 

	}; // class CAnimSet

} // namespace Graphics

#endif // __Graphics_AnimSetEntity_H
