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
#include <BaseSubsystems/Server.h> // FRS Los archivos no locales es mejor importarlos con < > (como todos los que tienes a continuacion)

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

		//UPGRADES
		tercerComboWindow->destroy();
		rezoHadesWindow->destroy();
		rezoTiqueWindow->destroy();
		rezoArtemisaWindow->destroy();
		rezoZeusWindow->destroy();
		rezoCronosWindow->destroy();
		rezoAresWindow->destroy();
		rezoAteneaWindow->destroy();
		fortalecerBaseWindow->destroy();
		debilitarBasesWindow->destroy();
		sacrificioWindow->destroy();
		sacrificioDefinitivoWindow->destroy();

		//ALLIES AND CRIATURES
		gorgonawindow->destroy();
		helenatroyawindow->destroy();
		centaurowindow->destroy();
		minotaurowindow->destroy();
		cerberuswindow->destroy();
		ciclopewindow->destroy();

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

		float posicionVertical = 0.0f;
		//posicionVertical+=5.0f;


		//UPGRADES TAB
		_upgradesWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/TabControl/Upgrades");

		tercerComboWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "tercerCombo.layout" );
		_upgradesWindow->addChildWindow(tercerComboWindow);
		//position x, and position y where windows it is shown
		tercerComboWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		tercerComboWindow->setVisible( true );
		tercerComboWindow->setInheritsAlpha(false);
		tercerComboWindow->getChild("tercerCombo/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::activateThirdCombo, this)
									 );

		//Tooltip is created
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


		posicionVertical+=44.0f;
		rezoHadesWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "rezoHades.layout" );
		_upgradesWindow->addChildWindow(rezoHadesWindow);
		rezoHadesWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		rezoHadesWindow->setInheritsAlpha(false);
		rezoHadesWindow->getChild("rezoHades/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::rezoHades, this)
									 );

		posicionVertical+=44.0f;
		rezoTiqueWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "rezoTique.layout" );
		_upgradesWindow->addChildWindow(rezoTiqueWindow);
		rezoTiqueWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		rezoTiqueWindow->setInheritsAlpha(false);
		rezoTiqueWindow->getChild("rezoTique/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::rezoTique, this)
									 );

		posicionVertical+=44.0f;
		rezoArtemisaWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "rezoArtemisa.layout" );
		_upgradesWindow->addChildWindow(rezoArtemisaWindow);
		rezoArtemisaWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		rezoArtemisaWindow->setInheritsAlpha(false);
		rezoArtemisaWindow->getChild("rezoArtemisa/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::rezoArtemisa, this)
									 );

		posicionVertical+=44.0f;
		rezoZeusWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "rezoZeus.layout" );
		_upgradesWindow->addChildWindow(rezoZeusWindow);
		rezoZeusWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		rezoZeusWindow->setInheritsAlpha(false);
		rezoZeusWindow->getChild("rezoZeus/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::rezoZeus, this)
									 );

		posicionVertical+=44.0f;
		rezoCronosWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "rezoCronos.layout" );
		_upgradesWindow->addChildWindow(rezoCronosWindow);
		rezoCronosWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		rezoCronosWindow->setInheritsAlpha(false);
		rezoCronosWindow->getChild("rezoCronos/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::rezoCronos, this)
									 );

		posicionVertical+=44.0f;
		rezoAresWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "rezoAres.layout" );
		_upgradesWindow->addChildWindow(rezoAresWindow);
		rezoAresWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		rezoAresWindow->setInheritsAlpha(false);
		rezoAresWindow->getChild("rezoAres/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::rezoAres, this)
									 );

		posicionVertical+=44.0f;
		rezoAteneaWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "rezoAtenea.layout" );
		_upgradesWindow->addChildWindow(rezoAteneaWindow);
		rezoAteneaWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		rezoAteneaWindow->setInheritsAlpha(false);
		rezoAteneaWindow->getChild("rezoAtenea/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::rezoAtenea, this)
									 );

		posicionVertical+=44.0f;
		fortalecerBaseWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "fortalecerBase.layout" );
		_upgradesWindow->addChildWindow(fortalecerBaseWindow);
		fortalecerBaseWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		fortalecerBaseWindow->setInheritsAlpha(false);
		fortalecerBaseWindow->getChild("fortalecerBase/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::fortalecerBase, this)
									 );

		posicionVertical+=44.0f;
		debilitarBasesWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "debilitarBases.layout" );
		_upgradesWindow->addChildWindow(debilitarBasesWindow);
		debilitarBasesWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		debilitarBasesWindow->setInheritsAlpha(false);
		debilitarBasesWindow->getChild("debilitarBases/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::debilitarBases, this)
									 );

		posicionVertical+=44.0f;
		sacrificioWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "sacrificio.layout" );
		_upgradesWindow->addChildWindow(sacrificioWindow);
		sacrificioWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		sacrificioWindow->setInheritsAlpha(false);
		sacrificioWindow->getChild("sacrificio/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::sacrificio, this)
									 );

		posicionVertical+=44.0f;
		sacrificioDefinitivoWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "sacrificioDefinitivo.layout" );
		_upgradesWindow->addChildWindow(sacrificioDefinitivoWindow);
		sacrificioDefinitivoWindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,5.0f), CEGUI::UDim(0,posicionVertical) ) );
		sacrificioDefinitivoWindow->setInheritsAlpha(false);
		sacrificioDefinitivoWindow->getChild("sacrificioDefinitivo/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::sacrificioDefinitivo, this)
									 );


		//CRIATURES OR ALLIES TAB
		_alliesWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/TabControl/Allies");

		posicionVertical=10.0f;

		gorgonawindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "gorgona.layout" );
		_alliesWindow->addChildWindow(gorgonawindow);
		gorgonawindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,posicionVertical) ) );
		gorgonawindow->setVisible( true );
		gorgonawindow->setInheritsAlpha(false);
		//gorgonawindow->subscribeEvent(CEGUI::FrameWindow::EventMouseButtonUp,
		//							 CEGUI::Event::Subscriber(&CShop::createGorgona, this)
		//							 );
		gorgonawindow->getChild("Gorgona/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::createGorgona, this)
									 );

		posicionVertical+=60.0f;
		helenatroyawindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "helenaTroya.layout" );
		_alliesWindow->addChildWindow(helenatroyawindow);
		helenatroyawindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,posicionVertical) ) );
		helenatroyawindow->setVisible( true );
		helenatroyawindow->setInheritsAlpha(false);
		//helenatroyawindow->subscribeEvent(CEGUI::FrameWindow::EventMouseButtonUp,
		//							 CEGUI::Event::Subscriber(&CShop::createHelenaTroya, this)
		//							 );
		helenatroyawindow->getChild("HelenaTroya/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::createHelenaTroya, this)
									 );

		posicionVertical+=60.0f;
		centaurowindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "centauro.layout" );
		_alliesWindow->addChildWindow(centaurowindow);
		centaurowindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,posicionVertical) ) );
		centaurowindow->setVisible( true );
		centaurowindow->setInheritsAlpha(false);
		//centaurowindow->subscribeEvent(CEGUI::FrameWindow::EventMouseButtonUp,
		//							 CEGUI::Event::Subscriber(&CShop::createCentauro, this)
		//							 );
		centaurowindow->getChild("Centauro/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::createCentauro, this)
									 );

		posicionVertical+=60.0f;
		minotaurowindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "minotauro.layout" );
		_alliesWindow->addChildWindow(minotaurowindow);
		minotaurowindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,posicionVertical) ) );
		minotaurowindow->setVisible( true );
		minotaurowindow->setInheritsAlpha(false);
		//minotaurowindow->subscribeEvent(CEGUI::FrameWindow::EventMouseButtonUp,
		//							 CEGUI::Event::Subscriber(&CShop::createMinotauro, this)
		//							 );
		minotaurowindow->getChild("Minotauro/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::createMinotauro, this)
									 );


		posicionVertical+=60.0f;
		cerberuswindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "cerberus.layout" );
		_alliesWindow->addChildWindow(cerberuswindow);
		cerberuswindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,posicionVertical) ) );
		cerberuswindow->setInheritsAlpha(false);
		//cerberuswindow->subscribeEvent(CEGUI::DefaultWindow::EventMouseButtonUp,
		//							 CEGUI::Event::Subscriber(&CShop::createCerberus, this)
		//							 );
		cerberuswindow->getChild("Cerberus/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::createCerberus, this)
									 );

		posicionVertical+=60.0f;
		ciclopewindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "ciclope.layout" );
		_alliesWindow->addChildWindow(ciclopewindow);
		ciclopewindow->setPosition( CEGUI::UVector2( CEGUI::UDim(0,10.0f), CEGUI::UDim(0,posicionVertical) ) );
		ciclopewindow->setInheritsAlpha(false);
		//ciclopewindow->subscribeEvent(CEGUI::DefaultWindow::EventMouseButtonUp,
		//							 CEGUI::Event::Subscriber(&CShop::createCiclope, this)
		//							 );
		ciclopewindow->getChild("Ciclope/btn")->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp,
									 CEGUI::Event::Subscriber(&CShop::createCiclope, this)
									 );


		//COMBOS
		/*_comboWindow = CEGUI::WindowManager::getSingletonPtr()->getWindow("Root/Shop/TabControl/Combos");*/
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

		//se recupera la informacion de Logic::CGameStatus cada medio segundo para actualizar la tienda
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
	

	bool CShop::activateControl()
	{
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showShop");

		//PT para limitar el uso del raton en la ventana de la tienda del Olimpo
		//CEGUI::MouseCursor::getSingletonPtr()->setConstraintArea(&_area);	

		// HACK FRS Windowed? -> Desactivar el cursor de CEGUI y superponer el de WIN32
		#if _WIN32	
				if(BaseSubsystems::CServer::getSingletonPtr()->isWindowedMode() )
					//CEGUI::MouseCursor::getSingletonPtr()->setVisible(false); //FER
					CEGUI::MouseCursor::getSingletonPtr()->hide();				//PT
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

	//---------------------------------------------------------



	//---------------------------------------------------------
	//-------- FUNCTIONS HANDLED BY ALLIES ICONS  -------------
    //---------------------------------------------------------

	void CShop::createAlly(const std::string &type, const unsigned int cost)
	{
		pm = _gameStatus->getPlayer(player)->getMeritPoints();

		if(pm >= cost)
		{

			std::ostringstream basestring;
			//basestring << "map" << numBase;
			basestring << "mapRed";

			unsigned short int ring = _gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getRing();
			unsigned short int degree =_gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getDegree();
			unsigned short int sense = _gameStatus->getPlayer(player)->getPlayer()->getLogicalPosition()->getSense();

			//correct sense if player is looking in another direction that LEFT or RIGHT
			if(sense!=LogicalPosition::Sense::LEFT && sense!=LogicalPosition::Sense::RIGHT)
				sense = LogicalPosition::Sense::LEFT;


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
				sense 
			);

			_gameStatus->getPlayer(player)->setMeritPoints(pm-cost);
		}

	}
    //---------------------------------------------------------


	bool CShop::createGorgona(const CEGUI::EventArgs&e)
	{

		const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
		if (args.button == CEGUI::LeftButton)
		{
			createAlly("Medusa", 300);
			return true;
		}
		return false;
	}

	bool CShop::createHelenaTroya(const CEGUI::EventArgs&e)
	{

		const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
		if (args.button == CEGUI::LeftButton)
		{
			createAlly("Medusa", 300);
			return true;
		}
		return false;
	}

	bool CShop::createCentauro(const CEGUI::EventArgs&e)
	{

		const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
		if (args.button == CEGUI::LeftButton)
		{
			createAlly("Medusa", 300);
			return true;
		}
		return false;
	}

	bool CShop::createMinotauro(const CEGUI::EventArgs&e)
	{

		const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
		if (args.button == CEGUI::LeftButton)
		{
			createAlly("Medusa", 300);
			return true;
		}
		return false;
	}

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



	bool CShop::createCiclope(const CEGUI::EventArgs&e)
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
	//-------- FUNCTIONS HANDLED BY UPGRADES ICONS  -----------
    //---------------------------------------------------------

	bool CShop::activateThirdCombo(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::rezoHades(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::rezoTique(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::rezoArtemisa(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::rezoZeus(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::rezoCronos(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::rezoAres(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::rezoAtenea(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::fortalecerBase(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::debilitarBases(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::sacrificio(const CEGUI::EventArgs&e)
	{
		return true;
	}

	bool CShop::sacrificioDefinitivo(const CEGUI::EventArgs&e)
	{
		return true;
	}



} // namespace Logic



