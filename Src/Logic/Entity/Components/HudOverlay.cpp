/**
@file HudOverlay.cpp

Contiene la implementación del componente que controla el HUD mediante overlays.
 
@see Logic::CHudOverlay
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/

#include "HudOverlay.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"

#include <OgreSceneManager.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreColourValue.h>
#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>


/*#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageHudLife.h"
#include "Logic/Messages/MessageHudShield.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageHudSpawn.h"
*/

namespace Logic 
{
	IMP_FACTORY(CHudOverlay);
	
	//---------------------------------------------------------

	bool CHudOverlay::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("life"))
		{
			_health= entityInfo->getFloatAttribute("life");
		}


		if(entityInfo->hasAttribute("lifeBase"))
		{
			_healthBase= entityInfo->getIntAttribute("lifeBase");
		}

		if(entityInfo->hasAttribute("base"))
		{
			_base = entityInfo->getIntAttribute("base");
		}

		
		setVisibleHud(false);

		
		_playersInBase = 0;

		//_numWeapons = entityInfo->getIntAttribute("numWeapons");
		_numWeapons = 3;

		int _actualWeapon;

		

		//Se crea un manager de overlays. _overlayPlay
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		_overlayPlay = overlayManager.create("_overlayPlay");


		/*
		float height = overlayManager.getViewportHeight();
		float width = overlayManager.getViewportWidth();
		*/

		float height = 800;
		float width = 600;
		

		float relativeWidth = width/26;
		float relativeHeight = height/31;


		///// panel DUMMY
		Ogre::OverlayContainer* panelDummy = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelDummy" ) );
		panelDummy->setMetricsMode(Ogre::GMM_PIXELS);
		panelDummy->setPosition(-1,-1);
		panelDummy->setDimensions(1,1);
        //panelDummy->setMaterialName("hudHealth");


		_textBoxArea[DUMMY] = static_cast<Ogre::TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", "textAreaPanelDummy"));
		_textBoxArea[DUMMY]->setMetricsMode(Ogre::GMM_PIXELS);
		_textBoxArea[DUMMY]->setPosition(0,0);
		_textBoxArea[DUMMY]->setDimensions(1,1);

		std::stringstream sDummy;//create a stringstream
		sDummy << "d";
		_textBoxArea[DUMMY]->setCaption(sDummy.str());
		_textBoxArea[DUMMY]->setCharHeight(2);
		_textBoxArea[DUMMY]->setFontName("fuenteSimple");
		_textBoxArea[DUMMY]->setColour(Ogre::ColourValue::White);
				
		panelDummy->addChild(_textBoxArea[DUMMY]);

		_overlayPlay->add2D( panelDummy );
         // Add the panel to the overlay



		///// panel health(para la vida del Jugador)
		Ogre::OverlayContainer* panelHealth = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelHealth" ) );
		panelHealth->setMetricsMode(Ogre::GMM_PIXELS);
		panelHealth->setPosition( 20, 20);
		panelHealth->setDimensions( 40, 40 );
        panelHealth->setMaterialName("hudHealth");


		_textBoxArea[HEALTH] = static_cast<Ogre::TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", "textAreaPanelHealth"));
		_textBoxArea[HEALTH]->setMetricsMode(Ogre::GMM_PIXELS);
		_textBoxArea[HEALTH]->setPosition(45, 10);
		_textBoxArea[HEALTH]->setDimensions(20, 20);

		std::stringstream sHealth;//create a stringstream
		sHealth << _health;//add number to the stream
		_textBoxArea[HEALTH]->setCaption(sHealth.str());
		_textBoxArea[HEALTH]->setCharHeight(24);
		_textBoxArea[HEALTH]->setFontName("fuenteSimple");
		_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);
				
		panelHealth->addChild(_textBoxArea[HEALTH]);

		_overlayPlay->add2D( panelHealth );
         // Add the panel to the overlay




		///// panel healthBase (para la Base)

		Ogre::OverlayContainer* panelHealthBase = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelHealthBase" ) );
		panelHealthBase->setMetricsMode(Ogre::GMM_PIXELS);
		panelHealthBase->setPosition( 20, 80);
		panelHealthBase->setDimensions( 40, 40 );
        panelHealthBase->setMaterialName("hudHealthBase");

		_textBoxArea[HEALTHBASE] = static_cast<Ogre::TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", "textAreaPanelHealthBase"));
				
		
		_textBoxArea[HEALTHBASE]->setMetricsMode(Ogre::GMM_PIXELS);
		_textBoxArea[HEALTHBASE]->setPosition(45, 10);
		_textBoxArea[HEALTHBASE]->setDimensions(40, 20);

		std::stringstream sHealthBase;//create a stringstream
		sHealthBase << _healthBase;//add number to the stream
		_textBoxArea[HEALTHBASE]->setCaption(sHealthBase.str());
		_textBoxArea[HEALTHBASE]->setCharHeight(24);
		_textBoxArea[HEALTHBASE]->setFontName("fuenteSimple");
		_textBoxArea[HEALTHBASE]->setColour(Ogre::ColourValue::White);
				
		panelHealthBase->addChild(_textBoxArea[HEALTHBASE]);

		_overlayPlay->add2D( panelHealthBase );
         // Add the panel to the overlay


		///// panel Enemies (numero de enemigos de la base)
		
		Ogre::OverlayContainer* panelPlayers = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelEnemies" ) );
		panelPlayers->setMetricsMode(Ogre::GMM_PIXELS);
		panelPlayers->setPosition( 20, 125);
		panelPlayers->setDimensions( 40, 40 );
        panelPlayers->setMaterialName("hudPlayers");

		_textBoxArea[PLAYERS] = static_cast<Ogre::TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", "textAreaPanelPlayers"));
				
		
		_textBoxArea[PLAYERS]->setMetricsMode(Ogre::GMM_PIXELS);
		_textBoxArea[PLAYERS]->setPosition(45, 10);
		_textBoxArea[PLAYERS]->setDimensions(40, 20);

		std::stringstream sPlayers;//create a stringstream
		sPlayers << _playersInBase;//add number to the stream
		_textBoxArea[PLAYERS]->setCaption(sPlayers.str());
		_textBoxArea[PLAYERS]->setCharHeight(24);
		_textBoxArea[PLAYERS]->setFontName("fuenteSimple");
		_textBoxArea[PLAYERS]->setColour(Ogre::ColourValue::White);
				
		panelPlayers->addChild(_textBoxArea[PLAYERS]);

		_overlayPlay->add2D( panelPlayers );
         // Add the panel to the overlay


		if(getVisibleHud())
		  _overlayPlay->show();
		else
		  _overlayPlay->hide();






		 //////////////////////////////////////AQUI ME CREO EL OVERLAY PARA CUANDO SE MUERA



		 /*
		 _overlayDie = overlayManager.create( "_overlayDie" );



		Ogre::OverlayContainer* panelDie = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelDie" ) );
		panelDie->setMetricsMode(Ogre::GMM_PIXELS);
		panelDie->setPosition( 5*relativeWidth, 12*relativeHeight);
		panelDie->setDimensions( relativeWidth*10, relativeHeight*10 );
        //panelDie->setMaterialName("cuadroArmas");

		_textAreaDie = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "_textAreaDie"));
				
		_textAreaDie->setMetricsMode(Ogre::GMM_PIXELS);
		_textAreaDie->setPosition(0.5, 0.5);
		_textAreaDie->setDimensions(10, 10);

		_textAreaDie->setCaption("MUERE");
		_textAreaDie->setCharHeight(46);
		//_textAreaDie->setFontName("fuenteSimple");
				
		panelDie->addChild(_textAreaDie);

		_overlayDie->add2D( panelDie );

		_overlayDie->show();*/

		 

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CHudOverlay::accept(const TMessage &message)
	{
		/*return message->getMessageType() == Message::HUD_LIFE
			|| message->getMessageType() == Message::HUD_SHIELD
			|| message->getMessageType() == Message::HUD_AMMO
			|| message->getMessageType() == Message::HUD_WEAPON
			|| message->getMessageType() == Message::HUD_SPAWN;
			*/
		return message._type == Message::CONTACTO || message._type == Message::CONTROL ;
		//return true;
	} // accept
	
	//---------------------------------------------------------

	void CHudOverlay::process(const TMessage &message)
	{
		/*switch(message->getMessageType())
		{
		case Message::HUD_LIFE: hudLife(((CMessageHudLife*)message)->getLife());
			break;
		case Message::HUD_SHIELD: hudShield(((CMessageHudShield*)message)->getShield());
			break;
		case Message::HUD_AMMO: hudAmmo( ((CMessageHudAmmo*)message)->getAmmo(), ((CMessageHudAmmo*)message)->getWeapon());
			break;
		case Message::HUD_WEAPON: hudWeapon(((CMessageHudWeapon*)message)->getAmmo(), ((CMessageHudWeapon*)message)->getWeapon());
			break;
		case Message::HUD_SPAWN: hudSpawn(((CMessageHudSpawn*)message)->getTime());
			break;
		}
		*/

			switch(message._type)
			{
				case Message::CONTACTO:
					if(!message._string.compare("updateLife"))
						hudLife(message._float);
				case Message::CONTROL:
					if(!message._string.compare("displayVisor"))
						hudVisor();
					if(!message._string.compare("addPlayerToBase"))
						hudPlayers(1);
					if(!message._string.compare("minusPlayerToBase"))
						hudPlayers(-1);
			}

	} // process

	void CHudOverlay::hudLife(float health){
		//_health = health;
		_health-= health;
		std::stringstream sHealth;
		sHealth << _health;
		_textBoxArea[HEALTH]->setCaption(sHealth.str());
	}

	void CHudOverlay::hudPlayers(short int valor){
		_playersInBase+= valor;
		std::stringstream sPlayers;
		sPlayers << _playersInBase;
		_textBoxArea[PLAYERS]->setCaption(sPlayers.str());
	}


	void CHudOverlay::hudSpawn(int spawmTime){
		
		if(_overlayPlay->isVisible()){
			_overlayPlay->hide();
		}

		std::stringstream sSpawn;
		sSpawn << "HAS MUERTO, LOSER \n Tiempo de respawn: " << spawmTime;
		_textAreaDie->setCaption(sSpawn.str());
		
		if(!_overlayDie->isVisible())
			_overlayDie->show();
		if(spawmTime <= 0.1){

			_overlayDie->hide();
			//reset para volver a mostrar solo el arma inicial al hacer show
			for(int i=1; i<_numWeapons;++i){
				_weaponsBox[i][NO_WEAPON]->show();
			}
			_overlayPlay->show();
		}

	}


	void CHudOverlay::hudVisor(){
		if(_overlayPlay->isVisible())
			_overlayPlay->hide();
		else
			_overlayPlay->show();
	}


} // namespace Logic

