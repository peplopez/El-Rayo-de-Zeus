/**
@file Main.cpp

Simple test que parsea un fichero.

@author David Llansó García
@date Agosto, 2010
*/
#include <iostream>
#include <fstream>

#include "MapParser.h"

/**
Simple test que parsea un fichero.
*/
int main(int argc, char *argv[])
{
	Map::CMapParser::Init();
	
	Map::CMapParser *mapParser = Map::CMapParser::getSingletonPtr();

	if(mapParser->parseFile("../map.txt"))
		Map::CMapParser::TEntityList entitylist = mapParser->getEntityList();

	Map::CMapParser::Release();

	return 0;

} // main