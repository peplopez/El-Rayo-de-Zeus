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

#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>


namespace Graphics 
{	
	const std::string COverlay::ChildType::TEXTAREA = "TextArea";
	const std::string COverlay::ChildType::PANEL = "Panel";

	COverlay::COverlay(const std::string &name, MetricsMode metricsMode) :
		_name(name), _overlayManager(0), _overlay(0)
	{
		// TODO FRS quiza lo suyo sería un metodo load como segunda fase, por si esto peta...?
		_overlayManager = Ogre::OverlayManager::getSingletonPtr();
		assert( !_overlayManager->getByName(name) && "Ya existe un overlay con el mismo nombre!" );
		_overlay = _overlayManager->create(name);		

		_metricsMode = static_cast<Ogre::GuiMetricsMode>(metricsMode);
	} // COverlay

	//------------------------------------------------------------

	COverlay::~COverlay(){

		// TODO FRS Comprobar "Make sure you're using this in other overlay..."
		// Quizá haya que destruir los hijos después del _overlay padre
		// o quizá la destrucción del padre conlleva la de los hijos...
		
		_overlay->clear(); // FRS Deattach all children

		TChildElements::const_iterator it = _childElements.begin();
		TChildElements::const_iterator end = _childElements.end();
			for(; it != end; ++it)
				_overlayManager->destroyOverlayElement( it->second );

		if(_overlay) 
			_overlayManager->destroy(_overlay);	

	} // ~COverlay

	//------------------------------------------------------------
		
	Ogre::OverlayElement* COverlay::createChildElement(const std::string &type, const std::string& name, 
		float left, float top, float width, float height)
	{		
		assert( !_overlayManager->hasOverlayElement(name) && "Ya existe un elemento hijo con el mismo nombre" );
		
		Ogre::OverlayElement* newChild = _overlayManager->createOverlayElement(type, name);
			newChild->setMetricsMode(_metricsMode); 
			newChild->setPosition(left, top);		// segun metricsMode: relativas o en pixeles
			newChild->setDimensions(width, height); 
		
		_childElements[name] = newChild;

		return newChild;
	} // createChildTextArea


	//------------------------------------------------------------
	
	void COverlay::createChildPanel(const std::string& name, 
		float left, float top, float width, float height,
		const std::string& material)
	{
		assert( !_overlayManager->hasOverlayElement(name) && "Ya existe un elemento hijo con el mismo nombre" );
		
		Ogre::OverlayElement* newChild = 
			createChildElement(ChildType::PANEL, name, left, top, width, height);

		if(material.length() > 0)
			newChild->setMaterialName(material);

		_overlay->add2D( static_cast<Ogre::OverlayContainer*>(newChild) );
	} //createChildPanel

	//------------------------------------------------------------


	void COverlay::createChildTextArea(const std::string &panelName, const std::string &name,
		float left, float top, float width, float height,
		const std::string &font, float fontSize, const std::string &text)
	{
		assert(_childElements[panelName] && "No existe un panel hijo con ese nombre en este overlay!");

		Ogre::TextAreaOverlayElement* newChild = static_cast<Ogre::TextAreaOverlayElement*>(
			createChildElement(ChildType::TEXTAREA, name, left, top, width, height)
		);									
			newChild->setFontName(font);
			newChild->setCharHeight(fontSize);
			newChild->setCaption(text);

		static_cast<Ogre::OverlayContainer*>( _childElements[panelName] )->addChild(newChild);
	} // createChildTextArea

	//------------------------------------------------------------
		
	void COverlay::clear(){		
		assert(_overlay && "Overlay no inicializado!");
		_overlay->clear();
	} // clear
	


	
	/*******************
		GET's & SET's
	********************/		
		
	void COverlay::setChildFontSize(const std::string &childName, float fontSize)
	{
		assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
		assert(_childElements[childName]->getTypeName() == ChildType::TEXTAREA
			&& "El elemento referenciado no es un TextArea");

		static_cast<Ogre::TextAreaOverlayElement*>( _childElements[childName] )
			->setCharHeight(fontSize);
	}
	// setChildTextSize

	//------------------------------------------------------------


	void COverlay::setChildFont(const std::string &childName, const std::string& font){
		
		assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
		assert(_childElements[childName]->getTypeName() == ChildType::TEXTAREA
			&& "El elemento referenciado no es un TextArea");

		static_cast<Ogre::TextAreaOverlayElement*>( _childElements[childName] )
			->setFontName(font);		
	} // setChildFont

	


} // namespace Graphics
