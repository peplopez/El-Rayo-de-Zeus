/**
@file Server.h

Contiene la declaración de la clase CServer, Singleton que se encarga de
la gestión de los scripts del juego.

@see ScriptManager::CServer

@author Pablo Terrado
@date Marzo, 2013
*/

#ifndef __ScriptManager_Server_H
#define __ScriptManager_Server_H

#include <list>
#include <string>

// Estructura con el contexto (estado) del intérprete de Lua.
struct lua_State;

//typedef int (*lua_CFunction)(lua_State*);

/**
Namespace para la gestión de los scripts del juego. Se encarga
de la carga y lectura de los mismos.

@author Pablo Terrado
@date Marzo, 2013
*/
namespace ScriptManager 
{

	/**
	Clase servidor del ScriptManager. Se encarga de la gestión de los scripts; la carga y lectura de los
	mismos. Será la clase encargada de inicializar luabind y de establecer la conexión entre lua
	y el código C++.

	@author Pablo Terrado
	@date Marzo, 2013
	*/
	class CServer 
	{

	public:
		/**
		Devuelve la referencia a la única instancia de la clase CServer.
		
		@return Referencia a la Única instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}

		//static CServer &getSingleton() { return *_instance; }

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();


		/**
		Borra la lista de scripts cargados previamente, haciendo una simulacion
		de reseteo del estado de Lua.
		*/
		void ResetCurrentState();

		/**
		Carga un fichero de script en lua y lo ejecuta. Lo guarda
		en la lista de scripts cargados para no cargarlo una segunda vez
		y para poder recargarlo "en caliente".

		@param script nombre del fichero a cargar y ejecutar.
		@return true si todo fue bien y false en caso contrario.

		@note Hay que tener en cuenta que aunque la ejecución no haya ido
		bien, si la carga se ha realizado con éxito, el script queda guardado,
		por lo que para volver a probarlo no haría falta volver a cargarlo, sino
		llamar a la función de recarga.
		*/
		bool loadExeScript(const char *script);


		/**
		Ejecuta una línea de texto como script de lua. La línea de
		texto puede ser una sentencia de lua o una función de un
		fichero anteriormente cargado.

		@param luaCode: línea de texto a ejecutar.
		@return true si todo fue bien y false en caso contrario.
		*/
		bool executeScript(const char *luaCode);

		/**
		Recarga y ejecuta todos los ficheros de script almacenados. Por
		cada uno muestra un mensaje indicando que se ha cargado y ejecutado
		correctamente o un mensaje de error en caso de que algo haya ido mal.
		*/
		void reloadScripts();


		/**
		Obtiene el valor de una variable global en Lua
		de tipo numérico entero a partir de su nombre.
		 
		@param name Nombre de la variable global que
		se quiere obtener.
		@param defaultValue Valor que se devolverá 
		si no existe esa variable global en Lua.
		 
		@return Valor de la variable global de Lua, 
		o <tt>defaultValue</tt> si no existía.
		 
		@note En Lua el tipo numérico es en principio
		<tt>double</tt>. Aquí sin embargo devolvemos
		un entero.
		*/
		int getGlobal(const char *name, int defaultValue);

		/**
		Obtiene el valor de una variable global en Lua
		de tipo bool a partir de su nombre.
		 
		@param name Nombre de la variable global que
		se quiere obtener.
		@param error Variable booleana que indica si
		ha habido un error accediendo a la variable. Es true
		si ha habido algún error y false en caso contrario.
		 
		@return Valor de la variable global de Lua, 
		o false si no existía.
		*/
		bool getGlobal(const char *name, bool &error);

		/**
		Obtiene el valor de una variable global en Lua
		de tipo cadena a partir de su nombre.
	 
		@param name Nombre de la variable global que
		se quiere obtener.
		@param defaultValue Valor que se devolverá 
		si no existe esa variable global en Lua.
	 
		@return Valor de la variable global de Lua, 
		o <tt>defaultValue</tt> si no existía.
	 
		@note El invocante asume la posesión del puntero
		devuelto, que tendrá que ser liberado con
		<tt>free</tt>. Esta liberación es necesaria
		<em>incluso aunque se haya devuelto
		<tt>defaultValue</tt></em> (se habrá hecho una
		copia).
	 
		@note Esta función tiene un peligro potencial;
		Lua soporta cadenas con <tt>'\\0'</tt> en su
		interior. Aquí <em>asumiremos que no lo tienen</em>
		porque usamos el <tt>'\\0'</tt> como marca de
		fin de cadena por simplicidad.
		*/
		const char *getGlobal(const char *name, const char *defaultValue);

		/**
		Establece el valor de una variable global en lua. Si ya 
		existe esa variable se sobreescribe.

		@param name Nombre de la variable.
		@param value Valor de la variable.
		*/
		template <class T>
		void setGlobal(const char *name, const T& value);

