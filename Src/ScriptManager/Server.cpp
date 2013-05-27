/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de los scripts del juego.

@see ScriptManager::CServer

@author Pablo Terrado
@date Marzo, 2013
*/

#include "Server.h"
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string> //PT

//PT
#include "ScriptManager\Classes\LUA_EntityFunctions.h"
#include "Logic\GameStatus.h"

//Como LUA esta compilado en C puro y duro. (se necesita incluirla con extern "C")
extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h> // Para inicializar la librería base de Lua
}

//LUABIND
#pragma warning( disable: 4251 )
#include <luabind/luabind.hpp> //nuevo para Luabind

using namespace std;

namespace ScriptManager {


	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer()
	{
		// Con esto evitamos que otros (Lua) no cree mas referencias de la clase.
		if (_instance == 0)
		{
			_instance = this;
			_lua = NULL;
		}
		
	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		// Con esto evitamos que otros (Lua) no destruyan nuestro singleton.
		if (_instance == this)
			_instance = 0;

	} // ~CServer

	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de ScriptManager::CServer no permitida!");

		new CServer();
		//_instance = new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "ScriptManager::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open() {

		// Obtengo el estado de lua
		//_lua  = lua_open();

		// Obtengo el estado de lua (inicialización de lua)
		_lua = luaL_newstate();

		if(!_lua)
			return false;

		// Abro las librerías de lua (segunda parte de la inicialización)
		luaL_openlibs(_lua);

		// Abrimos la librería base para hacer disponible
		// print() en Lua. (funcion que esta dentro de la libreria lualib.h)
		luaopen_base(_lua);

		//LUABIND (Activamos luabind en el intérprete)
		luabind::open(_lua);

		//Registrar las clases que se van a usar desde LUA
		registerClasses();

