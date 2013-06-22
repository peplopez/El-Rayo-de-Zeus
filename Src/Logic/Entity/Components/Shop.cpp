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
#include <BaseSubsystems/Server.h> // FRS Los archivos no locales es mejor importarlso con < > (como todos los que tienes a continuacion)

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

#include "ScriptManager\Server.h" //PT

#include "Logic/Server.h" //PT
#include "Logic/Maps/EntityFactory.h" //PT
#include "Logic/Maps/Map.h" //PT

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

		pm = _gameStatus->getPlayer(player)->getMeritPoints();

		//init Shop Layout and functions
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("Shop");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initShop");

		//Main Window of Shop
		_winShop = (CEGUI::FrameWindow*) CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop");

		//Properties of Main Window of Shop
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
		_itemsWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/ControlPestanas/Upgrades");

		item1window = CEGUI::WindowManager::getSingleton().loadWindowLayout( "item1.layout" );
		_itemsWindow->addChildWindow(item1window);
		//position x, and position y where windows it is shown
		item1window->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,5.0f) ) );
		item1window->setVisible( true );
		item1window->setInheritsAlpha(false);


		//se crea el tooltip
		CEGUI::System::getSingleton().setDefaultTooltip((CEGUI::utf8*)"TaharezLook/Tooltip"); // Set the name of the default tooltip
		CEGUI::Tooltip* tooltip = CEGUI::System::getSingleton().getDefaultTooltip();
		tooltip->setHoverTime(0.2f); // Display the tooltip after the mouse has been hovering over the widget for half a second
		tooltip->setDisplayTime(10.0f); // Display for 10 seconds then disappear
		tooltip->setFadeTime(1.0f); // Duration of the transition between fully visible and fully invisible
		// To set the tooltip text for a window simply call setTooltipText()   see staticText for an example


		//CEGUI::UDim x = tooltip->getPosition().d_x;
		//CEGUI::UDim y = tooltip->getPosition().d_y;

		//CEGUI::UDim xnew = x + tooltip->getWidth();
		//CEGUI::UDim ynew = y - tooltip->getHeight();

		//CEGUI::UVector2 vector = CEGUI::UVector2(xnew, ynew);

		//tooltip->setPosition(vector);


		item2window = CEGUI::WindowManager::getSingleton().loadWindowLayout( "item2.layout" );
		_itemsWindow->addChildWindow(item2window);
		item2window->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,49.0f) ) );
		item2window->setInheritsAlpha(false);

		item3window = CEGUI::WindowManager::getSingleton().loadWindowLayout( "item3.layout" );
		_itemsWindow->addChildWindow(item3window);
		item3window->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,93.0f) ) );
		item3window->setInheritsAlpha(false);


		//CRIATURES

		_criaturesWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/ControlPestanas/Allies");

		//The windows that are being loading are Taharez/ImageButtons
		medusawindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "medusa.layout" );
		_criaturesWindow->addChildWindow(medusawindow);
		medusawindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,10.0f) ) );
		medusawindow->setVisible( true );
		medusawindow->setInheritsAlpha(false);
		medusawindow->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::createMedusa, this)
									 );

		cancerberowindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "cancerbero.layout" );
		_criaturesWindow->addChildWindow(cancerberowindow);
		cancerberowindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,70.0f) ) );
		cancerberowindow->setInheritsAlpha(false);
		cancerberowindow->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::createCerberus, this)
									 );

		minotaurowindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "minotauro.layout" );
		_criaturesWindow->addChildWindow(minotaurowindow);
		minotaurowindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,130.0f) ) );
		minotaurowindow->setInheritsAlpha(false);



		//COMBOS
		/*_comboWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/ControlPestanas/Combos");*/



		//GRADOS
		//grados = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Grados");


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

    //---------------------------------------------------------
	void CShop::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//Important, in order to tooltip can be shown
		CEGUI::System::getSingleton().injectTimePulse(msecs*0.001f);

		//Para mostrar por pantalla los grados en los que se encuentra el Player
		//para poder calcular los grados donde debia estar la criatura seleccionada
		//gradosstr.str("");
		//gradosstr << (unsigned short)_gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getDegree();
		//grados->setText(gradosstr.str());

		_time += msecs;

		//se recupera la informacion de Logic::CGameStatus cada medio segundo para actualizar el HUD
		if(_time >= 500)
		{
			_time = 0;
			//Merite Points
			ScriptManager::CServer::getSingletonPtr()->executeProcedure("showPMSHop",_gameStatus->getPlayer(player)->getMeritPoints());
		}
		
	}//tick

    //---------------------------------------------------------

	void CShop::displayShop()
	{
		//ScriptManager::CServer::getSingletonPtr()->executeProcedure("changeShopVisibility");

		if(_winShop->isVisible())
			deactivateControl();
		else
			activateControl();

	}
    //---------------------------------------------------------

	bool CShop::handleClose(const CEGUI::EventArgs&e)
	{
		deactivateControl();
		return true;
	}

    //---------------------------------------------------------

	void CShop::createAlly(const std::string &type, const unsigned int cost)
	{
		pm = _gameStatus->getPlayer(player)->getMeritPoints();

		if(pm >= cost)
		{

			std::ostringstream basestring;
			basestring << "map" << numBase;

			unsigned short int ring = _gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getRing();
			unsigned short int degree =_gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getDegree();
			unsigned short int sense = _gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getSense();

			short int newdegree = degree;

			if(sense==LogicalPosition::RIGHT) //seeing right
				newdegree-= 10;
			else if(LogicalPosition::LEFT) //seeing left
				newdegree+= 10;

			//correction in the degrees where ally appears
			if(newdegree<0)
				newdegree = 360 + newdegree;
			if(newdegree > 360)
				newdegree = newdegree - 360;


			//Funcion para crear la entidad Aliada
			//PARAMETROS
			// nombreEntidad, Tipo de aliado, base origen (porque siempre se creara la criatura en su
			//base origen, no en el resto de bases), anillo, grados 

			Logic::CServer::getSingletonPtr()->getMap(basestring.str())->createAlly(
				type,
				type,
				numBase, 
				_gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getRing(),
				newdegree,
				_gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getSense() 
			);

			_gameStatus->getPlayer(player)->setMeritPoints(pm-cost);
		}

	}
    //---------------------------------------------------------

	bool CShop::createCerberus(const CEGUI::EventArgs&e)
	{

		const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
		if (args.button == CEGUI::LeftButton)
		{
			createAlly("Cancerbero", 500);
			return true;
		}
		return false;
	}

    //---------------------------------------------------------

	bool CShop::createMedusa(const CEGUI::EventArgs&e)
	{

		const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
		if (args.button == CEGUI::LeftButton)
		{
			createAlly("Medusa", 300);
			return true;
		}
		return false;
	}
	//---------------------------------------------------------
	

	bool CShop::activateControl()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showShop");

		//PT para limitar el uso del raton en la ventana de la tienda del Olimpo
		//CEGUI::MouseCursor::getSingletonPtr()->setConstraintArea(&_area);	

		// HACK FRS Windowed? -> Desactivar el cursor de CEGUI y superponer el de WIN32
#if _WIN32	
		if(BaseSubsystems::CServer::getSingletonPtr()->isWindowedMode() )
			CEGUI::MouseCursor::getSingletonPtr()->setVisible(false);
#endif

		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();
		GUI::CServer::getSingletonPtr()->getCameraController()->deactivate();

		return true;
	}
    //---------------------------------------------------------

	void CShop::deactivateControl()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideShop");

		GUI::CServer::getSingletonPtr()->getPlayerController()->activate();
		GUI::CServer::getSingletonPtr()->getCameraController()->activate();

	}


} // namespace Logic



