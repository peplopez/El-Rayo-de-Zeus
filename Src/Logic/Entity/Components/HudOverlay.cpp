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

#include "Graphics/Server.h"
#include "Graphics/Camera.h"
#include "Graphics/Overlay.h"
#include "Graphics/StaticEntity.h"

/*
#include <OgreSceneManager.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreColourValue.h>
#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>
*/

//Inclusion de los mensajes
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageBool.h"
#include "Logic/Entity/Messages/MessageInt.h"


namespace Logic 
{
	IMP_FACTORY(CHudOverlay);
	
	CHudOverlay::CHudOverlay() : IComponent(GetAltTypeIdOf(CHudOverlay)), 
			_server(0), _overlayPlay(0), _overlayDie(0), _health(1000.0), 
			_playersInBase(0),_visibleHud(false) { 
		_server = Graphics::CServer::getSingletonPtr(); // FRS El server siempre debería estar disponible desde el principio
	}
	
	// HACK FRS revisar...
	CHudOverlay::~CHudOverlay() {	 	
		//_server->removeOverlay("textAreaPanelPlayers");
		//_server->removeOverlay("panelPlayers");
		//_server->removeOverlay("textAreaPanelHealthBase");
		//_server->removeOverlay("panelHealthBase");
		//_server->removeOverlay("textAreaPanelHealth");
	/*	_server->removeOverlay("panelDummy");
		_server->removeOverlay("_overlayPlay");*/
	} // destructor

	//---------------------------------------------------------

	bool CHudOverlay::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_overlayPlay = _server->getOverlay("_overlayPlay");
			if( _overlayPlay  )
				return true;
		
		//--Recogida de datos del mapa
		if(entityInfo->hasAttribute("lifeMax"))		
			_health= entityInfo->getFloatAttribute("lifeMax");	
		if(entityInfo->hasAttribute("lifeBase"))		
			_healthBase= entityInfo->getIntAttribute("lifeBase");
		if(entityInfo->hasAttribute("base"))		
			_base = entityInfo->getIntAttribute("base");
		
		//--Recogida de datos del mapa
		
		setVisibleHud(false);		
		_playersInBase = 0;
		
		_overlayPlay = _server->createOverlay( "_overlayPlay" );
		
		int height = _server->getScreenHeight();
		int width = _server->getScreenWidth();

		float relativeWidth = (float)width/26;
		float relativeHeight = (float)height/31;

		///// panel DUMMY		
		Graphics::COverlay *panelDummy = _server->createOverlay("panelDummy",  "Panel" );
			panelDummy->setMetricsMode("pixel");
			panelDummy->setPosition(-1,-1);
			panelDummy->setDimensions(1,1);

		_textBoxArea[DUMMY] = _server->createOverlay("textAreaPanelDummy", "TextArea");
			_textBoxArea[DUMMY]->setMetricsMode("pixel");
			_textBoxArea[DUMMY]->setPosition(0,0);
			_textBoxArea[DUMMY]->setDimensions(1,1);

		std::stringstream sDummy;//create a stringstream
		sDummy << "d";
		_textBoxArea[DUMMY]->setText(sDummy.str());
		_textBoxArea[DUMMY]->setTextSize(2.0);
		_textBoxArea[DUMMY]->setFont("fuenteSimple");
		//_textBoxArea[DUMMY]->setColour(Ogre::ColourValue::White);
				
		panelDummy->addChild(_textBoxArea[DUMMY]);
		_overlayPlay->add2D( panelDummy );
         // Add the panel to the overlay

		///// panel health(para la vida del Jugador)
		Graphics::COverlay* panelHealth = _server->createOverlay("panelHealth", "Panel");
		panelHealth->setMetricsMode("pixel");
		panelHealth->setPosition( 20, 20);
		panelHealth->setDimensions( 40, 40 );
		panelHealth->setMaterial("hudHealth");

		_textBoxArea[HEALTH] =_server->createOverlay("textAreaPanelHealth", "TextArea");
		_textBoxArea[HEALTH]->setMetricsMode("pixel");
		_textBoxArea[HEALTH]->setPosition(45, 10);
		_textBoxArea[HEALTH]->setDimensions(20, 20);

		std::stringstream sHealth;//create a stringstream
		sHealth << _health;//add number to the stream
		_textBoxArea[HEALTH]->setText(sHealth.str());
		_textBoxArea[HEALTH]->setTextSize(24.0);
		_textBoxArea[HEALTH]->setFont("fuenteSimple");
		//_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);
				
		panelHealth->addChild(_textBoxArea[HEALTH]);

		_overlayPlay->add2D( panelHealth );
         // Add the panel to the overlay
		
	///// panel healthBase(para la base del Jugador)
		Graphics::COverlay* panelHealthBase = _server->createOverlay("panelHealthBase", "Panel");
		panelHealthBase->setMetricsMode("pixel");
		panelHealthBase->setPosition( 20, 80);
		panelHealthBase->setDimensions( 40, 40 );
		panelHealthBase->setMaterial("hudHealthBase");

		_textBoxArea[HEALTHBASE] =_server->createOverlay("textAreaPanelHealthBase", "TextArea");
		_textBoxArea[HEALTHBASE]->setMetricsMode("pixel");
		_textBoxArea[HEALTHBASE]->setPosition(45, 10);
		_textBoxArea[HEALTHBASE]->setDimensions(40, 20);

