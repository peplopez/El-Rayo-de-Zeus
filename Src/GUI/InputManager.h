//---------------------------------------------------------------------------
// InputManager.h
//---------------------------------------------------------------------------

/**
@file InputManager.h

Contiene la declaración del gestor de periféricos de entrada
así como las clases oyentes que deben extender las clases que
quieran ser avisadas de los eventos de dichos periféricos:
ratón y teclado.

@see GUI::CInputManager
@see GUI::CKeyboardListener
@see GUI::CMouseListener

@author David Llansó
@date Julio, 2010
*/

#ifndef __GUI_InputManager_H
#define __GUI_InputManager_H

#include <OISMouse.h>
#include <OISKeyboard.h>

#include <list>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace OIS
{
	class InputManager;
}

/**
Namespace con todo lo que se refiere a la interfaz con el usuario.
Recoge y procesa los eventos del ratón y teclado e informa al 
usuario cuando es necesario.
(para más información ver @ref GUIGroup).

@author David Llansó
@date Julio, 2010
*/
namespace GUI
{
	/**
	Namespace que contiene el tipo que define los diferentes códigos de 
	los botones del	ratón. De esta manera independizamos el resto de la 
	aplicación de la biblioteca OIS.
	*/
	namespace Button
	{
		/**
		Tipo para los códigos de los botones del ratón. Declararlos aquí 
		tiene como función independizar el resto de la aplicación de la 
		biblioteca OIS.
		*/
		enum TButton
		{
			UNASSIGNED = 0xFFFFFFFF,
			LEFT       = OIS::MB_Left,
			RIGHT      = OIS::MB_Right,
			MIDDLE     = OIS::MB_Middle,
			BUTTON_3   = OIS::MB_Button3,
			BUTTON_4   = OIS::MB_Button4,
			BUTTON_5   = OIS::MB_Button5,
			BUTTON_6   = OIS::MB_Button6,
			BUTTON_7   = OIS::MB_Button7

		}; // enum TButton

	} // namespace Button

	/**
	Tipo copia para los códigos de los botones del ratón. Por simplicidad.
	*/
	typedef Button::TButton TButton;

