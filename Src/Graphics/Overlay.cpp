//---------------------------------------------------------------------------
// Overlay.cpp
//---------------------------------------------------------------------------

/**
@file Overlay.cpp

Contiene la implementación de la clase que representa un Overlay.

@see Graphics::COverlay

@author Pablo Terrado
@date Enero, 2013
*/

#include "Overlay.h"
#include "Scene.h"
#include "Server.h"

#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>

namespace Graphics 
{
	COverlay::COverlay(Ogre::Overlay* overlay){
		_overlay = overlay;
	}
	 
	COverlay::COverlay(Ogre::OverlayElement* overlay){
		if(std::string::npos != overlay->getName().find("Panel"))
			_overlayContainer = static_cast<Ogre::OverlayContainer*>(overlay);		
		else
			_overlayText = static_cast<Ogre::TextAreaOverlayElement*>(overlay);
		
	} // COverlay

	COverlay::COverlay(const std::string &name, const std::string &type){

		_overlay = NULL;
		_overlayContainer = NULL;
		_overlayText = NULL;

		if(type.empty()){
			_overlay = Graphics::CServer::getSingletonPtr()->getOverlayManager()->create(name);
		}else{
			if(type == "Panel")
				_overlayContainer = static_cast<Ogre::OverlayContainer*> (Graphics::CServer::getSingletonPtr()->getOverlayManager()->createOverlayElement(type, name));
			if(type == "TextArea")
				_overlayText = static_cast<Ogre::TextAreaOverlayElement*>(Graphics::CServer::getSingletonPtr()->getOverlayManager()->createOverlayElement(type, name)) ;
		}
		_type = type;
	} // COverlay

	//------------------------------------------------------------

	COverlay::~COverlay(){
	
	} // ~COverlay

	//------------------------------------------------------------

	void COverlay::setVisible(bool visible){
		if(visible){
			if(_overlay){ _overlay->show();}
			/*if(_overlayContainer){ _overlayContainer->show();}
			if(_overlayText){ _overlayText->show();}*/
		}else{
			if(_overlay){ _overlay->hide();}
			/*if(_overlayContainer){ _overlayContainer->hide();}
			if(_overlayText){ _overlayText->hide();}*/
		}
	} // setVisible

	//------------------------------------------------------------

	bool COverlay::isVisible(){

		if(_overlay){return _overlay->isVisible();}
		if(_overlayContainer){return _overlayContainer->isVisible();}
		if(_overlayText){return _overlayText->isVisible();}
	} // isVisible

	//------------------------------------------------------------

	void COverlay::add2D(COverlay* overlayContainer){
		if(_overlay)
			_overlay->add2D(overlayContainer->getOverlayContainer()); 	
		
	} // add2D

	//------------------------------------------------------------

	
	/*void COverlay::add3D(CScene* scene){
		if(_overlay){
			_overlay->add3D( scene->getSceneMgr()->getRootSceneNode());
		}

	} // add3D
	*/

	//------------------------------------------------------------


	void COverlay::setPosition(float left, float top){
		if(_overlayContainer){ _overlayContainer->setPosition(left, top);}
		if(_overlayText){ _overlayText->setPosition(left, top);}
	} // setPosition

	//------------------------------------------------------------

	
	void COverlay::setDimensions(float width, float height){
		if(_overlayContainer){ _overlayContainer->setDimensions(width, height);}
		if(_overlayText){ _overlayText->setDimensions(width, height);}
	} // setDimensions

	//------------------------------------------------------------

	
	void COverlay::setMetricsMode(std::string unit){
		Ogre::GuiMetricsMode metricsMode = Ogre::GMM_RELATIVE;
		if(unit == "pixel")
			metricsMode = Ogre::GMM_PIXELS;

		if(_overlayContainer){ _overlayContainer->setMetricsMode(metricsMode);}
		if(_overlayText){ _overlayText->setMetricsMode(metricsMode);}
	} // setMetricsMode

	//------------------------------------------------------------

	
	void COverlay::setMaterial(const std::string& material){
		if(_overlayContainer){ _overlayContainer->setMaterialName(material);}
		if(_overlayText){ _overlayText->setMaterialName(material);}
	} // setMaterial

	//------------------------------------------------------------

	
	void COverlay::setText(const std::string& text){
		if(_overlayContainer){ _overlayContainer->setCaption(text);}
		if(_overlayText){ _overlayText->setCaption(text);}
	} // setText

	//------------------------------------------------------------


	void COverlay::setTextSize(const float textSize){
		if(_overlayText){ _overlayText->setCharHeight(textSize);}
	} // setTextSize

	//------------------------------------------------------------


	void COverlay::setFont(const std::string& font){
		if(_overlayText){ 
			_overlayText->setFontName(font);
		}
	} // setFont

	//------------------------------------------------------------


	void COverlay::addChild(COverlay* child){
		if(_overlayContainer)
			_overlayContainer->addChild(child->getOverlayText());
	} // addChild

	//-----------------------------------------------------------
	
	void COverlay::clear(){
		if(_overlay)
			_overlay->clear();
	} // clear


} // namespace Graphics
