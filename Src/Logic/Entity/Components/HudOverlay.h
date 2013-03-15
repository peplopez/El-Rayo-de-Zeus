/***

@see Logic::CHudOverlay
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/
#ifndef __Logic_HudOverlay_H
#define __Logic_HudOverlay_H

//#include "Graphics/Server.h"
#include "Logic/Entity/Component.h"


namespace Graphics 
{
	class CServer;
	class COverlay;
}


//declaración de la clase
namespace Logic 
{
/**
	Este componente controla el hud, mediante overlays
    @ingroup logicGroup

	@author Pablo Terrado
	@date Febrero, 2013
*/
	class CHudOverlay : public IComponent
	{
		DEC_FACTORY(CHudOverlay);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CHudOverlay() : IComponent(GetAltTypeIdOf(CHudOverlay)), 
			 _overlay(0), _health(1000.0), 
			_playersInBase(0),_visibleHud(false) { }
		
		/** Destructor */
		CHudOverlay::~CHudOverlay();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool accept(const CMessage *message);

		void process(CMessage *message);	

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		*/
		void deactivate();

		bool getVisibleHud(){return _visibleHud; }

		void setVisibleHud(bool valor){_visibleHud = valor;}

		

	protected:

		static enum eOverlayTextArea {HEALTH, HEALTHBASE, PLAYERS, DUMMY };
		static enum ColorValue { WHITE,BLACK,RED,GREEN,BLUE };

		//UNDONE FRS temporalmente
		//void hudLife(float health);
		//void hudVisor();
		//void hudSpawn(int spawn);
		//void hudPlayers(short int valor);

		float _health;
		int _healthBase; //la base tiene 3 vidas (3 rayos)
		int _base; //numero de base
		short int _playersInBase; //num Players contrarios en la base del Player
		bool _visibleHud;

	

		// TODO FRS de momento puntero, pero podría ser estático...
		//Overlay de juego y overlay al morir el Player.
		Graphics::COverlay* _overlay;
		

		//Graphics::COverlay *_textBoxArea[4];
		//Graphics::COverlay *_overlayDie;

		//Graphics::COverlay *_textAreaDie;
		
		 
	}; // class CHudOverlay

	REG_FACTORY(CHudOverlay);

} // namespace Logic

#endif // __Logic_HudOverlay_H
