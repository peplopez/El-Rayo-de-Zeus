//#include "ScriptManager.h"
//
//#include <cassert>
//
////Como LUA esta compilado en C puro y duro. (se necesita incluirla con extern "C")
//extern "C" {
//	#include <lua.h>
//	#include <lauxlib.h>
//	#include <lualib.h>
//}
//
////LUABIND
//#pragma warning( disable: 4251 )
//#include <luabind/luabind.hpp> //nuevo para Luabind
//
//using namespace std;
//
//namespace ScriptManager {
//
//
//	CScriptManager *CScriptManager::_instance = 0;
//
//	bool CScriptManager::open() {
//		//llamamos al lua_open
//
//		_lua  = lua_open();
//
//		if(!_lua)
//			return false;
//
//		luaopen_base(_lua); //funcion que esta dentro de la libreria lualib.h
//
//		//LUABIND
//		luabind::open(_lua);
//
//		//luaL_loadfile(_lua,"miLua.lua");
//		//lua_call(_lua, 0, 0);
//		
//		return true;
//
//	}
//
//	void CScriptManager::close() {
//		//llamamos al lua_close
//		if(_lua)
//			lua_close(_lua);
//	}
//
//	bool CScriptManager::Init() {
//		assert(!_instance);
//		_instance = new CScriptManager();
//		if (!_instance->open()) {
//			delete _instance;
//			_instance = 0;
//		}
//		return _instance != 0;
//	}
//
//	void CScriptManager::Release() {
//		if (_instance) {
//			_instance->close();
//			delete _instance;
//		}
//	}
//
//	//getGlobal con LUA
///*
//	int CScriptManager::getGlobal(const char *name, int defaultValue)
//	{
//
//		assert(_lua);
//		int result;
//		lua_getglobal(_lua,name); //recuperamos el valor de la variable "name" y se pone en el top de la pila (posicion -1)
//		if(!lua_isnumber(_lua,-1)) //si no es un numero le damos el valor por defecto
//			result = defaultValue;
//		else //si es un numero
//		    result = lua_tonumber(_lua,-1); //cojo el primer numero de la pila y lo devuelvo
//		    lua_pop(_lua,1);//desapilo el resultado para no ocupar memoria en la pila
//		return result;
//	}
//*/
//
//	// getGlobal con LUABIND
//	int CScriptManager::getGlobal(const char *name, int defaultValue)
//	{
//
//		assert(_lua);
//		luabind::object obj = luabind::globals(_lua)[name];
//
//		//comprobacion de que el tipo es ENTERO (que es el que queremos recoger)
//		if(!obj.is_valid() || (luabind::type(obj) != LUA_TNUMBER))
//			return defaultValue;
//
//		return luabind::object_cast<int>(obj);
//	}
//
//	//getGlobalBool con LUA
//	/*
//	bool CScriptManager::getGlobalBool(const char *name, bool defaultValue)
//	{
//
//		assert(_lua);
//		bool result;
//		lua_getglobal(_lua,name);
//		if(!lua_isboolean(_lua,-1))
//			result = defaultValue;
//		else
//		    result = lua_toboolean(_lua,-1); //cojo el primero de la pila
//		    lua_pop(_lua,1);//desapilo el resultado
//		return result;
//	}
//	*/
//
//	//getGlobalBool con LUABIND
//	bool CScriptManager::getGlobalBool(const char *name, bool defaultValue)
//	{
//
//		assert(_lua);
//		luabind::object obj = luabind::globals(_lua)[name];
//
//		//comprobacion de que el tipo es BOOLEANO (que es el que queremos recoger)
//		if(!obj.is_valid() || (luabind::type(obj) != LUA_TBOOLEAN))
//			return defaultValue;
//
//		return luabind::object_cast<bool>(obj);
//	}
//
//	//getGlobalString con LUA
//	/*
//	const char* CScriptManager::getGlobalString(const char *name, const char *defaultValue)
//	{
//		assert(_lua);
//		const char *result;
//		lua_getglobal(_lua,name);
//		if(!lua_isstring(_lua,-1))
//			result = defaultValue;
//		else
//		    result = lua_tostring(_lua,-1); //cojo el primero de la pila
//		    lua_pop(_lua,1);//desapilo el resultado
//		return result;
//	}
//	*/
//
//
//	//getGlobalString con LUABIND
//	const char* CScriptManager::getGlobalString(const char *name, const char *defaultValue)
//	{
//
//		assert(_lua);
//		luabind::object obj = luabind::globals(_lua)[name];
//
//		//comprobacion de que el tipo es STRING (que es el que queremos recoger)
//		if(!obj.is_valid() || (luabind::type(obj) != LUA_TSTRING))
//			return defaultValue;
//
//		return luabind::object_cast<const char*>(obj);
//	}
//
//	bool CScriptManager::loadScript(const char *file) {
//		luaL_loadfile(_lua,file);
//		lua_call(_lua, 0, 0);
//		return true;
//	}
//	
//
//	bool CScriptManager::executeScript(const char *luaCode) {
//		luaL_loadstring(_lua,luaCode);
//		lua_call(_lua, 0, 0);
//		return true;
//	}
//
//	//getField con LUA
//	/*
//	int CScriptManager::getField(const char *table, const char *field, int defaultValue){
//
//		assert(_lua);
//		int result;
//		if(!lua_istable(_lua,-1))
//
//		lua_gettable(_lua,-1); //recuperamos el valor de la variable "table" y se pone en el top de la pila (posicion -1)
//		lua_getglobal(_lua,field); //recuperamos el valor de la variable "field" y se pone en el top de la pila (posicion -1)
//
//		if(!lua_isnumber(_lua,-1)) //si no es un numero le damos el valor por defecto
//			result = defaultValue;
//		else //si es un numero
//		    result = lua_tonumber(_lua,-1); //cojo el primer numero de la pila y lo devuelvo
//
//		lua_pop(_lua,1);//desapilo el resultado para no ocupar memoria en la pila
//		return result;
//	}
//	*/
//
//	//getField con LUABIND
//	int CScriptManager::getField(const char *table, const char *field, int defaultValue){
//
//		assert(_lua);
//		luabind::object obj = luabind::globals(_lua)[table];
//
//		//comprobacion de que el tipo es TABLA (que es el que queremos recoger)
//		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
//			return defaultValue;
//
//		//Faltaria comprobar si obj[field] es un entero
//		if(luabind::type(obj[field]) != LUA_TNUMBER)
//			return defaultValue;
//
//		return luabind::object_cast<int>(obj[field]);
//	}
//
//	//getFieldBool con LUABIND
//	bool CScriptManager::getFieldBool(const char *table, const char *field, bool defaultValue){
//		assert(_lua);
//		luabind::object obj = luabind::globals(_lua)[table];
//
//		//comprobacion de que el tipo es TABLA (que es el que queremos recoger)
//		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
//			return defaultValue;
//
//		//Faltaria comprobar si obj[field] es un booleano
//		if(luabind::type(obj[field]) != LUA_TBOOLEAN)
//			return defaultValue;
//
//		return luabind::object_cast<bool>(obj[field]);
//	}
//
//	//getFieldString con LUA
//	/*
//	char* CScriptManager::getFieldString(const char *table, const char *field, char *defaultValue){
//		assert(_lua);
//		char *result;
//		lua_gettable(_lua,-1); //recuperamos el valor de la variable "table" y se pone en el top de la pila (posicion -1)
//		lua_getglobal(_lua,field); //recuperamos el valor de la variable "field" y se pone en el top de la pila (posicion -1)
//
//		if(!lua_isstring(_lua,-1)) //si no es un string le damos el valor por defecto
//			result = defaultValue;
//		else //si es un string
//		    result = (char*)lua_tostring(_lua,-1); //cojo el primer string de la pila y lo devuelvo
//
//		lua_pop(_lua,1);//desapilo el resultado para no ocupar memoria en la pila
//		return result;
//	}
//	*/
//
//	//getFieldString con LUABIND
//	const char* CScriptManager::getFieldString(const char *table, const char *field, const char *defaultValue){
//		assert(_lua);
//		luabind::object obj = luabind::globals(_lua)[table];
//
//		//comprobacion de que el tipo es TABLA (que es el que queremos recoger)
//		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
//			return defaultValue;
//
//		//Faltaria comprobar si obj[field] es un string
//		if(luabind::type(obj[field]) != LUA_TSTRING)
//			return defaultValue;
//
//		//return luabind::object_cast<const char*>(obj[field]);
//		return luabind::object_cast<const char*>(obj[field]);
//	}
//
//	//practica 8 con LUA
//	/*
//	void CScriptManager::executeProcedure(const char *name){
//		lua_getglobal(_lua,name);
//		
//		if(!lua_isfunction(_lua,-1)){
//			lua_pop(_lua,1);
//			return;
//		}
//
//		lua_call(_lua,0,0); //no hace falta desapilar, lua_call te lo desapila solo
//
//	}
//	*/
//	//Practica8 con LUABIND
//	bool CScriptManager::executeProcedure(const char *name){
//		assert(_lua);
//		luabind::object obj = luabind::globals(_lua)[name];
//
//		//comprobacion de que el tipo es FUNCTION (que es el que queremos recoger)
//		if(!obj.is_valid() || (luabind::type(obj) != LUA_TFUNCTION))
//		{
//			return false;
//		}
//		else{
//			//Asumimos que obj es una funcion, la llamamos.
//			obj();
//			return true;
//		}
//
//	}
//
//	//executeProcedure con param1 con LUA
//	/*
//	void CScriptManager::executeProcedure(const char *name, int param1){
//		lua_getglobal(_lua,name);
//		
//		if(!lua_isfunction(_lua,-1)){
//			lua_pop(_lua,1);
//			return;
//		}
//
//		lua_pushnumber(_lua,param1);
//
//		lua_call(_lua,1,0); //no hace falta desapilar, lua_call te lo desapila solo. la funcion que voy a llamar tiene
//		//un parametro apilado
//	}
//	*/
//
//	//executeProcedure con param1 con LUABIND
//	bool CScriptManager::executeProcedure(const char *name, int param1){
//		assert(_lua);
//
//		try{
//			luabind::globals(_lua)[name](param1);
//		} catch (luabind::error &ex) {
//			cout << ex.what() << endl;
//			return false;
//		}
//
//		return true;
//	}
//
//	//executeFunction con LUA
//	/*
//	int CScriptManager::executeFunction(const char *name, int param1){
//
//		lua_getglobal(_lua,name);
//		
//		if(!lua_isfunction(_lua,-1)){
//			lua_pop(_lua,1);
//			return -1;
//		}
//
//		lua_pushnumber(_lua,param1);
//
//		lua_call(_lua,1,1); //no hace falta desapilar, lua_call te lo desapila solo. la funcion que voy a llamar tiene
//		//un parametro apilado
//
//		//ahora vamos a la pila de LUA y cogerlo
//		if(!lua_isnumber(_lua,-1)){
//			lua_pop(_lua,1); //desapilamos de la pila
//			return -1;
//		}
//
//		int ret = (int)lua_tonumber(_lua,-1); //hacemos el casting porque lua_tonumber devuelve un double
//		lua_pop(_lua,1);
//		return ret;
//	}
//	*/
//	//executeFunction con LUABIND
//	bool CScriptManager::executeFunction(const char *name, int param1, int &result){
//
//		assert(_lua);
//
//		try{
//			luabind::object ret;
//			ret = luabind::globals(_lua)[name](param1);
//			if (!ret.is_valid() || (luabind::type(ret) != LUA_TNUMBER)  )
//				return false;
//			result = luabind::object_cast<int>(ret);
//		} catch (luabind::error &ex) {
//			//cout << ex.what() << endl;
//			return false;
//		}
//
//		return true;
//	}
//	void CScriptManager::registerFunction(lua_CFunction f, const char *luaName){
//		assert(_lua);
//		lua_register(_lua, luaName, f);
//	}
//
//
//} //end namespace ScriptManager