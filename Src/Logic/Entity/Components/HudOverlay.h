/***

@see Logic::CHudOverlay
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/
#ifndef __Logic_HudOverlay_H
#define __Logic_HudOverlay_H

#include "Logic/Entity/Component.h"

#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>

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
		//CHudOverlay() : IComponent(), _health(0), _shield(0), _ammo(1), _actualWeapon(0), _numWeapons(0) {}
		CHudOverlay() : IComponent(), _health(1000.0), _shield(200), _playersInBase(0),_visibleHud(false) {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		bool accept(const CMessage *message);
		//virtual bool accept(const TMessage &message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		void process(CMessage *message);	

		//virtual void process(const TMessage &message);

		bool getVisibleHud(){return _visibleHud; }

		void setVisibleHud(bool valor){_visibleHud = valor;}

		

	protected:
		enum eOverlayTextArea {HEALTH, HEALTHBASE, PLAYERS, DUMMY };
		

		

		void hudLife(float health);
		void hudShield(int shield);
		void hudVisor();
		void hudSpawn(int spawn);
		void hudPlayers(short int valor);


		float _health;
		int _healthBase;
		int _shield;
		int _base;
		short int _playersInBase;
		bool _visibleHud;

		Ogre::Overlay *_overlayPlay;
		Ogre::Overlay *_overlayDie;

		Ogre::TextAreaOverlayElement *_textAreaDie;

		Ogre::TextAreaOverlayElement *_textBoxArea[3];

		// En vez de 4 deberia de ir el numero de armas pero no tengo cojones U.U
		Ogre::OverlayContainer *_weaponsBox[4][3];

	}; // class CHudOverlay

	REG_FACTORY(CHudOverlay);

} // namespace Logic

#endif // __Logic_HudOverlay_H
