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
	
	// HACK FRS: Inicialización un poco chunga; lo suyo sería guardarse los nombres en variables
	// y cargar las propiedades del map
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
		
		// FRS panelDummy no hace nada, pero parece ser necesario para el texto
		_overlay = new Graphics::COverlay("overlay");
			_overlay->createChildPanel( "panel", -1, -1, 1, 1);
			_overlay->createChildTextArea( "panel", "text", 
				0, 0, 1, 1,"fuenteSimple", 2.0, "dummyText");
			// TextArea se lo metes a este textBoxArea Dummy (contenedor de textArea)

		// HEALTH (para la vida del Jugador)
		_overlay->createChildPanel("panelHealth", 20, 20, 40, 40, "hudHealth");

		std::stringstream sHealth;//create a stringstream
			sHealth << _health;//add number to the stream

		_overlay->createChildTextArea("panelHealth", "textAreaPanelHealth",
			45, 10, 20, 20, "fuenteSimple", 24.0, sHealth.str() );

		//  HEALTH BASE (para la base del Jugador)
		_overlay->createChildPanel("panelHealthBase", 20, 80, 40, 40, "hudHealthBase");

		std::stringstream sHealthBase;//create a stringstream
			sHealthBase << _healthBase;//add number to the stream

		_overlay->createChildTextArea("panelHealthBase", "textAreaPanelHealthBase",
			45, 10, 40, 20, "fuenteSimple", 24.0, sHealthBase.str() );

		// PLAYERS(para los Players enemigos que hay en nuestra base)
		_overlay->createChildPanel("panelPlayers", 20, 125, 40, 40, "hudPlayers");

		std::stringstream sPlayers;//create a stringstream
			sPlayers << _healthBase;//add number to the stream

		_overlay->createChildTextArea("panelPlayers", "textAreaPanelPlayers",
			45, 10, 40, 20, "fuenteSimple", 24.0, sPlayers.str() );

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
					if(message->getAction() == Message::DISPLAY_HUD)
						displayHud();
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

	//float value: may be positive (increase health) or negative(decrease health)
	void CHudOverlay::hudLife(float value){
		_health+= value;
			std::stringstream sHealth;
				sHealth << _health;
		_overlay->setChildText("textAreaPanelHealthBase", sHealth.str() );
	}

	//short int valor: may be 1 or -1. Depends if a player has dead or another player has enter in the base player.
	void CHudOverlay::hudPlayers(short int valor){
		_playersInBase+= valor;
			std::stringstream sPlayers;
				sPlayers << _playersInBase;		
		_overlay->setChildText("textAreaPanelPlayers", sPlayers.str() );		
	}


	void CHudOverlay::hudSpawn(int spawmTime){
		
		_overlay->setVisible(false);

		std::stringstream sSpawn;
			sSpawn << "HAS MUERTO, LOSER \n Tiempo de respawn: " << spawmTime;		
		_overlay->setChildText("textAreaDie", sSpawn.str() );		
		
		_overlayDie->setVisible(true);

		if(spawmTime <= 0.1){

			_overlayDie->setVisible(false);
			//reset para volver a mostrar solo el arma inicial al hacer show
			/*for(int i=1; i<_numWeapons;++i){
				_weaponsBox[i][NO_WEAPON]->show();
			}
			*/
			_overlay->setVisible(true);
		}

	}


	void CHudOverlay::displayHud(){
		_overlay->isVisible() ?
			_overlay->setVisible(false) :		
			_overlay->setVisible(true);
	}




} // namespace Logic

