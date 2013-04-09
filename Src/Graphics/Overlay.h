//---------------------------------------------------------------------------
// Overlay.h
//---------------------------------------------------------------------------

/**
@file Overlay.h

Contiene la declaración de la clase que maneja el Overlay.

@see Graphics::COverlay

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_Overlay_H
#define __Graphics_Overlay_H

#include <OgreOverlay.h>
#include <OgreOverlayElement.h> // FRS Necesario para el enum de MetricsMode


// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class OverlayManager;
};

namespace Graphics 
{
	
	/**
	Clase que representa un Overlay. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gráfico.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class COverlay
	{
	public:
		typedef std::map<std::string, Ogre::OverlayElement*> TChildElements;

		class TChildType{
		public:
			static const std::string TEXTAREA;
			static const std::string PANEL;
		};

		enum MetricsMode { 
			PIXELS = Ogre::GMM_PIXELS,		// medidas en pixeles 
			RELATIVE = Ogre::GMM_RELATIVE // (la otra opcion es proporciones de pantalla 0 -> 1)
		};
			
		/**
		Constructor de la clase.
		Genera un Overlay
		@param name Nombre del Overlay.
		*/
		COverlay(const std::string &name, MetricsMode metricsMode = PIXELS);		

		/**
		Destructor del overlay.
		*/
		~COverlay();

		/*********************
			CREATE CHILDS
		*********************/

		//	PANELS	
		void createChildPanel(const std::string& name, 
			float left, float top, float width, float height,
			const std::string &material = ""); 
	
		//	TEXT AREA		
		void createChildTextArea(const std::string &panelName, const std::string &name,
			float left, float top, float width, float height,
			const std::string &font, float fontSize, const std::string &text = "");

		/** Vacía el overlay de todos sus hijos*/ 
		void clear();


		/*****************
			GET's & SET's
		******************/
		
		/**
		Indica si el Overlay sera visible o invisible
		@param visible. Indicara si se ha de poner visible (true) u oculto (false)
		*/			
		void setVisible(bool visible){ visible ? _overlay->show() : _overlay->hide();} 

		/**
		Indica si el Overlay es visible o no
		@return Indicara si es visible (true) u oculto (false)
		*/
		bool isVisible() const { return _overlay->isVisible(); } 

		/**
		Indica si un elemento hijo del Overlay sera visible o invisible
		@param visible. Indicara si se ha de poner visible (true) u oculto (false)
		*/			
		void setChildVisible(const std::string &childName, bool visible){
			assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
			visible ? 
				_childElements[childName]->show() : 
				_childElements[childName]->hide();
		} 

		/**
		Indica si un elemento hijo del Overlay es visible o no
		@return Indicara si es visible (true) u oculto (false)
		*/
		bool isChildVisible(const std::string &childName){ 
			assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
			return _childElements[childName]->isVisible(); 
		} 

		/**
		Establece donde se posicionara el Overlay.
		Solo aplicable a los contenedores. 
		Se puede usar en pixeles o en posiciones de 0 a 1 (Siendo 1,1 la esquina superior izquierda).
		Para cambiar de sistema de unidades usar el metodo setMetricsMode
		@param left, establece el x donde comenzara el overlay
		@param top, establece el y donde comenzara el overlay
		*/
		void setChildPosition(const std::string &childName, float left, float top){
			assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
			_childElements[childName]->setPosition(left, top);
		} 

		/**
		Establece las dimensiones del overlay.
		Solo aplicable a los contenedores. 
		Se puede usar en pixeles o en posiciones de 0 a 1 (Siendo 1,1 la esquina superior izquierda).
		Para cambiar de sistema de unidades usar el metodo setMetricsMode 
		@param width, establece el ancho del overlay
		@param height, establece el largo del overlay
		*/
			
		void setChildDimensions(const std::string &childName, float width, float height){
			assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
			_childElements[childName]->setDimensions(width, height);		
		} 

		
		/**
		Establece la unidad de medida que se usara, debe indicarse por cada overlay. Por defecto se usa en pixeles.
		@param unit, unidad de medida que se usara, "pixel" para pixeles, "relative" para relativas
		*/
		void setChildMetricsMode(const std::string &childName, MetricsMode metricsMode){
			assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
			_childElements[childName]->setMetricsMode( static_cast<Ogre::GuiMetricsMode>(metricsMode) );
		}
			

		/**
		Solo aplicable a los contenedores. Asigna al overlay un material mediante su nombre.
		El material debera existir ya que no sino dara un error gordo.
		Dicho material poseera una textura con la imagen que se quiera mostrar por pantalla. 
		El material debera de tener las siguientes propiedades.
		<ul>
			<li>depth_check off</li>
			<li>depth_write off</li>
			<li>scene_blend alpha_blend</li>
			<li>lighting off</li>
			<li>cull_hardware none</li>
			<li>cull_software none</li>
		</ul>

		@param material, nombre del material que tendra el overlay.
		*/
		void setChildMaterial(const std::string &childName, const std::string& material) {
			assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
			_childElements[childName]->setMaterialName(material);
		}

		/**
		Asigna un texto a un overlay
		@param text, texto que se le asiganara al overlay
		*/
		void setChildText(const std::string &childName, const std::string& text){
			assert(_childElements[childName] && "No existe un elemento con ese nombre en este overlay!");
			_childElements[childName]->setCaption(text);
		}

		/**
		Cambia el tamaño de la letra por el indicado
		@param textSize, tamaño de la letra
		*/
		void setChildFontSize(const std::string &childName, float fontSize);
	

		/**
		Especifica la fuente que se usara, la fuente ha de tener ciertas caracteristicas.
		Mirar la carpeta fonts para mas info.
		@param font, fuente que se usara.
		*/
		void setChildFont(const std::string &childName, const std::string& font);
				
		


	private:

		Ogre::OverlayManager* _overlayManager;
		Ogre::Overlay *_overlay;
		TChildElements _childElements;

		std::string _name;
		Ogre::GuiMetricsMode _metricsMode;

		Ogre::OverlayElement* createChildElement(const std::string &type, const std::string& name, 
			float left, float top, float width, float height);
	
	}; // class COverlay

} // namespace Graphics

#endif // __Graphics_Overlay_H