		/**
		Obtiene el valor de un campo de tipo numérico de
		una tabla global.
	 
		@param table Nombre de la tabla (global) a la que
		se quiere acceder.
		@param field Nombre del campo (índice) cuyo valor se
		desea obtener.
		@param defaultValue Valor que se devolverá 
		si no existe la tabla o ese campo (índice) no
		es de tipo numérico (o no hay una conversión posible).
	 
		@return Valor del campo solicitado, 
		o <tt>defaultValue</tt> si no existía.
	 
		@note En Lua el tipo numérico es en principio
		<tt>double</tt>. Aquí sin embargo devolvemos
		un entero.
		*/
		int getField(const char *table, const char *field, int defaultValue);

		/**
		Obtiene el valor de un campo de tipo booleano de
		una tabla global.
	 
		@param table Nombre de la tabla (global) a la que
		se quiere acceder.
		@param field Nombre del campo (índice) cuyo valor se
		desea obtener.
		@param error Variable booleana que indica si
		ha habido un error accediendo a la variable de la tabla. Es true
		si ha habido algún error y false en caso contrario.
	 
		@return Valor del campo solicitado, 
		o false si no existía.
		*/
		bool getField(const char *table, const char *field, bool &error);

		/**
		Obtiene el valor de un campo de tipo cadena de
		una tabla global.
	 
		@param table Nombre de la tabla (global) a la que
		se quiere acceder.
		@param field Nombre del campo (índice) cuyo valor se
		desea obtener.
		@param defaultValue Valor que se devolverá 
		si no existe la tabla o ese campo (índice) no
		es de tipo cadena (o no hay una conversión posible).
	 
		@return Valor del campo solicitado, 
		o <tt>defaultValue</tt> si no existía.
	 
		@note El invocante asume la posesión del puntero
		devuelto, que tendrá que ser liberado con
		<tt>free</tt>. Esta liberación es necesaria
		<em>incluso aunque se haya devuelto
		<tt>defaultValue</tt></em> (se habrá hecho una
		copia).
	 
		@note Esta función tiene un peligro potencial;
		Lua soporta cadenas con <tt>'\\0'</tt> en su
		interior. Aquí <em>asumiremos que no lo tienen</em>
		porque usamos el <tt>'\\0'</tt> como marca de
		fin de cadena por simplicidad.
		*/
		const char* getField(const char *table, const char *field, const char *defaultValue);

		/**
		Ejecuta un procedimiento de lua sin parámetros y sin
		valores de vuelta.

		@param subroutineName Nombre del procedimiento.

		@return true si la ejecución del procedimiento fue bien
		y false en caso contrario.
		*/
		bool executeProcedure(const char *name);

		/**
		Método templatizado que ejecuta un procedimiento de 
		lua con un parámetro de entrada y sin valor de vuelta

		@param subroutineName Nombre del procedimiento.
		@param param1 parámetro del procedimiento.

		@return true si la ejecución del procedimiento fue bien
		y false en caso contrario.

		@note Esta es una plantilla definida en el fichero .cpp
		de la clase, por tanto la definición de la misma no puede
		quedarse como genérica ya que el compilador no lo acepta.
		Para instanciar la plantilla y que pueda compilar pero que
		al mismo tiempo la definición del método quede lo suficientemente
		genérica, se han definido instancias concretas al final del
		fichero .cpp. Para cualquier instancia nueva que se necesite
		de la plantilla basta con definir una nueva línea con el
		tipo deseado en la sección concreta del fichero .cpp.
		*/
		template <class T>
		bool executeProcedure(const char *name, const T& param1);

		/**
		Método templatizado que ejecuta un procedimiento de 
		lua con 2 parámetros de entrada y sin valor de vuelta

		@param subroutineName Nombre del procedimiento.
		@param param1 parámetro1 del procedimiento.
		@param param2 parámetro2 del procedimiento.

		@return true si la ejecución del procedimiento fue bien
		y false en caso contrario.

		@note Esta es una plantilla definida en el fichero .cpp
		de la clase, por tanto la definición de la misma no puede
		quedarse como genérica ya que el compilador no lo acepta.
		Para instanciar la plantilla y que pueda compilar pero que
		al mismo tiempo la definición del método quede lo suficientemente
		genérica, se han definido instancias concretas al final del
		fichero .cpp. Para cualquier instancia nueva que se necesite
		de la plantilla basta con definir una nueva línea con el
		tipo deseado en la sección concreta del fichero .cpp.
		*/
		template <class T>
		bool executeProcedure(const char *name, const T& param1, const T& param2);

