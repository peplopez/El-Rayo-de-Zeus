/** 
@file Parser.yy 

Contiene el codigo autogenerado del parser Bison.

@author David Llansó
@date Agosto, 2010
*/

%{ /*** Declaraciones C++ ***/

#include <stdio.h>
#include <string>
#include <map>
#include <cassert>
#include "MapEntity.h"

%}

/*** Declaraciones yacc/Bison ***/

/* Requiere Bison 2.3 o superior */
%require "2.3"

/* Añade salida de depuración. Para la versión definitiva debería eliminarse esta opción. */
%debug

/* El símbolo de empiece de la gramática es "Map" */
%start Map

/* Escribe una cabecera con los tokens */
%defines

/* Usamos el skeleton file "lalr1.cc" */
%skeleton "lalr1.cc"

/* namespace en el que se encuentra el parser */
%name-prefix="Map"

/* El nombre de la clase será "CParser" */
%define "parser_class_name" "CParser"

%locations
%initial-action
{
    // Ponemos el nombre del fichero de entrada
    @$.begin.filename = @$.end.filename = &_mapParser._streamname;
};

/* CMapParser se pasa como referencia al parser y al scenner, así no necesitamos
   variables globales. */
%parse-param { class CMapParser& _mapParser }

/* Mensajes de error*/
%error-verbose

/*** EMPIECE de los tokens de la gramática ***/

%union {
   std::string* string;
   int integer;
}

%token END 0 "end of file"
%token EQUAL
%token SEMICOLON
%token COMMA
%token LEFT_PAR
%token RIGHT_PAR
%token <string> TRUE
%token <string> FALSE
%token <string> NUMBER
%token <string> STRING
%token <string> ID
%token MAPBEGIN

%type <string> Entities
%type <string> Entity
%type <string> Attributes
%type <string> Attribute
%type <string> Feature
%type <string> Position
%type <string> Separator

%destructor { delete $$; } TRUE
%destructor { delete $$; } FALSE
%destructor { delete $$; } NUMBER
%destructor { delete $$; } STRING
%destructor { delete $$; } ID
%destructor { delete $$; } Feature

/*** FIN de los tokens de la gramática ***/

%{

#include "MapParser.h"
#include "scanner.h"

/* De esta manera conectamos el parser Bison con el scanner Flex en el mapParser.
   Define la llamada a función yylex() que devuelve el siguiente token del
   _lexer del mapParser. */
#undef yylex
#define yylex _mapParser._lexer->lex

%}

%% /*** Reglas gramaticales ***/

/*** EMPIECE de las reglas gramaticales ***/
 
Map: 	MAPBEGIN 
		EQUAL 
		LEFT_PAR 
		Entities 
		RIGHT_PAR {printf("done");}
		;
		
Entities: Entities Entity Separator {}
		| {}
		;
		
Entity: ID 
		{
			_mapParser._entityInProgress = new Map::CEntity($1->c_str());
			delete $1;
		}
		EQUAL
        LEFT_PAR
		Attributes 
		RIGHT_PAR 
		{
		#ifdef _DEBUG
			assert(_mapParser._entityInProgress->getType().compare("") && "No se ha establecido tipo a la entidad");
			Map::CMapParser::TEntityList::const_iterator it, end;
			it = _mapParser._entityList.begin();
			end = _mapParser._entityList.end();
			for(; it != end; it++)
			{
				assert((*it)->getName().compare(_mapParser._entityInProgress->getName()) && "Ya existe una entidad con este nombre.");
			}
		#endif // _DEBUG
			_mapParser._entityList.push_back(_mapParser._entityInProgress);
		}
		;
		
Attributes: Attributes Attribute Separator {}
		| {}
		;
		
Attribute: ID EQUAL Feature {
			assert($1->compare("name") && "El atributo \"name\" no se puede definir.");
			if(!$1->compare("type"))
				_mapParser._entityInProgress->setType($3->c_str());
			else
				_mapParser._entityInProgress->setAttribute($1->c_str(),$3->c_str());
			delete $1;
			delete $3;
			}
		;
			
Feature: STRING {$$ = $1;}
		| NUMBER {$$ = $1;}
		| Position {$$ = $1;}
		| TRUE {$$ = $1;}
		| FALSE {$$ = $1;}
		;
			
Position: LEFT_PAR 
		NUMBER 
		Separator
		NUMBER 
		Separator
		NUMBER 
		RIGHT_PAR 
		{
			std::string separator = " ";
			std::string aux = *$2 + separator + *$4 + separator + *$6;
			delete $2;
			delete $4;
			delete $6;
			$$ = new std::string(aux);
		}
		;
		
Separator: SEMICOLON {}
		| COMMA {}
		;

/*** FIN de las reglas gramaticales ***/

%% /*** Código C++ adicional para el parser ***/

void Map::CParser::error(const CParser::location_type& l,
			    const std::string& m)
{
    _mapParser.error(l, m);
}