		std::stringstream sHealthBase;//create a stringstream
		sHealthBase << _healthBase;//add number to the stream
		_textBoxArea[HEALTHBASE]->setText(sHealthBase.str());
		_textBoxArea[HEALTHBASE]->setTextSize(24.0);
		_textBoxArea[HEALTHBASE]->setFont("fuenteSimple");
		//_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);
				
		panelHealthBase->addChild(_textBoxArea[HEALTHBASE]);

		_overlayPlay->add2D( panelHealthBase );
         // Add the panel to the overlay

	///// panel panelPlayers(para los Players enemigos que hay en nuestra base)
		Graphics::COverlay* panelPlayers = _server->createOverlay("panelPlayers", "Panel");
		panelPlayers->setMetricsMode("pixel");
		panelPlayers->setPosition( 20, 125);
		panelPlayers->setDimensions( 40, 40 );
		panelPlayers->setMaterial("hudPlayers");

		_textBoxArea[PLAYERS] =_server->createOverlay("textAreaPanelPlayers", "TextArea");
		_textBoxArea[PLAYERS]->setMetricsMode("pixel");
		_textBoxArea[PLAYERS]->setPosition(45, 10);
		_textBoxArea[PLAYERS]->setDimensions(40, 20);

		std::stringstream sPlayers;//create a stringstream
		sPlayers << _healthBase;//add number to the stream
		_textBoxArea[PLAYERS]->setText(sPlayers.str());
		_textBoxArea[PLAYERS]->setTextSize(24.0);
		_textBoxArea[PLAYERS]->setFont("fuenteSimple");
		//_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);
				
		panelPlayers->addChild(_textBoxArea[PLAYERS]);

		_overlayPlay->add2D( panelPlayers );
         // Add the panel to the overlay

		return true;

	} // spawn

	bool CHudOverlay::activate()
	{

		//Se crea un manager de overlays. _overlayPlay
		/*
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		_overlayPlay = overlayManager.create("_overlayPlay");


		float height = (float)overlayManager.getViewportHeight();
		float width = (float)overlayManager.getViewportWidth();
		

		float relativeWidth = width/26;
		float relativeHeight = height/31;


		///// panel DUMMY
			Ogre::OverlayContainer* panelDummy = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelDummy" ) );
			panelDummy->setMetricsMode(Ogre::GMM_PIXELS);
			panelDummy->setPosition(-1,-1);
			panelDummy->setDimensions(1,1);


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
		
		Ogre::OverlayContainer* panelPlayers = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelPlayers" ) );
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

		_overlayDie->show();
		 */
		return true;

	} //activate

	// FRS DANGER CAUTION ATTENTION: deactivate sólo desactiva temporalmente el componente 
	//(en cuanto a tick y en cuanto a recepción de mensajes); nunca debería liberar memoria 
	//(eso sólo en el dtor)
	void CHudOverlay::deactivate()
	{
		//overlayManager.destroyAllOverlayElements();
		//overlayManager.destroyAll();
	}//deactivate
	
	//---------------------------------------------------------

	bool CHudOverlay::accept(const CMessage *message)
	{
		return message->getType() == Message::HUD || message->getType() == Message::CONTROL;
	} // accept
	
	//---------------------------------------------------------

	void CHudOverlay::process(CMessage *message)
	{

			switch(message->getType())
			{
				/*case Message::CONTACT:
						hudLife(message._float);*/
				case Message::HUD:
					//CMessageFloat *maux = static_cast<CMessageFloat*>(message);
					if(message->getAction() == Message::DISPLAY_HUD)
						hudVisor();
					/*if(!message._string.compare("addPlayerToBase"))
						hudPlayers(1);
					if(!message._string.compare("minusPlayerToBase"))
						hudPlayers(-1);
						*/
					break;
			}

	} // process


	//float value: may be positive (increase health) or negative(decrease health)
	void CHudOverlay::hudLife(float value){
		_health+= value;
		std::stringstream sHealth;
		sHealth << _health;
		_textBoxArea[HEALTH]->setText(sHealth.str());
	}

	//short int valor: may be 1 or -1. Depends if a player has dead or another player has enter in the base player.
	void CHudOverlay::hudPlayers(short int valor){
		_playersInBase+= valor;
		std::stringstream sPlayers;
		sPlayers << _playersInBase;
		_textBoxArea[PLAYERS]->setText(sPlayers.str());
	}


	void CHudOverlay::hudSpawn(int spawmTime){
		
		if(_overlayPlay->isVisible()){
			_overlayPlay->setVisible(false);
		}

		std::stringstream sSpawn;
		sSpawn << "HAS MUERTO, LOSER \n Tiempo de respawn: " << spawmTime;
		_textAreaDie->setText(sSpawn.str());
		
		if(!_overlayDie->isVisible())
			_overlayDie->setVisible(true);
		if(spawmTime <= 0.1){

			_overlayDie->setVisible(false);
			//reset para volver a mostrar solo el arma inicial al hacer show
			/*for(int i=1; i<_numWeapons;++i){
				_weaponsBox[i][NO_WEAPON]->show();
			}
			*/
			_overlayPlay->setVisible(true);
		}

	}


	void CHudOverlay::hudVisor(){
		if(_overlayPlay->isVisible())
			_overlayPlay->setVisible(false);
		else
			_overlayPlay->setVisible(true);
	}




} // namespace Logic