		return true;

	}

	void CServer::close() {
		//llamamos al lua_close para cerrar el interprete de LUA
		if(_lua)
			lua_close(_lua);

		//se limpia la lista de scripts cargados.
		_scriptList.clear();
	}

	//---------------------------------------------------------

	void CServer::ResetCurrentState()
	{
		_scriptList.clear();

	}


	//---------------------------------------------------------

	int CServer::getGlobal(const char *name, int defaultValue)
	{
		assert(_lua && "No se ha hecho la inicialización de lua");

		// Obtengo la variable global por el nombre.
		luabind::object obj = luabind::globals(_lua)[name];

		// Hago gestión de errores para asegurarme de que la variable existe y es del tipo correcto.
		if (!obj.is_valid() || (luabind::type(obj) != LUA_TNUMBER))
		{
			showErrorMessage("ERROR DE LUA! - La variable numérica \"" + std::string(name) + "\" a la que se está intentando acceder no existe en ningún script de lua o no es de tipo numérico.");

			return defaultValue;
		}

		// Devuelvo la variable haciendo casting de tipo para adecuar la variable de lua a nuestro C++.
		return luabind::object_cast<int>(obj);

	} // getGlobal(int)

	//---------------------------------------------------------

	bool CServer::getGlobal(const char *name, bool &error)
	{
		assert(_lua && "No se ha hecho la inicialización de lua");

		// Obtengo la variable global por el nombre.
		luabind::object obj = luabind::globals(_lua)[name];

		// Hago gestión de errores para asegurarme de que la variable existe y es del tipo correcto.
		if (!obj.is_valid() || (luabind::type(obj) != LUA_TBOOLEAN))
		{
			showErrorMessage("ERROR DE LUA! - La variable booleana \"" + std::string(name) + "\" a la que se está intentando acceder no existe en ningún script de lua o no es de tipo booleana.");

			error = true;
			return false;
		}

		// Devuelvo la variable haciendo casting de tipo para adecuar la variable de lua a nuestro C++. Y actualizo la variable de error.
		error = false;
		return luabind::object_cast<bool>(obj);
		
	} // getGlobal(bool)

	//---------------------------------------------------------

	const char* CServer::getGlobal(const char *name, const char *defaultValue)
	{
		assert(_lua && "No se ha hecho la inicialización de lua");

		// Obtengo la variable global por el nombre.
		luabind::object obj = luabind::globals(_lua)[name];

		// Hago gestión de errores para asegurarme de que la variable existe y es del tipo correcto.
		if (!obj.is_valid() || (luabind::type(obj) != LUA_TSTRING))
		{
			showErrorMessage("ERROR DE LUA! - La variable de tipo cadena \"" + std::string(name) + "\" a la que se está intentando acceder no existe en ningún script de lua o no es de tipo cadena.");

			return defaultValue;
		}

		// Devuelvo la variable haciendo casting de tipo para adecuar la variable de lua a nuestro C++.
		return luabind::object_cast<const char*>(obj);
		
	} // getGlobal(const char*)

	//---------------------------------------------------------

	bool CServer::loadExeScript(const char *script)
	{
		assert(_lua && "No se ha hecho la inicialización de lua");

		// Busco si el script ya lo he cargado anteriormente.
		bool exists = false;

		for (TScriptList::iterator it = _scriptList.begin(); it != _scriptList.end(); it++)
			exists = exists || (script == it._Ptr->_Myval);

		// Si ya lo he cargado muestro un mensaje de error, salgo y devuelvo false.

		if (exists)
		{
			//showErrorMessage("ERROR DE LUA! - Error al cargar el fichero de script \"" + std::string(script) + "\". Este fichero ya se ha cargado anteriormente.");
			showMessage("El fichero de script \"" + std::string(script) + "\". ya se ha cargado anteriormente.");
			return false;
		}


		_scriptList.push_back(script);

		// Cargo el script.
		if (!loadScript(script, false))
			return false;

		// Ejecuto el script.
		if (!executeLastLoadScript(script))
			return false;

		// Si he llegado aquí es que todo ha ido bien, por lo tanto me guardo el script y muestro un mensaje de confirmación.
		showMessage("Fichero \"" + std::string(script) + "\" cargado y ejecutado correctamente");

		return true;

	} // loadExeScript


	bool CServer::loadScript(const char *script, bool inmediate)
	{
		// Completo la ruta del script.
		std::stringstream filename;
		filename << "media\\scripts\\" << script << ".lua";

		// Cargo el fichero de script dependiendo del parámetro booleano.
		int errorType = 0;

		if (inmediate)
			errorType = luaL_loadstring(_lua, script);
		else
			errorType = luaL_loadfile(_lua, filename.str().c_str());

		// Error al cargar el fichero de script
		if (errorType == LUA_ERRFILE)
		{
			showErrorMessage("ERROR DE LUA! - Error al cargar el fichero de script \"" + std::string(script) + "\". Comprueba que está bien escrito el nombre y que el fichero existe.");

			return false;
		}
		// Error de sintaxis de lua
		else if (errorType == LUA_ERRSYNTAX)
		{
			showErrorMessage("ERROR DE LUA! - Error de sintaxis de lua al cargar el script \"" + std::string(script) + "\". Comprueba que no hay errores en el fichero de script.");

			return false;
		}
		// Memory allocation error
		else if (errorType == LUA_ERRMEM)
		{
			showErrorMessage("ERROR DE LUA! - Error chungo de lua al cargar el script \"" + std::string(script) + "\" : Memory allocation error.");

			return false;
		}

		return true;
		
	} // loadScript


	//---------------------------------------------------------

	bool CServer::executeLastLoadScript(const char *script)
	{
		// Ejecuto el script y hago el manejo de errores.
		int msgHandler = 0;
		int errorType = lua_pcall(_lua, 0, 0, msgHandler);

		// Si es un runtime error, accedo al tipo de error guardado en la pila y lo muestro por consola
		if (errorType == LUA_ERRRUN)
		{
			// Si el handler es distinto de cero, el mensaje de error se encuentra en la pila. Lo recupero y lo muestro.
			if (msgHandler != 0)
			{
				const char* error = lua_tostring(_lua, msgHandler);
				showErrorMessage("ERROR DE LUA! - Error de runtime de lua al ejecutar el script \"" + std::string(script) + "\": " + std::string(error));
			}
			else
				// En teoría esta situación no debería darse nunca, pero no está de más hacer la comprobación.
				showErrorMessage("ERROR DE LUA! - Error desconocido de runtime de lua al ejecutar el script \"" + std::string(script) + "\"");

			return false;
		}
		// Memory allocation error
		else if (errorType == LUA_ERRMEM)
		{
			showErrorMessage("ERROR DE LUA! - Error chungo de lua al ejecutar el script \"" + std::string(script) + "\" : Memory allocation error.");

			return false;
		}
		// Error ejecutando el message handler
		else if (errorType == LUA_ERRERR)
		{
			showErrorMessage("ERROR DE LUA! - Error chungo de lua al ejecutar el script \"" + std::string(script) + "\" : Error ejecutando el message handler.");

			return false;
		}

		return true;

	} // executeLastLoadScript


	bool CServer::executeScript(const char *luaCode) {

		assert(_lua && "No se ha hecho la inicialización de lua");

		// Cargo el script en modo inmediato.
		if (!loadScript(luaCode, true))
			return false;

		// Ejecuto el script
		if (!executeLastLoadScript(luaCode))
			return false;

		return true;
	}

	//---------------------------------------------------------

	void CServer::reloadScripts()
	{
		assert(_lua && "No se ha hecho la inicialización de lua");

		for (TScriptList::iterator it = _scriptList.begin(); it != _scriptList.end(); it++)
		{
				if (loadScript(it._Ptr->_Myval, false) && executeLastLoadScript(it._Ptr->_Myval))
					showMessage("Fichero \"" + std::string(it._Ptr->_Myval) + "\" cargado y ejecutado correctamente");
		}
	
	} // reloadScripts

	//---------------------------------------------------------

	void CServer::reloadScript(const char *script)
	{
		assert(_lua && "No se ha hecho la inicialización de lua");

				if (loadScript(script, false) && executeLastLoadScript(script))
					showMessage("Fichero \"" + std::string(script) + "\" cargado y ejecutado correctamente");
	
	} // reloadScript

	//---------------------------------------------------------

	template <class T>
	void CServer::setGlobal(const char *name, const T& value)
	{
		luabind::globals(_lua)[name] = value;

	} // setGlobal

	//---------------------------------------------------------

	int CServer::getField(const char *table, const char *field, int defaultValue){

		assert(_lua && "No se ha hecho la inicialización de lua");

		//obj contiene la tabla
		luabind::object obj = luabind::globals(_lua)[table];

		//comprobacion de que el tipo es TABLA (que es el que queremos recoger)
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		{
			showErrorMessage("ERROR DE LUA! - La tabla \"" + std::string(table) + "\" a la que se está intentando acceder no existe o no es de tipo tabla.");
			return defaultValue;
		}

		// Obtengo el campo de la tabla.
		luabind::object valor = obj[field];

		//Faltaria comprobar si valor - obj[field] -  es un entero
		if(!valor.is_valid() || (luabind::type(valor) != LUA_TNUMBER))
		{
			showErrorMessage("ERROR DE LUA! - No existe ningún campo \"" + std::string(field) + "\" en la tabla \"" + std::string(table) + "\" o no es de tipo entero.");
			return defaultValue;
		}

		// Devuelvo la variable haciendo casting de tipo para adecuar la variable de lua a nuestro C++.
		return luabind::object_cast<int>(valor);
	} // getField(int)

		//---------------------------------------------------------

	bool CServer::getField(const char *table, const char *field, bool &error){

		assert(_lua && "No se ha hecho la inicialización de lua");

		//obj contiene la tabla
		luabind::object obj = luabind::globals(_lua)[table];

		//comprobacion de que el tipo es TABLA (que es el que queremos recoger)
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		{
			showErrorMessage("ERROR DE LUA! - La tabla \"" + std::string(table) + "\" a la que se está intentando acceder no existe o no es de tipo tabla.");
			error = true;
			return false;
		}

		// Obtengo el campo de la tabla.
		luabind::object valor = obj[field];

		//Faltaria comprobar si valor - obj[field] -  es un booleano
		if(!valor.is_valid() || (luabind::type(valor) != LUA_TBOOLEAN))
		{
			showErrorMessage("ERROR DE LUA! - No existe ningún campo \"" + std::string(field) + "\" en la tabla \"" + std::string(table) + "\" o no es de tipo booleano.");
			error = true;
			return false;
		}


		// Devuelvo la variable haciendo casting de tipo para adecuar la variable de lua a nuestro C++.
		error = false;
		return luabind::object_cast<bool>(valor);
	} // getField(bool)

		//---------------------------------------------------------

	const char* CServer::getField(const char *table, const char *field, const char* defaultValue){

		assert(_lua && "No se ha hecho la inicialización de lua");

		//obj contiene la tabla
		luabind::object obj = luabind::globals(_lua)[table];

		//comprobacion de que el tipo es TABLA (que es el que queremos recoger)
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		{
			showErrorMessage("ERROR DE LUA! - La tabla \"" + std::string(table) + "\" a la que se está intentando acceder no existe o no es de tipo tabla.");
			return defaultValue;
		}

		// Obtengo el campo de la tabla.
		luabind::object valor = obj[field];

		//Faltaria comprobar si valor - obj[field] -  es una cadena
		if(!valor.is_valid() || (luabind::type(valor) != LUA_TSTRING))
		{
			showErrorMessage("ERROR DE LUA! - No existe ningún campo \"" + std::string(field) + "\" en la tabla \"" + std::string(table) + "\" o no es de tipo cadena.");
			return defaultValue;
		}

		// Devuelvo la variable haciendo casting de tipo para adecuar la variable de lua a nuestro C++.
		return luabind::object_cast<const char*>(valor);
	} // getField(const char*)

	//---------------------------------------------------------

	bool CServer::executeProcedure(const char *name){
		
		assert(_lua && "No se ha hecho la inicialización de lua");

		// Obtengo el procedimiento definido en lua
		luabind::object obj = luabind::globals(_lua)[name];

		//comprobacion de que el tipo es FUNCTION (que es el que queremos recoger)
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TFUNCTION))
		{
			showErrorMessage("ERROR DE LUA! - El procedimiento \"" + std::string(name) + "\" que se está intentando ejecutar no existe o es un procedimiento.");
			return false;
		}
		// Lo ejecuto y hago comprobación de errores.
		try {
			obj();
		} catch (luabind::error &ex) {
			showErrorMessage("ERROR DE LUA! - Error al ejecutar el procedimiento \"" + std::string(name) + "\". Tipo de error: " + std::string(ex.what()));
			return false;
		}

		return true;

	} //executeProcedure (no params)

	//---------------------------------------------------------

	template <class T>
	bool CServer::executeProcedure(const char *name, const T& param1){
		
		assert(_lua && "No se ha hecho la inicialización de lua");

		// Obtengo el procedimiento definido en lua
		luabind::object obj = luabind::globals(_lua)[name];

		//comprobacion de que el tipo es FUNCTION (que es el que queremos recoger)
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TFUNCTION))
		{
			showErrorMessage("ERROR DE LUA! - El procedimiento \"" + std::string(name) + "\" que se está intentando ejecutar no existe o es un procedimiento.");
			return false;
		}
		// Lo ejecuto y hago comprobación de errores.
		try {
			obj(param1);
		} catch (luabind::error &ex) {
			showErrorMessage("ERROR DE LUA! - Error al ejecutar el procedimiento \"" + std::string(name) + "\". Tipo de error: " + std::string(ex.what()));
			return false;
		}

		return true;

	} //executeProcedure (1 param)