	/**
	Namespace que contiene el tipo que define los diferentes códigos de 
	las teclas. Declararlos aquí tiene como función independizar el resto 
	de la aplicación de la biblioteca OIS.
	*/
	namespace Key
	{
		/**
		Tipo para los códigos de las teclas. Declararlos aquí tiene como 
		función independizar el resto de la aplicación de la biblioteca 
		OIS.
		*/
		enum TKeyID
		{
			UNASSIGNED    = OIS::KC_UNASSIGNED,
			ESCAPE        = OIS::KC_ESCAPE,
			NUMBER1       = OIS::KC_1,
			NUMBER2       = OIS::KC_2,
			NUMBER3       = OIS::KC_3,
			NUMBER4       = OIS::KC_4,
			NUMBER5       = OIS::KC_5,
			NUMBER6       = OIS::KC_6,
			NUMBER7       = OIS::KC_7,
			NUMBER8       = OIS::KC_8,
			NUMBER9       = OIS::KC_9,
			NUMBER0       = OIS::KC_0,
			MINUS         = OIS::KC_MINUS,    // - on main keyboard
			EQUALS        = OIS::KC_EQUALS,
			BACK          = OIS::KC_BACK,    // backspace
			TAB           = OIS::KC_TAB,
			Q             = OIS::KC_Q,
			W             = OIS::KC_W,
			E             = OIS::KC_E,
			R             = OIS::KC_R,
			T             = OIS::KC_T,
			Y             = OIS::KC_Y,
			U             = OIS::KC_U,
			I             = OIS::KC_I,
			O             = OIS::KC_O,
			P             = OIS::KC_P,
			LBRACKET      = OIS::KC_LBRACKET,
			RBRACKET      = OIS::KC_RBRACKET,
			RETURN        = OIS::KC_RETURN,    // Enter on main keyboard
			LCONTROL      = OIS::KC_LCONTROL,
			A             = OIS::KC_A,
			S             = OIS::KC_S,
			D             = OIS::KC_D,
			F             = OIS::KC_F,
			G             = OIS::KC_G,
			H             = OIS::KC_H,
			J             = OIS::KC_J,
			K             = OIS::KC_K,
			L             = OIS::KC_L,
			NGRAVE        = OIS::KC_SEMICOLON,    // Ñ
			APOSTROPHE    = OIS::KC_APOSTROPHE,
			GRAVE         = OIS::KC_GRAVE,    // accent
			LSHIFT        = OIS::KC_LSHIFT,
			BACKSLASH     = OIS::KC_BACKSLASH,
			Z             = OIS::KC_Z,
			X             = OIS::KC_X,
			C             = OIS::KC_C,
			V             = OIS::KC_V,
			B             = OIS::KC_B,
			N             = OIS::KC_N,
			M             = OIS::KC_M,
			COMMA         = OIS::KC_COMMA,
			PERIOD        = OIS::KC_PERIOD,    // . on main keyboard
			SLASH         = OIS::KC_SLASH,    // / on main keyboard
			RSHIFT        = OIS::KC_RSHIFT,
			MULTIPLY      = OIS::KC_MULTIPLY,    // * on numeric keypad
			LALT          = OIS::KC_LMENU,    // left Alt
			SPACE         = OIS::KC_SPACE,
			CAPITAL       = OIS::KC_CAPITAL,
			F1            = OIS::KC_F1,
			F2            = OIS::KC_F2,
			F3            = OIS::KC_F3,
			F4            = OIS::KC_F4,
			F5            = OIS::KC_F5,
			F6            = OIS::KC_F6,
			F7            = OIS::KC_F7,
			F8            = OIS::KC_F8,
			F9            = OIS::KC_F9,
			F10           = OIS::KC_F10,
			NUMLOCK       = OIS::KC_NUMLOCK,
			SCROLL        = OIS::KC_SCROLL,    // Scroll Lock
			NUMPAD7       = OIS::KC_NUMPAD7,
			NUMPAD8       = OIS::KC_NUMPAD8,
			NUMPAD9       = OIS::KC_NUMPAD9,
			SUBTRACT      = OIS::KC_SUBTRACT,    // - on numeric keypad
			NUMPAD4       = OIS::KC_NUMPAD4,
			NUMPAD5       = OIS::KC_NUMPAD5,
			NUMPAD6       = OIS::KC_NUMPAD6,
			ADD           = OIS::KC_ADD,    // + on numeric keypad
			NUMPAD1       = OIS::KC_NUMPAD1,
			NUMPAD2       = OIS::KC_NUMPAD2,
			NUMPAD3       = OIS::KC_NUMPAD3,
			NUMPAD0       = OIS::KC_NUMPAD0,
			DECIMAL       = OIS::KC_DECIMAL,    // . on numeric keypad
			OEM_102       = OIS::KC_OEM_102,    // < > | on UK/Germany keyboards
			F11           = OIS::KC_F11,
			F12           = OIS::KC_F12,
			NUMPADEQUALS  = OIS::KC_NUMPADEQUALS,    // = on numeric keypad (NEC PC98)
			PREVTRACK     = OIS::KC_PREVTRACK,    // Previous Track (OIS::KC_CIRCUMFLEX on Japanese keyboard)
			AT            = OIS::KC_AT,    //                     (NEC PC98)
			COLON         = OIS::KC_COLON,    //                     (NEC PC98)
			UNDERLINE     = OIS::KC_UNDERLINE,    //                     (NEC PC98)
			STOP          = OIS::KC_STOP,    //                     (NEC PC98)
			NEXTTRACK     = OIS::KC_NEXTTRACK,    // Next Track
			NUMPADENTER   = OIS::KC_NUMPADENTER,    // Enter on numeric keypad
			RCONTROL      = OIS::KC_RCONTROL,
			MUTE          = OIS::KC_MUTE,    // Mute
			CALCULATOR    = OIS::KC_CALCULATOR,    // Calculator
			PLAYPAUSE     = OIS::KC_PLAYPAUSE,    // Play / Pause
			MEDIASTOP     = OIS::KC_MEDIASTOP,    // Media Stop
			VOLUMEDOWN    = OIS::KC_VOLUMEDOWN,    // Volume -
			VOLUMEUP      = OIS::KC_VOLUMEUP,    // Volume +
			WEBHOME       = OIS::KC_WEBHOME,    // Web home
			NUMPADCOMMA   = OIS::KC_NUMPADCOMMA,    // , on numeric keypad (NEC PC98)
			DIVIDE        = OIS::KC_DIVIDE,    // / on numeric keypad
			SYSRQ         = OIS::KC_SYSRQ,
			RALT          = OIS::KC_RMENU,    // right Alt
			PAUSE         = OIS::KC_PAUSE,    // Pause
			HOME          = OIS::KC_HOME,    // Home on arrow keypad
			UPARROW       = OIS::KC_UP,    // UpArrow on arrow keypad
			PGUP          = OIS::KC_PGUP,    // PgUp on arrow keypad
			LEFTARROW     = OIS::KC_LEFT,    // LeftArrow on arrow keypad
			RIGHTARROW    = OIS::KC_RIGHT,    // RightArrow on arrow keypad
			END           = OIS::KC_END,    // End on arrow keypad
			DOWNARROW     = OIS::KC_DOWN,    // DownArrow on arrow keypad
			PGDOWN        = OIS::KC_PGDOWN,    // PgDn on arrow keypad
			INSERT        = OIS::KC_INSERT,    // Insert on arrow keypad
			DEL           = OIS::KC_DELETE,    // Delete on arrow keypad
			LWIN          = OIS::KC_LWIN,    // Left Windows key
			RWIN          = OIS::KC_RWIN,    // Right Windows key
			APPS          = OIS::KC_APPS,    // AppMenu key
			POWER         = OIS::KC_POWER,    // System Power
			SLEEP         = OIS::KC_SLEEP,    // System Sleep
			WAKE          = OIS::KC_WAKE,    // System Wake
			WEBSEARCH     = OIS::KC_WEBSEARCH,    // Web Search
			WEBFAVORITES  = OIS::KC_WEBFAVORITES,    // Web Favorites
			WEBREFRESH    = OIS::KC_WEBREFRESH,    // Web Refresh
			WEBSTOP       = OIS::KC_WEBSTOP,    // Web Stop
			WEBFORWARD    = OIS::KC_WEBFORWARD,    // Web Forward
			WEBBACK       = OIS::KC_WEBBACK,    // Web Back
			MYCOMPUTER    = OIS::KC_MYCOMPUTER,    // My Computer
			MAIL          = OIS::KC_MAIL,    // Mail
			MEDIASELECT   = OIS::KC_MEDIASELECT     // Media Select

		}; // enum TKeyID

	} // namespace Key

