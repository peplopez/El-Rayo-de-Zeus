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
	class CAnimSet;
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
	
	enum AnimationName : unsigned short //Esto pertenece a la lógica, son nombre lógicos que tendrán su traducción a strings de animación
	{
		NONE,
		IDLE,
		WALK,
		RUN,
		DEATH,
		JUMP,
		JUMP_DOWN,
		ALERT,
		DAMAGE,
		SWITCH_ALTAR,
		COVER_WITH_WEAPON,
		COVER_WITH_SHIELD,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		COMBO1,
		COMBO2,
		COMBO3
	};

	enum Tracks : unsigned short //Posibles tracks de animaciones.
	{
		ANIMATION_BEGIN=0,
		ANIMATION_END=1,
		COMBO_TRACK=2,
		DAMAGE_TRACK=3,
		COVER_MOMENT=4,
		SWITCH_MOMENT=5
	};
	




	class CAnimatedGraphics : public CGraphics, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CAnimatedGraphics);
	public:
				/**
		Tipo de elemento que contiene el vector de tiempos, formado por un identificador de track y un tiempo float
		*/
		//std::map<Logic::AnimationName,std::vector<float>> _eventChain;

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAnimatedGraphics() : CGraphics(GetAltTypeIdOf(CAnimatedGraphics)), _graphicalEntity(0), _animSet(0), _deathAllocation(false),
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

		virtual void detachFromMap();
		virtual void attachToMap(CMap* map);
		
		////////////////////////////////////////
		// Métodos de CAnimatedEntityListener //
		////////////////////////////////////////
		/**
		Método que será invocado siempre que se termine una animación.
		Las animaciones en cíclicas no invocarán nunca este método.

		@param animation Nombre de la animación terminada.
		*/
		void animationFinished(const std::pair<unsigned short,float> track);

		void animationMomentReached(const std::pair<unsigned short,float> track);
	
	protected:
		/**
		Entidad gráfica.
		*/
		Graphics::CAnimatedEntity *_graphicalEntity;

		/**	puntero a la clase de animaciones. */
		Graphics::CAnimSet *_animSet;
				
		/**
		Animación lógica que tiene la entidad activada.
		*/
		Logic::AnimationName _currentLogicAnimation;

		/**
		Animación por defecto de una entidad gráfica animada.
		*/
		std::string _defaultAnimation;  //cambiar por animación lógica

		/**
		Método virtual que construye la entidad gráfica animada de la entidad. 
		Sobreescribe el método de la clase CGraphics.
		
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Entidad gráfica creada, NULL si hubo algún problema.
		*/
		Graphics::CEntity* createGraphicalEntity(const Map::CEntity *entityInfo);

		/**
		Método que construye el animSet de la entidad. 
		
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return puntero al conjuto de animaciones de la entidad, NULL si hubo problemas.
		*/
		bool initializeAnimSet(const Map::CEntity *entityInfo);

		/**
		Variable que dice si una entidad ha muerto y ya se ha puesto, si es necesario, una posición nueva del cadaver (para que por ejemplo no se quede flotando en el aire)
		*/
		bool _deathAllocation;
	
	
	}; // class CAnimatedGraphics

	REG_FACTORY(CAnimatedGraphics);

} // namespace Logic

#endif // __Logic_AnimatedGraphics_H
