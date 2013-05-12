//---------------------------------------------------------------------------
// AnimatedEntity.h
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.h

Contiene la declaración de la clase que representa una entidad gráfica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_AnimatedEntity_H
#define __Graphics_AnimatedEntity_H

#include "Graphics\Entity.h"


// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class AnimationState;
};


namespace Graphics 
{

/*	namespace AnimNames {  //Lo dejo por ahora pero esto es para borrarlo, esta info estaría dirigida por datos en Arquetipes.txt
//		
		
		const std::string IDLE				= "idle"; // ESPARTANO
		const std::string RUN				= "run";
		const std::string DEATH				= "die";		
		const std::string JUMP				= "jump";
		const std::string DAMAGE			= "damage";
		const std::string ACTIVATE_ALTAR	= "activate";	
		const std::string COVER_WITH_WEAPON = "cover";
		const std::string COVER_WITH_SHIELD = "shieldcover";	
		const std::string ATTACK1			= "attack1";
		const std::string ATTACK2			= "attack2";
		const std::string ATTACK3			= "attack3";
		const std::string COMBO1			= "combo1";
		const std::string COMBO2			= "combo2";
		const std::string COMBO3			= "combo3";*/
		
/*/
		const std::string IDLE				= "IdleKatana";	// MARINE
		const std::string RUN				= "RunKatana";
		const std::string DEATH				= "Death";		
		const std::string JUMP				= "Crouch";
		const std::string DAMAGE			= "Damage";
		const std::string ACTIVATE_ALTAR	= "activate";	
		const std::string COVER_WITH_WEAPON = "CrouchKatana";
		const std::string COVER_WITH_SHIELD = "CrouchKatana";	
		const std::string ATTACK1			= "FireKatana";
		const std::string ATTACK2			= "GetObject";
		const std::string COMBO1			= "combo1";
		const std::string COMBO2			= "combo2";
		const std::string COMBO3			= "combo3";

	};/**/


	/**
	Esta clase debe ser implementada por las clases que quieren
	registrarse en una entidad animada para enterarse de cuando
	terminan las animaciones de ésta.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CAnimatedEntityListener 
	{
	public:

		/**
		M?todo que ser? invocado siempre que se termine una animaci?n.
		Las animaciones en c?clicas no invocar?n nunca este m?todo.

		@param animation Nombre de la animaci?n terminada.
		*/
		virtual void animationFinished(const std::string &animation) {}

		virtual void animationMomentReached(const std::string &animation) {}

	}; // CAnimatedEntityListener

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

	@author David Llansó
	@date Julio, 2010
	*/
	class CAnimatedEntity : public CEntity
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CAnimatedEntity(const std::string &name, const std::string &mesh):
					CEntity(name,mesh), _currentAnimation(0), _currentAnimationName(""), _rewinding(false),_momentEnabled(true),_paused(false),_ticksPaused(0),_maxTicks(0) {}


		/**
		Activa una animación a partir de su nombre.

		@param anim Nombre de la animación a activar.
		@param loop true si la animación debe reproducirse cíclicamente.
		@return true si la animación solicitada fue correctamente activada.
		*/
		bool setAnimation(const std::string &anim, float moment, bool loop, std::list<float>* eventChain);
		
		/**
		Desactiva una animación a partir de su nombre.

		@param anim Nombre de la animación a activar.
		@return true si la animación solicitada fue correctamente desactivada.
		*/
		bool stopAnimation(const std::string &anim);
		
		/**
		Desactiva todas las animaciones de una entidad.
		*/
		void stopAllAnimations();

		/**
		Funcin que registra al oyente de la entidad grfica. Por 
		simplicidad solo habr un oyente por entidad.
		*/
		void setObserver(CAnimatedEntityListener *observer) {_observer = observer;}

		/**
		Funci?n que quita al oyente de la entidad gr?fica. Por 
		simplicidad solo habr? un oyente por entidad.
		*/
		void removeObserver() { _observer = 0;}
		
		
		void rewind(const std::string &anim,const bool moment)	{_rewinding=true;} 


		bool pauseAnimation(const std::string &anim,float moment);

		bool pauseAnimationXTicks(const std::string &anim,float moment, unsigned int ticks);	

	private:

		/**
		Objeto oyente que es informado de cambios en la entidad como 
		la terminaci?n de las animaciones. Por simplicidad solo habr?
		un oyente por entidad.
		*/
		CAnimatedEntityListener *_observer;

		/**
		Animación que tiene la entidad activada.
		*/
		Ogre::AnimationState *_currentAnimation;

		/**
		Nombre de la animación actual
		*/
		std::string _currentAnimationName;
		/**
		Para revovinar una animación
		*/
		bool _rewinding;
		/**
		Para que solo se envie un mensaje al llegar un momento de la animación. No tantos como ticks
		*/
		bool _momentEnabled;
		

		bool load();

		/**
		Actualiza el estado de la entidad cada ciclo.
		
		@param secs Número de milisegundos transcurridos desde la última 
		llamada.
		*/
		void tick(float secs);

		bool _paused;
		
		unsigned int _ticksPaused;

		unsigned int _maxTicks;


		std::list<float>* _activeEventChain;
	}; // class CAnimatedEntity

} // namespace Graphics

#endif // __Graphics_AnimatedEntity_H
