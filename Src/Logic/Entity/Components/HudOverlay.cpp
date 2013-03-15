/**
@file HudOverlay.cpp

Contiene la implementación del componente que controla el HUD mediante overlays.
 
@see Logic::CHudOverlay
@see Logic::IComponent

@author Pablo Terrado
@date Febrero, 2013
*/

#include "HudOverlay.h"

#include "Graphics/Overlay.h"

//Inclusion de los mensajes
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"
#include "Logic/Entity/Messages/MessageBool.h"
#include "Logic/Entity/Messages/MessageInt.h"

#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CHudOverlay);
	
	//---------------------------------------------------------
		
	CHudOverlay::~CHudOverlay() {	 	
		delete _overlay;
	} // destructor

	//---------------------------------------------------------

	bool CHudOverlay::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
				
		//--Recogida de datos del mapa
		if(entityInfo->hasAttribute("lifeMax"))		
			_health= entityInfo->getFloatAttribute("lifeMax");	
		if(entityInfo->hasAttribute("lifeBase"))		
			_healthBase= entityInfo->getIntAttribute("lifeBase");
		if(entityInfo->hasAttribute("base"))		
			_base = entityInfo->getIntAttribute("base");
		
		//--Recogida de datos del mapa
		
		setVisibleHud(false);	
		
		_overlay = new Graphics::COverlay("testOverlay");
			_overlay->createChildPanel("panelDummy", -1, -1, 1, 1);
	
	//	// FRS panelDummy no hace nada, pero parece ser necesario
	//	Graphics::COverlay *panelDummy = _server->createOverlay("panelDummy",  "Panel" );
	//		

	//	// TextArea se lo metes a este textBoxArea Dummy (contenedor de textArea)
	//	_textBoxArea[DUMMY] = _server->createOverlay("textAreaPanelDummy", "TextArea");
	//		_textBoxArea[DUMMY]->setMetricsMode("pixel");
	//		_textBoxArea[DUMMY]->setPosition(0,0);
	//		_textBoxArea[DUMMY]->setDimensions(1,1);

	//	std::stringstream sDummy;//create a stringstream
	//	sDummy << "d";
	//	_textBoxArea[DUMMY]->setText(sDummy.str());
	//	_textBoxArea[DUMMY]->setTextSize(2.0);
	//	_textBoxArea[DUMMY]->setFont("fuenteSimple");
	//	//_textBoxArea[DUMMY]->setColour(Ogre::ColourValue::White);
	//			
	//	panelDummy->addChild(_textBoxArea[DUMMY]);
	//	_overlayPlay->add( panelDummy );
 //        // Add the panel to the overlay de juego

	//	///// panel health(para la vida del Jugador)
	//	Graphics::COverlay* panelHealth = _server->createOverlay("panelHealth", "Panel");
	//	panelHealth->setMetricsMode("pixel");
	//	panelHealth->setPosition( 20, 20);
	//	panelHealth->setDimensions( 40, 40 );
	//	panelHealth->setMaterial("hudHealth");

	//	_textBoxArea[HEALTH] =_server->createOverlay("textAreaPanelHealth", "TextArea");
	//	_textBoxArea[HEALTH]->setMetricsMode("pixel");
	//	_textBoxArea[HEALTH]->setPosition(45, 10);
	//	_textBoxArea[HEALTH]->setDimensions(20, 20);

	//	std::stringstream sHealth;//create a stringstream
	//	sHealth << _health;//add number to the stream
	//	_textBoxArea[HEALTH]->setText(sHealth.str());
	//	_textBoxArea[HEALTH]->setTextSize(24.0);
	//	_textBoxArea[HEALTH]->setFont("fuenteSimple");
	//	//_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);
	//			
	//	panelHealth->addChild(_textBoxArea[HEALTH]);

	//	_overlayPlay->add2D( panelHealth );
 //        // Add the panel to the overlay
	//	
	/////// panel healthBase(para la base del Jugador)
	//	Graphics::COverlay* panelHealthBase = _server->createOverlay("panelHealthBase", "Panel");
	//	panelHealthBase->setMetricsMode("pixel");
	//	panelHealthBase->setPosition( 20, 80);
	//	panelHealthBase->setDimensions( 40, 40 );
	//	panelHealthBase->setMaterial("hudHealthBase");

	//	_textBoxArea[HEALTHBASE] =_server->createOverlay("textAreaPanelHealthBase", "TextArea");
	//	_textBoxArea[HEALTHBASE]->setMetricsMode("pixel");
	//	_textBoxArea[HEALTHBASE]->setPosition(45, 10);
	//	_textBoxArea[HEALTHBASE]->setDimensions(40, 20);

	//	std::stringstream sHealthBase;//create a stringstream
	//	sHealthBase << _healthBase;//add number to the stream
	//	_textBoxArea[HEALTHBASE]->setText(sHealthBase.str());
	//	_textBoxArea[HEALTHBASE]->setTextSize(24.0);
	//	_textBoxArea[HEALTHBASE]->setFont("fuenteSimple");
	//	//_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);
	//			
	//	panelHealthBase->addChild(_textBoxArea[HEALTHBASE]);

	//	_overlayPlay->add2D( panelHealthBase );
 //        // Add the panel to the overlay

	/////// panel panelPlayers(para los Players enemigos que hay en nuestra base)
	//	Graphics::COverlay* panelPlayers = _server->createOverlay("panelPlayers", "Panel");
	//	panelPlayers->setMetricsMode("pixel");
	//	panelPlayers->setPosition( 20, 125);
	//	panelPlayers->setDimensions( 40, 40 );
	//	panelPlayers->setMaterial("hudPlayers");

	//	_textBoxArea[PLAYERS] =_server->createOverlay("textAreaPanelPlayers", "TextArea");
	//	_textBoxArea[PLAYERS]->setMetricsMode("pixel");
	//	_textBoxArea[PLAYERS]->setPosition(45, 10);
	//	_textBoxArea[PLAYERS]->setDimensions(40, 20);

	//	std::stringstream sPlayers;//create a stringstream
	//	sPlayers << _healthBase;//add number to the stream
	//	_textBoxArea[PLAYERS]->setText(sPlayers.str());
	//	_textBoxArea[PLAYERS]->setTextSize(24.0);
	//	_textBoxArea[PLAYERS]->setFont("fuenteSimple");
	//	//_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);
	//			
	//	panelPlayers->addChild(_textBoxArea[PLAYERS]);

	//	_overlayPlay->add2D( panelPlayers );
         // Add the panel to the overlay

		return true;

	} // spawn


	//---------------------------------------------

	bool CHudOverlay::activate()
	{
		if(_overlay)
			_overlay->setVisible(true);
		return true;
	} //activate

	// FRS DANGER CAUTION ATTENTION: deactivate sólo desactiva temporalmente el componente 
	//(en cuanto a tick y en cuanto a recepción de mensajes); nunca debería liberar memoria 
	//(eso sólo en el dtor)
	void CHudOverlay::deactivate()
	{
		if(_overlay)
			_overlay->setVisible(false);
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
//					if(message->getAction() == Message::DISPLAY_HUD)
//						hudVisor();
					/*if(!message._string.compare("addPlayerToBase"))
						hudPlayers(1);
					if(!message._string.compare("minusPlayerToBase"))
						hudPlayers(-1);
						*/
					break;
			}

	} // process


	/*******************
		HUD CONTROL
	*******************/

	////float value: may be positive (increase health) or negative(decrease health)
	//void CHudOverlay::hudLife(float value){
	//	_health+= value;
	//	std::stringstream sHealth;
	//	sHealth << _health;
	//	_textBoxArea[HEALTH]->setText(sHealth.str());
	//}

	////short int valor: may be 1 or -1. Depends if a player has dead or another player has enter in the base player.
	//void CHudOverlay::hudPlayers(short int valor){
	//	_playersInBase+= valor;
	//	std::stringstream sPlayers;
	//	sPlayers << _playersInBase;
	//	_textBoxArea[PLAYERS]->setText(sPlayers.str());
	//}


	//void CHudOverlay::hudSpawn(int spawmTime){
	//	
	//	if(_overlayPlay->isVisible()){
	//		_overlayPlay->setVisible(false);
	//	}

	//	std::stringstream sSpawn;
	//	sSpawn << "HAS MUERTO, LOSER \n Tiempo de respawn: " << spawmTime;
	//	_textAreaDie->setText(sSpawn.str());
	//	
	//	if(!_overlayDie->isVisible())
	//		_overlayDie->setVisible(true);
	//	if(spawmTime <= 0.1){

	//		_overlayDie->setVisible(false);
	//		//reset para volver a mostrar solo el arma inicial al hacer show
	//		/*for(int i=1; i<_numWeapons;++i){
	//			_weaponsBox[i][NO_WEAPON]->show();
	//		}
	//		*/
	//		_overlayPlay->setVisible(true);
	//	}

	//}


	//void CHudOverlay::hudVisor(){
	//	if(_overlayPlay->isVisible())
	//		_overlayPlay->setVisible(false);
	//	else
	//		_overlayPlay->setVisible(true);
	//}




} // namespace Logic