	/**
	Tipo copia para los códigos de las teclas del teclado. Por 
	simplicidad.
	*/
	typedef struct TKey
	{
		TKey(const unsigned int text, const Key::TKeyID keyId)
						{this->text = text; this->keyId = keyId;}
		unsigned int text;
		Key::TKeyID keyId;
	}TKey;

	/**
	Namespace que contiene el tipo que define los diferentes códigos de 
	las teclas "modificadoras" Shift, Ctrl y Alt. Declararlos aquí tiene 
	como función independizar el resto de la aplicación de la biblioteca 
	OIS.
	*/
	namespace Modifier
	{
		/**
		Tipo para los códigos de las teclas "modificadoras" Shift, Ctrl y 
		Alt. Declararlos aquí tiene como función independizar el resto de 
		la aplicación de la biblioteca OIS.
		*/
		enum TModifier
		{
			SHIFT  = OIS::Keyboard::Shift,
			CTRL   = OIS::Keyboard::Ctrl,
			ALT    = OIS::Keyboard::Alt

		}; // enum TModifier

	} // namespace Modifier

	/**
	Tipo copia para los códigos de las teclas "modificadoras" Shift, Ctrl 
	y Alt. Por simplicidad.
	*/
	typedef Modifier::TModifier TModifier;
		
