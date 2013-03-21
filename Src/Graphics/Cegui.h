////---------------------------------------------------------------------------
//// Cegui.h
////---------------------------------------------------------------------------
//
///**
//@file Cegui.h
//
//Contiene la declaración de la clase que maneja el Cegui.
//
//@see Graphics::CCegui
//
//@author Pablo Terrado
//@date Enero, 2013
//*/
//
//#ifndef __Graphics_Cegui_H
//#define __Graphics_Cegui_H
//
//
//#include <cegui\CEGUIWindow.h>
//#include <cegui\CEGUISystem.h>
//#include <cegui\CEGUIWindowManager.h>
//
//
//// Predeclaración de clases para ahorrar tiempo de compilación
//namespace CEGUI
//{
//	class System;
//	class Window;
//}
//
//namespace Graphics 
//{
//	
//	/**
//	Clase que representa un Cegui. Contiene una referencia a
//	una entidad de Ogre y al Nodo que la contiene.
//	Oculta los detalles escabrosos del motor gráfico.
//	
//	@ingroup graphicsGroup
//
//	@author Pablo Terrado
//	@date Marzo, 2013
//	*/
//	class CCegui
//	{
//	public:
//
//		/**
//		Constructor de la clase.
//		Genera un Cegui
//		@param name Nombre del Cegui.
//		*/
//		CCegui(const std::string &name);		
//
//		/**
//		Destructor del Cegui.
//		*/
//		~CCegui();
//
//		/*****************
//			GET's & SET's
//		******************/
//		
//		/**
//		Indica si el Cegui es visible o no
//		@return Indicara si es visible (true) u oculto (false)
//		*/
//		bool isVisible() const { return _cegui->isVisible(); } 
//
//		/**
//		Asigna un texto a un Cegui
//		@param text, texto que se le asiganara al Cegui
//		*/
//		void setText(const std::string &text);
//
//		void cargarFicheroLayout(const std::string &nameLayout);
//
//		void cargarVentana(const std::string &nameWindow);
//		
//		CEGUI::Window* conseguirHijo(const std::string &nameChild);
//
//		void establecerPropiedad(CEGUI::Window* _window, const std::string &nameProperty, const std::string &valueProperty);
//				
//	private:
//		
//		CEGUI::Window *_cegui;
//		std::string _name;
//
//	
//	}; // class CCegui
//
//} // namespace Graphics
//
//#endif // __Graphics_Cegui_H