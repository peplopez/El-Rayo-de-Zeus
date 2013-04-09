/**
@file AnimatedGraphics.h

Contiene la declaración del componente que controla la representación
gráfica de una entidad estática.

@see Logic::CAnimatedGraphics
@see Logic::CGraphics

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_AnimatedGraphics_H
#define __Logic_AnimatedGraphics_H

#include "Graphics.h"
#include "Graphics/AnimatedEntity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CAnimatedEntity;
}
namespace Logic
{
	class CMessage;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representación gráfica animada de una 
	entidad. Especializa a la clase CGraphics para que la entidad gráfica
	creada sea animada.
	<p>
	Además de los mensajes de cambio de posición y de cambio de orientación
	que acepta CGraphics, acepta mensajes para poner o para una animación
	(SET_ANIMATION y STOP_ANIMATION).
	
    @ingroup logicGroup

	@author David Llansó García
	@date Agosto, 2010
*/
	class CAnimatedGraphics : public CGraphics, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CAnimatedGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAnimatedGraphics() : CGraphics(GetAltTypeIdOf(CAnimatedGraphics)), _graphicalEntity(0),
				_defaultAnimation("") {}

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		SET_ANIMATION y STOP_ANIMATION.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool accept(const CMessage *message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(CMessage *message);
		
		////////////////////////////////////////
		// Métodos de CAnimatedEntityListener //
		////////////////////////////////////////
		/**
		Método que será invocado siempre que se termine una animación.
		Las animaciones en cíclicas no invocarán nunca este método.

		@param animation Nombre de la animación terminada.
		*/
		void animationFinished(const std::string &animation);
		void animationMomentReached(const std::string &animation);

		virtual Graphics::CEntity* CAnimatedGraphics::reCreateGraphicalEntity();
	
	
	protected:
		/**
		Entidad gráfica.
		*/
		Graphics::CAnimatedEntity *_graphicalEntity;

				
		/**
		Animación por defecto de una entidad gráfica animada.
		*/
		std::string _defaultAnimation;

		/**
		Método virtual que construye la entidad gráfica animada de la entidad. 
		Sobreescribe el método de la clase CGraphics.
		
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Entidad gráfica creada, NULL si hubo algún problema.
		*/
		Graphics::CEntity* createGraphicalEntity(const Map::CEntity *entityInfo);



	}; // class CAnimatedGraphics

	REG_FACTORY(CAnimatedGraphics);

} // namespace Logic

#endif // __Logic_AnimatedGraphics_H
