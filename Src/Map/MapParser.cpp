/**
@file MapParser.cpp

Contiene la implementación de la clase que encapsula el parseo de mapas.

@see Map::CMapParser

@author David Llansó García
@date Agosto, 2010
*/

#include <fstream>
#include <sstream>
#include <cassert>

#include "MapEntity.h"
#include "MapParser.h"
#include "scanner.h"

namespace Map {

	CMapParser* CMapParser::_instance = 0;

	//--------------------------------------------------------

	CMapParser::CMapParser(): _traceScanning(false),
							  _traceParsing(false),
							  _entityInProgress(0)
	{
		_instance = this;

	} // CMapParser

	//--------------------------------------------------------

	CMapParser::~CMapParser()
	{
		releaseEntityList();
		_instance = 0;

	} // ~CMapParser
	
	//--------------------------------------------------------

	bool CMapParser::Init()
	{
		assert(!_instance && "Segunda inicialización de Map::CMapParser no permitida!");

		new CMapParser();
		return true;

	} // Init

	//--------------------------------------------------------

	void CMapParser::Release()
	{
		assert(_instance && "Map::CMapParser no está inicializado!");
		if(_instance)
		{
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CMapParser::parseStream(std::istream& in, const std::string& name)
	{
		_streamname = name;

		CScanner scanner(&in);
		scanner.set_debug(_traceScanning);
		_lexer = &scanner;

		releaseEntityList();
		CParser parser(*this);
		parser.set_debug_level(_traceParsing);
		return (parser.parse() == 0);
	} // parseStream

	//--------------------------------------------------------

	bool CMapParser::parseFile(const std::string &filename)
	{
		std::ifstream in(filename.c_str());
		if (!in.good()) return false;
		return parseStream(in, filename);
	} // parseFile

	//--------------------------------------------------------

	bool CMapParser::parseString(const std::string &input, const std::string& name)
	{
		std::istringstream iss(input);
		return parseStream(iss, name);
	} // parseString

	//--------------------------------------------------------

	void CMapParser::error(const class location& l,
			   const std::string& m)
	{
		std::cerr << l << ": " << m << std::endl;
	} // error

	//--------------------------------------------------------

	void CMapParser::error(const std::string& m)
	{
		std::cerr << m << std::endl;
	} // error

	//--------------------------------------------------------
		
	void CMapParser::releaseEntityList()
	{
		while(!_entityList.empty())
		{
			_entityInProgress = _entityList.back();
			_entityList.pop_back();
			delete _entityInProgress;
		}
	}

} // namespace Map