	/**
	Esta clase debe ser implementada por las clases que quieren
	registrarse en el gestor de periféricos de entrada para ser
	avisados de eventos del teclado.
	Las funciones retornan cierto si el evento ha sido procesado.
	En este caso el gestor no llamará a otros listeners. Por 
	defecto los métodos devuelven false.
	
	@ingroup GUIGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CKeyboardListener 
	{
	public:

		/**
		Método que será invocado siempre que se pulse una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. Por defecto los 
		métodos devuelven false.
		*/
		virtual bool keyPressed(TKey key) {return false;}
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. Por defecto los 
		métodos devuelven false.
		*/
		virtual bool keyReleased(TKey key) {return false;}

	}; // CKeyboardListener
	
	/**
	clase que representa el estado del ratón. Sirve para aunar 
	varios atributos simples que definen el estado. Los atributos
	son públicos para que sean más fáciles de acceder.
	
	@ingroup GUIGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CMouseState
	{
	public:

		/**
		Constructor. Por defecto se le dan unos valores a los 
		atributos, pero estos deben de ser cambiados en función
		de las propiedades de la ventana, posición del ratón,
		botón pulsado, etc.
		*/
		CMouseState()
		{
			width = 800;
			height = 600;
			posAbsX = 0;
			posAbsY = 0;
			posRelX = 0;
			posRelY = 0;
			movX = 0;
			movY = 0;
			scrool = 0;
			button =  Button::UNASSIGNED;
		}

		/**
		Constructor parametrizado.
		
		@param width Anchura de la ventana en pixels.
		@param height Altura de la ventana en pixels.
		@param posAbsX Posición absoluta del eje X del puntero del ratón.
		Va desde 0 hasta el ancho de la ventana.
		@param posAbsY Posición absoluta del eje Y del puntero del ratón.
		Va desde 0 hasta el alto de la ventana.
		*/
		CMouseState(unsigned int width, unsigned int height, 
					unsigned int posAbsX = 0, unsigned int posAbsY = 0)
		{
			setExtents(width,height);
			setPosition(posAbsX,posAbsY);
			movX = 0;
			movY = 0;
			scrool = 0;
			button = (TButton)-1; // Button::UNASSIGNED
		}

		/**
		Establece la anchura y altura del área de 
		trabajo del ratón.

		@param width Anchura de la ventana en pixels.
		@param height Altura de la ventana en pixels.
		*/
		void setExtents(unsigned int width, unsigned int height)
		{
			this->width = width;
			this->height = height;
		}

		/**
		Establece las posiciones de los ejes X e Y del puntero del 
		ratón.
		
		@param posAbsX Posición absoluta del eje X del puntero del ratón.
		Va desde 0 hasta el ancho de la ventana.
		@param posAbsY Posición absoluta del eje Y del puntero del ratón.
		Va desde 0 hasta el alto de la ventana.
		*/
		void setPosition(unsigned int posAbsX, unsigned int posAbsY)
		{
			this->posAbsX = posAbsX;
			this->posAbsY = posAbsY;
			this->posRelX = (float)posAbsX / (float)width;
			this->posRelY = (float)posAbsY / (float)height;
		}

		/** 
		Estos valores representan la anchura y altura del área de 
		trabajo del ratón. Suele estar asociado al tamaño de la 
		ventana de renderizado. Su valor es mutable para que se
		pueda cambiar aunque se pase como const.
		*/
		unsigned int width, height;

		/**
		Posiciones absolutas de los ejes X e Y del puntero del 
		ratón. Los valores van desde 0 hasta el ancho y alto de 
		la ventana.
		*/
		unsigned int posAbsX, posAbsY;

		/**
		Posiciones relativas de los ejes X e Y del puntero del 
		ratón. Los valores van desde 0 hasta 1 independientemente
		del ancho y alto de la ventana.
		*/
		float posRelX, posRelY;

		/**
		Movimiento de los ejes X e Y del puntero del ratón respecto 
		del último evento. Valores positivos movimiento a derechas,
		negativos a izquierdas.
		*/
		int movX, movY;
		
		/**
		Movimiento de la rueda del ratón respecto del último evento.
		*/
		int scrool;

		/**
		Indica que botón ha sido el último en producir un evento.
		Si no ha habido eventos de ratón su valor es 
		Button::UNASSIGNED.
		*/
		TButton button;
	};
	
	/**
	Esta clase debe ser implementada por las clases que quieren
	registrarse en el gestor de periféricos de entrada para ser
	avisados de eventos del ratón.
	Las funciones retornan cierto si el evento ha sido procesado.
	En este caso el gestor no llamará a otros listeners. Por 
	defecto los métodos devuelven false.
	
	@ingroup GUIGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CMouseListener 
	{
	public:
		
		/**
		Método que será invocado siempre que se mueva el ratón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. Por defecto los 
		métodos devuelven false.
		*/
		virtual bool mouseMoved(const CMouseState &mouseState)
														{return false;}
		
		/**
		Método que será invocado siempre que se pulse un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. Por defecto los 
		métodos devuelven false.
		*/
		virtual bool mousePressed(const CMouseState &mouseState)
														{return false;}

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. Por defecto los 
		métodos devuelven false.
		*/
		virtual bool mouseReleased(const CMouseState &mouseState)
														{return false;}

	}; // CMouseListener

	/**
	Gestor de periféricos de entrada. Controla y captura los eventos
	que se producen en teclado y ratón. Permite registrar oyentes que
	serán informados de dichos eventos. Cuando se registra un evento
	se emite este a los diferentes oyentes hasta que uno de los
	oyentes capture dicho evento. Los nuevos oyentes tienen prioridad
	a la hora de ser avisados de los eventos. Esto viene bien por si
	en condiciones especiales se quiere añadir un oyente que cambia
	las acciones de ciertas teclas sin necesidad de eliminar oyentes.
	<p>
	Para la gestión de los eventos se usa OIS. Sin embargo esta clase
	redefine las clases oyentes de teclado y ratón, así como los 
	códigos de las teclas del teclado y los botones y el estado del 
	ratón para que el resto de las clases de la aplicación sean 
	independientes del sistema usado para la captura de eventos (OIS).
	<p>
	La inicialización del subsistema OIS NO se hace en esta clase
	sino que lo hace BaseSubsystems::CServer. De la misma manera esta
	clase no es responsable de su destrucción.
	<p>
	Por simplicidad no se han añadido otros periféricos de entrada 
	como joysticks, aunque OIS los permite.
	
	@ingroup GUIGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CInputManager : public OIS::KeyListener, 
		public OIS::MouseListener 
	{
	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al gestor de periféricos de entrada.
		*/
		static GUI::CInputManager *getSingletonPtr() { return _instance; }

		/** 
		Inicia el gestor de periféricos de entrada basado en OIS.

		@return true si la inicialización se hizo correctamente.
		*/
		static bool Init();

		/** 
		Libera el gestor de periféricos de entrada basado en OIS. 
		Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/** 
		Comprueba si ha habido eventos en los dispositivos de entrada.
		Debe llamarse en cada vuelta de la aplicación.
		*/
		void tick();

		/** 
		Añade un oyente del teclado.
		
		@param keyListener Oyente del teclado
		*/
		void addKeyListener(CKeyboardListener *keyListener);

		/** 
		Añade un oyente del ratón.
		
		@param keyListener Oyente del ratón.
		*/
		void addMouseListener(CMouseListener *mouseListener);

		/** 
		Borra un oyente del teclado.
		
		@param keyListener Oyente del teclado
		*/
		void removeKeyListener(CKeyboardListener *keyListener);

		/** 
		Borra un oyente del ratón.
		
		@param keyListener Oyente del ratón.
		*/
		void removeMouseListener(CMouseListener *mouseListener);

		/** 
		Borra todos los oyentes.
		*/
		void removeAllListeners();

		/** 
		Borra todos los oyentes del teclado.
		*/
		void removeAllKeyListeners();

		/** 
		Borra todos los oyentes del ratón.
		*/
		void removeAllMouseListeners();

		/**
		Método que consulta si una tecla está o no pulsada.

		@param key Tecla que se quiere saber si está o no pulsada.
		@return true si la tecla está pulsada.
		*/
		bool isKeyPressed(TKey key) 
		{return _keyboard->isKeyDown((OIS::KeyCode)key.keyId);}

		/**
		Método que consulta si una tecla "modificadora" está o no pulsada.

		@param key Tecla que se quiere saber si está o no pulsada.
		@return true si la tecla está pulsada.
		*/
		bool isModifierPressed(TModifier modifier) 
			{return _keyboard->isModifierDown((OIS::Keyboard::Modifier)modifier);}

	private:

		/**
		Constructor.
		*/
		CInputManager();

		/**
		Destructor.
		*/
		virtual ~CInputManager();

		/** 
		Segunda parte de la inicialización del gestor de periféricos de entrada 
		basado en OIS. Sirve para hacer inicializaciones de la propia instancia 
		en vez de inicializaciones estáticas.

		@return true si la inicialización se hizo correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();

		/** 
		Única instancia de la clase. 
		*/
		static CInputManager *_instance;

		/** 
		Método invocado por OIS cuando se pulsa una tecla. Es el
		encargado de avisar a todos los oyentes del evento.

		@param e Evento producido.
		@return true si se captura el evento.
		*/
		bool keyPressed(const OIS::KeyEvent &e);
		
		/** 
		Método invocado por OIS cuando se deja de pulsar una tecla. 
		Es el encargado de avisar a todos los oyentes del evento.

		@param e Evento producido.
		@return true si se captura el evento.
		*/
		bool keyReleased(const OIS::KeyEvent &e);
		
		/**
		Método que convierte un evento de teclado OIS a uno propio.

		@param e Evento de teclado de OIS.
		@return Evento de teclado propio.
		*/
		TKey ois2galeon(const OIS::KeyEvent &e);

		/** 
		Método invocado por OIS cuando se mueve el ratón. 
		Es el encargado de avisar a todos los oyentes del evento.

		@param e Evento producido.
		@return true si se captura el evento.
		*/
		bool mouseMoved(const OIS::MouseEvent &e);
		
		/** 
		Método invocado por OIS cuando se pulsa un botón del ratón. 
		Es el encargado de avisar a todos los oyentes del evento.

		@param e Evento producido.
		@param button código del botón pulsado.
		@return true si se captura el evento.
		*/
		bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID button);

		/** 
		Método invocado por OIS cuando se deja de pulsar un botón. 
		Es el encargado de avisar a todos los oyentes del evento.

		@param e Evento producido.
		@param button código del botón soltado.
		@return true si se captura el evento.
		*/
		bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID button);

		/** 
		Buffer de la entrada del ratón OIS.
		*/
		OIS::Mouse *_mouse;

		/** 
		Buffer de la entrada del teclado OIS.
		*/
		OIS::Keyboard *_keyboard;
		
		/**
		Estado del ratón en el último evento. Se usa para transmitir
		los cambios a las clases oyentes. Sirve para independizar el
		resto de la aplicación de OIS.
		*/
		CMouseState _mouseState;

		/**
		Sistema de gestión de periféricos de entrada de OIS.
		*/
		OIS::InputManager *_inputSystem;

		/**
		Lista de oyentes de eventos del teclado.
		*/
		std::list<CKeyboardListener*> _keyListeners;

		/**
		Lista de oyentes de eventos del ratón.
		*/
		std::list<CMouseListener*> _mouseListeners;

	}; // class InputManager

} // namespace GUI

#endif // __GUI_