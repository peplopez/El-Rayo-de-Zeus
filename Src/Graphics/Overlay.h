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

#include "BaseSubsystems/Math.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneManager;
	class Overlay;
	class OverlayManager;
	class OverlayContainer;
	class TextAreaOverlayElement;
}

namespace Graphics 
{
	class CScene;
	class CServer;
}

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

		/**
		Constructor de la clase.
		Genera un Overlay estandar, que poseera a su ver Contenedores con las respectivas imagenes.
		*/
		COverlay(Ogre::Overlay* overlay);
		COverlay(Ogre::OverlayElement* overlay);

		/**
		Constructor de la clase.
		Genera un Overlay, puede ser TextArea o Panel, que luego sera introducido en un overlay
		@param name Nombre del Overlay.
		*/
		COverlay(const std::string &name);
		COverlay(const std::string &name, const std::string& type = "");

		/**
		Destructor de la aplicación.
		*/
		virtual ~COverlay();


		/**
		Añade un Overlay a otro para establecer grupos. 
		@param overlayContainer, overlay que tendra ubicado dentro, tiene que ser un <b>Panel</b>.
		El material de estos debe de tener ciertas caracteristicas, mirar setMaterial para mas info.
		*/
		void add2D(COverlay* overlayContainer);

		/**
		Añade un overlayContainer a otro, estableciendo una secuencia de padres e hijos.
		Asegurate que el overlay hijo es del tipo <b> TextArea </b> y el padre del tipo <b> Panel </b>
		@param child overlay que sera el hijo.
		*/
		void addChild(COverlay* child);

		/** FRS Vacía el overlay de todos sus hijos
		*/
		void clear();


		/*****************
			GET's & SET's
		******************/

		/**
		Indica si un Overlay sera visible o invisible
		@param visible. Indicara si se ha de poner visible (true) u oculto (false)
		*/
		void setVisible(bool visible);
		 
		/**
		Indica si un Overlay es visible o no
		@return Indicara si es visible (true) u oculto (false)
		*/
		bool isVisible();

		
		/**
		Establece donde se posicionara el Overlay.
		Solo aplicable a los contenedores. 
		Se puede usar en pixeles o en posiciones de 0 a 1 (Siendo 1,1 la esquina superior izquierda).
		Para cambiar de sistema de unidades usar el metodo setMetricsMode
		@param left, establece el x donde comenzara el overlay
		@param top, establece el y donde comenzara el overlay
		*/
		void setPosition(float left, float top);

		/**
		Establece las dimensiones del overlay.
		Solo aplicable a los contenedores. 
		Se puede usar en pixeles o en posiciones de 0 a 1 (Siendo 1,1 la esquina superior izquierda).
		Para cambiar de sistema de unidades usar el metodo setMetricsMode 
		@param width, establece el ancho del overlay
		@param height, establece el largo del overlay
		*/
		void setDimensions(float width, float height);
		
		/**
		Establece la unidad de medida que se usara, debe indicarse por cada overlay. Por defecto se usa en pixeles.
		@param unit, unidad de medida que se usara, "pixel" para pixeles, "relative" para relativas
		*/
		void setMetricsMode(std::string unit);

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
		void setMaterial(const std::string& material);

		/**
		Asigna un texto a un overlay
		@param text, texto que se le asiganara al overlay
		*/
		void setText(const std::string& text);

		/**
		Cambia el tamaño de la letra por el indicado
		@param textSize, tamaño de la letra
		*/
		void setTextSize(const float textSize);

		/**
		Especifica la fuente que se usara, la fuente ha de tener ciertas caracteristicas.
		Mirar la carpeta fonts para mas info.
		@param font, fuente que se usara.
		*/
		void setFont(const std::string& font);


		

		/**
		metodo que devuelve el overlayContainer
		@return la variable que contiene el elemento
		*/
		Ogre::OverlayContainer* getOverlayContainer(){ return _overlayContainer;};
		
		/**
		metodo que devuelve el overlayElement
		@return la variable que contiene el elemento
		*/
		Ogre::TextAreaOverlayElement* getOverlayText(){ return _overlayText;};
		
	protected:

		// CScene es la única que puede añadir o eliminar overlays sí misma
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		Añade la entidad al SceneManager pasado por parámetro. Si la entidad
		no está cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere añadir
		la entidad.
		@return true si la entidad se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y eliminó de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();


	private:

		std::string _type;
		Ogre::Overlay *_overlay;
		Ogre::OverlayContainer *_overlayContainer;
		Ogre::TextAreaOverlayElement *_overlayText;

		static enum ColorValue { colorNotDefined,White,Black,Red,Green,Blue };

	}; // class COverlay

} // namespace Graphics

#endif // __Graphics_Overlay_H