bool CServer::executeProcedureString(const char *name, std::string param1){
		
		assert(_lua && "No se ha hecho la inicialización de lua");

		// Obtengo el procedimiento definido en lua
		luabind::object obj = luabind::globals(_lua)[name];

		//comprobacion de que el tipo es FUNCTION (que es el que queremos recoger)
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TFUNCTION))
		{
			showErrorMessage("ERROR DE LUA! - El procedimiento \"" + std::string(name) + "\" que se está intentando ejecutar no existe o es un procedimiento.");
			return false;
		}
		// Lo ejecuto y hago comprobación de errores.
		try {
			obj(param1);
		} catch (luabind::error &ex) {
			showErrorMessage("ERROR DE LUA! - Error al ejecutar el procedimiento \"" + std::string(name) + "\". Tipo de error: " + std::string(ex.what()));
			return false;
		}

		return true;

	} //executeProcedureString (1 param)


	//---------------------------------------------------------

	template <class T>
	bool CServer::executeProcedure(const char *name, const T& param1, const T& param2){
		
		assert(_lua && "No se ha hecho la inicialización de lua");

		// Obtengo el procedimiento definido en lua
		luabind::object obj = luabind::globals(_lua)[name];

		//comprobacion de que el tipo es FUNCTION (que es el que queremos recoger)
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TFUNCTION))
		{
			showErrorMessage("ERROR DE LUA! - El procedimiento \"" + std::string(name) + "\" que se está intentando ejecutar no existe o es un procedimiento.");
			return false;
		}
		// Lo ejecuto y hago comprobación de errores.
		try {
			obj(param1,param2);
		} catch (luabind::error &ex) {
			showErrorMessage("ERROR DE LUA! - Error al ejecutar el procedimiento \"" + std::string(name) + "\". Tipo de error: " + std::string(ex.what()));
			return false;
		}

		return true;

	} //executeProcedure (2 params)

	//---------------------------------------------------------

	template <class T>
	bool CServer::executeFunction(const char *name, const T& param1, int &result){

		assert(_lua && "No se ha hecho la inicialización de lua");

		// Obtengo la función definida en lua
		luabind::object obj = luabind::globals(_lua)[name];

		//comprobacion de que el tipo es FUNCTION (que es el que queremos recoger)
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TFUNCTION))
		{
			showErrorMessage("ERROR DE LUA! - La funcion \"" + std::string(name) + "\" que se está intentando ejecutar no existe o no es una funcion.");
			return false;
		}

		try{
			//intento ejecutar la funcion con el param1
			luabind::object res = obj(param1);

			//Hago comprobación de errores para asegurarme de que lo que me ha devuelto la función es correcto.
			if(!res.is_valid() || (luabind::type(res) != LUA_TNUMBER))
			{
				showErrorMessage("ERROR DE LUA! - La funcion \"" + std::string(name) + "\" no ha devuelto ningún valor o el valor devuelto no es de tipo entero.");
				return false;
			}


			result = luabind::object_cast<int>(res);

		} catch (luabind::error &ex) {
			showErrorMessage("ERROR DE LUA! - Error al ejecutar la función \"" + std::string(name) + "\". Tipo de error: " + std::string(ex.what()));
			return false;
		}

		return true;
	}//executeFunction

	//---------------------------------------------------------

	template <class F>
	void CServer::registerFunction(const char *name, F f)
	{
		assert(_lua && "No se ha hecho la inicialización de lua");

		luabind::module(_lua)
		[
			luabind::def(name, f)
		];
		
	} // registerFunction

	//---------------------------------------------------------

	void CServer::showMessage(std::string &message)
	{
		std::cout << endl;
		std::cout << message;
		std::cout << endl;
		
	} // showMessage

	//---------------------------------------------------------

	void CServer::showErrorMessage(std::string &message)
	{
		assert(!"showErrorMessage: Ha ocurrido un error con lua. Mira la consola para saber porqué.");

		std::cout << endl;
		std::cout << message;
		std::cout << endl;

	
	} // showErrorMessage

	//---------------------------------------------------------

	void CServer::registerClasses()
	{

		assert(_lua && "No se ha hecho la inicialización de lua");

		//------------------------------------------------------//
		//			REGISTRO DEL SERVIDOR DE SCRIPTS			//
		//------------------------------------------------------//
		
		luabind::module(_lua) 
			[ 
				/** Note: ScriptManager is a class with static member functions */ 
				luabind::class_<CServer>("ScriptManager")
				.scope
				[
					luabind::def("getSingleton", &CServer::getSingletonPtr)
				]
				.def("loadScript", &CServer::loadExeScript)
			];

		//Con esta variable Global en LUA llamada SManager puedo cargar ficheros con el metodo loadScript
		luabind::globals(_lua)["SManager"] = CServer::getSingletonPtr();


		//------------------------------------------------------//
		//				REGISTRO DE LOS MENSAJES					//
		//------------------------------------------------------//



		// Funciones especiales

		luabind::module(_lua)
			[
				luabind::def("isPlayer", &isPlayer)
			];
		luabind::module(_lua)
			[
				luabind::def("getEntityID", &getEntityID)
			];
		luabind::module(_lua)
			[
				luabind::def("getName", &getName)
			];
		luabind::module(_lua)
			[
				luabind::def("deleteEntity", &deleteEntity)
			];
		luabind::module(_lua)
			[
				luabind::def("gameOver", &gameOver)
			];

	}//registerClasses


	//---------------------------------------------------------

	bool CServer::initCorutine(const char *corutineName, const char *corutineFunction, const char *resultName)
	{
		// Creo la corrutina.
		std::stringstream corutineCreate;
		corutineCreate << corutineName << " = coroutine.create(" << corutineFunction << ")";
		executeScript(corutineCreate.str().c_str());

		// Ejecuto la corrutina.
		std::stringstream corutineInit;
		corutineInit << "correct, " << resultName << " = coroutine.resume(" << corutineName << ")";
		executeScript(corutineInit.str().c_str());

		// Variable temporal para llamar a "getGlobal"
		bool result;

		// Devuelvo si la corrutina se ha ejecutado correctamente o no.
		return ScriptManager::CServer::getSingletonPtr()->getGlobal("correct", result);

	} // initCorutine

	//---------------------------------------------------------

	bool CServer::resumeCorutine(const char *corutineName, const char *resultName, const char *param)
	{
		// Ejecuto la corrutina
		std::stringstream corutineResume;
		corutineResume << "correct, " << resultName << " = coroutine.resume(" << corutineName << ", " << param <<")";
		executeScript(corutineResume.str().c_str());

		// Variable temporal para llamar a "getGlobal"
		bool result;

		// Devuelvo si la corrutina se ha ejecutado correctamente o no.
		return ScriptManager::CServer::getSingletonPtr()->getGlobal("correct", result);

	} // resumeCorutine

	//---------------------------------------------------------



	//------------------------------------------------------//
	//				DEFINICIONES DE PLANTILLAS				//
	//------------------------------------------------------//

	template bool CServer::executeProcedure<int>(const char *subroutineName, const int& param1);
	//template bool CServer::executeProcedure<const char*>(const char *subroutineName, const char* param1);
	template bool CServer::executeProcedure<int>(const char *subroutineName, const int& param1, const int& param2);
	template bool CServer::executeProcedure<unsigned short>(const char *subroutineName, const unsigned short& param1);
	template bool CServer::executeProcedure<unsigned short>(const char *subroutineName, const unsigned short& param1, const unsigned short& param2);
	template bool CServer::executeFunction<int>(const char *subroutineName, const int& param1, int &result);
	template void CServer::registerFunction<int(*)(lua_State *)>(const char *name, int(*f)(lua_State*));
	//template void CServer::registerFunction<void(*)()>(const char *name, void (*f)());
	template void CServer::setGlobal<int>(const char *name, const int& value);
	//template void CServer::setGlobal<const char>(const char *name, const char& value);

} //end namespace ScriptManager