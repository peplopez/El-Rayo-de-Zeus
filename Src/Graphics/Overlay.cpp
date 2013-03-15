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


#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayManager.h>

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

	void COverlay::createChildPanel(const std::string& name, 
		float left, float top, float width, float height)
	{
		assert( !_overlayManager->hasOverlayElement(name) && "Ya existe un elemento hijo con el mismo nombre" );
		createChildElement(ChildType::PANEL, name, left, top, width, height);		
	} //createChildPanel

	//------------------------------------------------------------

	void COverlay::createChildTextArea(const std::string &name,
		float left, float top, float width, float height)
	{
		createChildElement(ChildType::TEXTAREA, name, left, top, width, height);
	} // createChildTextArea

	//------------------------------------------------------------

	
	void COverlay::createChildElement(const std::string &type, const std::string& name, 
		float left, float top, float width, float height)
	{		
		assert( !_overlayManager->hasOverlayElement(name) && "Ya existe un elemento hijo con el mismo nombre" );
		
		Ogre::OverlayElement* newChild = _overlayManager->createOverlayElement(type, name);
			newChild->setMetricsMode(_metricsMode); 
			newChild->setPosition(left, top);		// segun metricsMode: relativas o en pixeles
			newChild->setDimensions(width, height); 
		
		_childElements[name] = newChild;
	} // createChildTextArea

	//------------------------------------------------------------
		
	void COverlay::clear(){		
		assert(_overlay && "Overlay no inicializado!");
		_overlay->clear();
	} // clear
	


	
	/*******************
		GET's & SET's
	********************/		
		
	void COverlay::setChildFontSize(const std::string &childName, const float fontSize)
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