		/**
		Método templatizado que ejecuta una función de lua 
		con un parámetro de entrada y con un valor de vuelta 
		de tipo entero.

		@param subroutineName Nombre de la función.
		@param param1 parámetro de la función.
		@param result valor devuelto por la función de lua.

		@return true si la ejecución de la función fue bien
		y false en caso contrario.

		@note Esta es una plantilla definida en el fichero .cpp
		de la clase, por tanto la definición de la misma no puede
		quedarse como genérica ya que el compilador no lo acepta.
		Para instanciar la plantilla y que pueda compilar pero que
		al mismo tiempo la definición del método quede lo suficientemente
		genérica, se han definido instancias concretas al final del
		fichero .cpp. Para cualquier instancia nueva que se necesite
		de la plantilla basta con definir una nueva línea con el
		tipo deseado en la sección concreta del fichero .cpp.
		*/
		template <class T>
		bool executeFunction(const char *name, const T& param1, int &result);

		/**
		Método templatizado que registra una función de C++ en el 
		contexto de lua. La función a registrar no debe ser miembro 
		de ninguna clase ni estar incluida en ningún espacio de nombres.

		@param name Nombre que tendrá la función en el contexto de lua.
		@param f Puntero a la función de C++.


		@note Esta es una plantilla definida en el fichero .cpp
		de la clase, por tanto la definición de la misma no puede
		quedarse como genérica ya que el compilador no lo acepta.
		Para instanciar la plantilla y que pueda compilar pero que
		al mismo tiempo la definición del método quede lo suficientemente
		genérica, se han definido instancias concretas al final del
		fichero .cpp. Para cualquier instancia nueva que se necesite
		de la plantilla basta con definir una nueva línea con el
		tipo deseado en la sección concreta del fichero .cpp.
		*/
		template <class F>
		void registerFunction(const char *name, F f);

		/**
		Método que crea e inicia una corrutina de lua con el nombre y la función
		pasadas como parámetro.

		@param corutineName Nombre con el que se va a crear la corrutina.
		@param corutineFunction Nombre de la función a partir de la cual se va a
		crear la corrutina.
		@param resultName Nombre de la variable global de lua donde se va a guardar 
		el resultado devuelto por el primer yield del script.

		@return true si la corrutina se ha ejecutado y pausado con éxito o no.
		*/
		bool initCorutine(const char *corutineName, const char *corutineFunction, const char *resultName);

		/**
		Método que continua una corrutina pausada pasándole un parámetro en formato de cadena de caracteres.

		@param corutineName Nombre de la corutina que hay que despertar.
		@param resultName Nombre de la variable global de lua donde se va a guardar
		el resultado devuelto por el siguiente yield del script.
		@param param Parámetro a pasarle al yield del script actual. Independientemente del formato
		de la variable hay que pasarlo en formato texto de forma que lo entienda lua.

		@return true si la corrutina se ha ejecutado y pausado con éxito hasta el siguiente
		yield o no.
		*/
		bool resumeCorutine(const char *corutineName, const char *resultName, const char *param);

		/**
		Devuelve el estado de lua, que es la variable representativa
		de lua en C++.

		@return El estado de lua.
		*/
		lua_State* getLuaState() {return _lua;}

	protected:

		/**
		Constructor.
		*/
		CServer();

		/**
		Destructor.
		*/
		~CServer();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();


	private:

		/**
		Carga un fichero de script haciendo todo el manejo de errores y
		mostrando por consola los mensajes pertinentes.

		@param script nombre del script a cargar.
		@param inmediate true si queremos cargar y ejecutar el script en modo inmediato y false en caso contrario.
		@return true si la carga fue bien y false en caso contrario.

		@note Esta función no ejecuta el script, solo lo carga y lo deja preparado
		para su ejecución. Tampoco hace la gestión de qué scripts se han cargado ya
		o guardar los nuevos.
		*/
		bool loadScript(const char *script, bool inmediate);

		/**
		Ejecuta el último script cargado y hace el manejo de errores,
		mostrando por consola los mensajes pertinentes.

		@param script nombre del último script cargado (se utiliza para mostrar los mensajes de error).
		@return true si la ejecución fue bien y false en caso contrario.
		*/
		bool executeLastLoadScript(const char *script);

		/**
		Muestra un mensaje por consola.

		@param message Mensaje a mostrar.
		*/
		void showMessage(std::string &message);

		/**
		Muestra un mensaje de error por consola y salta un assert.

		@param message Mensaje de error a mostrar.
		*/
		void showErrorMessage(std::string &message);
		
		/**
		Registra los objetos C++ que vas a ser necesarios en LUA
		*/
		void registerClasses();

		/**
		Única instancia de la clase.
		*/
		static CServer* _instance;

		/**
		Atributo que representa la comunicación con lua desde el código C++.
		*/
		lua_State *_lua;

		/**
		Tipo lista de scripts cargados en el manager de scripts.
		*/
		typedef std::list<const char*> TScriptList;

		/**
		Lista de scripts cargados.
		*/
		TScriptList _scriptList;


	}; //fin Class CServer

} // namespace ScriptManager

#endif // __ScriptManager_Server_H

