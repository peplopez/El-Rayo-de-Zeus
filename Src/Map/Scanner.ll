/** 
@file Scanner.ll 

Contiene el codigo autogenerado del scanner léxico de Flex.

@author David Llansó
@date Agosto, 2010
*/

%{ /*** Declaraciones C++ ***/

#include <string>

#include "Scanner.h"

#include <stdio.h>
#include "Parser.h"

/* importamos el tipo de los tokens del parser a un tipo local */
typedef Map::CParser::token token;
typedef Map::CParser::token_type token_type;

/* Redefinimos yyterminate para que devuelva el token END. */
#define yyterminate() return token::END

/* Desabilitamos la inclusión de unistd.h, no disponible bajo Visual C++
   en Win32. A cambio se usan STL streams. */
#define YY_NO_UNISTD_H

%}

/*** Declaraciones y opciones de Flex ***/

/* Habilitamos la generación de la clase C++ para el scanner*/
%option c++

/* Cambiamos el prefijo de la clase generada. El resultado será "CMapFlexLexer" */
%option prefix="CMap"

/* Una optimización */
%option batch

/* Añade salida de depuración. Para la versión definitiva debería eliminarse esta opción. */
%option debug

/* No permitimos inclusiones de ficheros */
%option yywrap nounput 

%option stack

%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

_STRING \"[^\"]*\"
_ID [[:alpha:]_][[:alnum:]_]*
_NUMBER "-"?[0-9]+"."?[0-9]*

%% /*** Parte de las expresiones regulares ***/

 /* Código añadido al principio de yylex() */
%{
    // reset location
    yylloc->step();
%}

 /*** EMPIECE de las reglas léxicas ***/

"Map"        {return(token::MAPBEGIN);}
"true"       {yylval->string = new std::string(yytext, yyleng);return(token::TRUE);}
"false"       {yylval->string = new std::string(yytext, yyleng);return(token::FALSE);}
"="          {return(token::EQUAL);}
";"          {return(token::SEMICOLON);}
","          {return(token::COMMA);}
"{"          {return(token::LEFT_PAR);}
"}"          {return(token::RIGHT_PAR);}
{_NUMBER}    {yylval->string = new std::string(yytext, yyleng); return(token::NUMBER);}
{_ID}      {yylval->string = new std::string(yytext, yyleng); return(token::ID);}
{_STRING}    {yylval->string = new std::string(yytext+1, yyleng-2); return(token::STRING);} /* Quitamos las comillas */

 /*** FIN de las reglas léxicas ***/

%% /*** Código adicional ***/

namespace Map {

	CScanner::CScanner(std::istream* in,
			 std::ostream* out)
		: CMapFlexLexer(in, out)
	{
	}

	CScanner::~CScanner()
	{
	}

	void CScanner::set_debug(bool b)
	{
		yy_flex_debug = b;
	}

}

/**
Debemos reimplementar CMapFlexLexer::yylex() para completar la vtable de
CMapFlexLexer. Esta nueva definición sin embargo reporta un error ya que
lo que haremos es definir la funcionalidad de esta función en la clase
CScanner que heredará de CMapFlexLexer y es la que se usará. 
*/

#ifdef yylex
#undef yylex
#endif

int CMapFlexLexer::yylex()
{
    std::cerr << "in CMapFlexLexer::yylex() !" << std::endl;
    return 0;
}

int CMapFlexLexer::yywrap()
{
    return 1;
}
