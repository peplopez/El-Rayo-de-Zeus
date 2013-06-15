/***
@file Shop.cpp

Contiene la implementación del componente que controla el Shop mediante CEGUI.
 
@see Logic::CShop
@see Logic::IComponent

@author Pablo Terrado
@date Junio, 2013
*/

#include "Shop.h"

//Inclusion de los mensajes
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"

#include "Logic/GameStatus.h"
#include "Logic/BaseInfo.h"
#include "Logic/PlayerInfo.h"
 
#include "../../../../dependencies/include/cegui/CEGUISystem.h" //PT
#include "../../../../dependencies/include/cegui/CEGUIWindowManager.h" //PT
#include "../../../../dependencies/include/cegui/CEGUIWindow.h" //PT
#include "../../../../dependencies/include/cegui/CEGUI.h" //PT

#include "GUI/Server.h" //PT
#include "GUI/PlayerController.h" //PT
#include "GUI/CameraController.h" //PT
#include "GUI/HudController.h" //PT
#include "GUI/ShopController.h" //PT

#include "ScriptManager\Server.h"

namespace Logic 
{
	IMP_FACTORY(CShop);
	
	//---------------------------------------------------------
		
	CShop::~CShop() {

		item1window->destroy();
		item2window->destroy();
		item3window->destroy();

		medusawindow->destroy();
		cancerberowindow->destroy();
		minotaurowindow->destroy();
	} // destructor

	//---------------------------------------------------------
	
	bool CShop::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		numBase = entity->getOriginBase();

		player = numBase;

		//Get CGameStatus Singleton to recover info to show in the Shop
		_gameStatus = Logic::CGameStatus::getSingletonPtr();

		//init Shop Layout and functions
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("Shop");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initShop");

		_winShop = (CEGUI::FrameWindow*) CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop");
		_winShop->setCloseButtonEnabled(true); //para poder cerrar la ventana con la X
		_winShop->setSizingEnabled(false); //No se permite mover de tamaño ni cambiar el tamaño de la ventana
		_winShop->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&CShop::handleClose, this));

		_winShop->setVisible(false); //para que no se vea al iniciar el juego
		_winShop->setDragMovingEnabled(false); //para que no se pueda mover

		_winShop->setRollupEnabled(false); //para que al clickar dos veces sobre el titlebar no desaparezca

		//_area = _winShop->getInnerRectClipper(); //conseguir el area de la ventana (para el movimiento del raton)

		// ajustar margenes del area de movimiento del raton
		//_area.d_top = _area.d_top - 18; //para que se pueda pulsar sobre el TitleBar
		//_area.d_bottom = _area.d_bottom - 10;

		//ITEMS
			_itemsWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/ControlPestanas/Items");

			item1window = CEGUI::WindowManager::getSingleton().loadWindowLayout( "item1.layout" );
			_itemsWindow->addChildWindow(item1window);
			item1window->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,50.0f) ) );
			item1window->setVisible( true );
			item1window->setInheritsAlpha(false);

			item2window = CEGUI::WindowManager::getSingleton().loadWindowLayout( "item2.layout" );
			_itemsWindow->addChildWindow(item2window);
			item2window->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,100.0f) ) );
			item2window->setInheritsAlpha(false);

			item3window = CEGUI::WindowManager::getSingleton().loadWindowLayout( "item3.layout" );
			_itemsWindow->addChildWindow(item3window);
			item3window->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,150.0f) ) );
			item3window->setInheritsAlpha(false);


		//CRIATURES


			_criaturesWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/ControlPestanas/Criatures");

			//The windows that are being loading are Taharez/ImageButtons
			medusawindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "medusa.layout" );
			_criaturesWindow->addChildWindow(medusawindow);
			medusawindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,10.0f) ) );
			medusawindow->setVisible( true );
			medusawindow->setInheritsAlpha(false);

			cancerberowindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "cancerbero.layout" );
			_criaturesWindow->addChildWindow(cancerberowindow);
			cancerberowindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,70.0f) ) );
			cancerberowindow->setInheritsAlpha(false);

			minotaurowindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "minotauro.layout" );
			_criaturesWindow->addChildWindow(minotaurowindow);
			minotaurowindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,130.0f) ) );
			minotaurowindow->setInheritsAlpha(false);



		//COMBOS
		_comboWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/ControlPestanas/Combos");




		return true;

	} // spawn


	//---------------------------------------------

	bool CShop::activate()
	{
		//PT no hago nada porque no quiero que se active al inicio del juego (solo cuando se pulse la tecla T)
		return true;
	} //activate

	//---------------------------------------------

	void CShop::deactivate()
	{
		//ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideShop");

		//ITEMS
		//CEGUI::WindowManager::getSingleton().destroyWindow(item1window->getName());
		//CEGUI::WindowManager::getSingleton().destroyWindow(item2window->getName());
		//CEGUI::WindowManager::getSingleton().destroyWindow(item3window->getName());

		//CRIATURES
		//CEGUI::WindowManager::getSingleton().destroyWindow(medusawindow->getName());
		//CEGUI::WindowManager::getSingleton().destroyWindow(cancerberowindow->getName());
		//CEGUI::WindowManager::getSingleton().destroyWindow(minotaurowindow->getName());




	}//deactivate
	
	//---------------------------------------------------------

	bool CShop::accept(const CMessage *message)
	{
		return message->getType() == Message::SHOP;
	} // accept
	
	//---------------------------------------------------------

	void CShop::process(CMessage *message)
	{

			switch(message->getType())
			{
				case Message::SHOP:

					if(message->getAction() == Message::DISPLAY_SHOP)
						displayShop();
					if(message->getAction() == Message::ACTIVATE_SHOP)
						activateControl();
					if(message->getAction() == Message::DEACTIVATE_SHOP)
						deactivateControl();
			}

	} // process

	void CShop::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//se recupera la informacion de Logic::CGameStatus cada medio segundo para actualizar el Shop

			////Enemies number / players that are not me in my origin base
			//ScriptManager::CServer::getSingletonPtr()->executeProcedure("showEnemiesInBase",_gameStatus->getEnemiesInBase(numBase));

			//Merite Points
			//ScriptManager::CServer::getSingletonPtr()->executeProcedure("showPM",_gameStatus->getPlayer(player)->getMeritPoints());
		
	}//tick

	void CShop::displayShop()
	{
		//ScriptManager::CServer::getSingletonPtr()->executeProcedure("changeShopVisibility");

		if(_winShop->isVisible())
			deactivateControl();
		else
			activateControl();

	}

	bool CShop::handleClose(const CEGUI::EventArgs&e)
	{
		deactivateControl();
		return true;
	}

	bool CShop::activateControl()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showShop");

		//PT para limitar el uso del raton en la ventana de la tienda del Olimpo
		//CEGUI::MouseCursor::getSingletonPtr()->setConstraintArea(&_area);

		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();
		GUI::CServer::getSingletonPtr()->getCameraController()->deactivate();

		return true;
	}

	void CShop::deactivateControl()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideShop");

		GUI::CServer::getSingletonPtr()->getPlayerController()->activate();
		GUI::CServer::getSingletonPtr()->getCameraController()->activate();

	}


} // namespace Logic



