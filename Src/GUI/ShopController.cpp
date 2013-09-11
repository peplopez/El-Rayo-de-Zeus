
/**
@file ShopController.cpp

Contiene la implementación de la clase CShopController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
cambiar la tienda del Olimpo

@see GUI::CShopController

@author Pablo Terrado Contreras
@date Junio, 2013
*/

#include "ShopController.h"
#include "InputManager.h"
#include "Logic/Entity/Entity.h"
#include "ScriptManager\Server.h"
#include "Logic/Entity/Messages/Message.h"

namespace GUI {

	CShopController::CShopController():_controlledShop(0)
	{
		//activate();
	} // CShopController

	//--------------------------------------------------------

	CShopController::~CShopController()
	{
		deactivate();
	} // ~CShopController

	//--------------------------------------------------------

	void CShopController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);
	} // activate

	//--------------------------------------------------------

	void CShopController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
	} // deactivate

	//--------------------------------------------------------

	bool CShopController::keyPressed(TKey key)
	{
		return false;
	} // keyPressed

	//--------------------------------------------------------

	bool CShopController::keyReleased(TKey key)
	{
		if(_controlledShop)
		{
			switch(key.keyId)
			{
				case GUI::Key::T: {
					//ScriptManager::CServer::getSingletonPtr()->reloadScript("Shop");
					//ScriptManager::CServer::getSingletonPtr()->executeProcedure("changeShopVisibility");
					Logic::CMessage *m = new Logic::CMessage();
					m->setType(Logic::Message::SHOP);
					m->setAction(Logic::Message::DISPLAY_SHOP); 
					_controlledShop->emitMessage(m);
					break;
				}

				//case GUI::Key::R:
				//	ScriptManager::CServer::getSingletonPtr()->reloadScript("Shop");
				//	ScriptManager::CServer::getSingletonPtr()->executeProcedure("reloadShop");
				//	break;

				default:
					return false;
			}
		}
		return true;

	} // keyReleased


} // namespace GUI


