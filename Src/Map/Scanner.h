/**
@file Scanner.h

Contiene la declaración de un scanner flex con funcionalidades extra.

@see Map::CScanner

@author David Llansó García
@date Agosto, 2010
*/

#ifndef __Scanner_H
#define __Scanner_H

#ifndef YY_DECL

// Macro para la declaración de la función principal usada en el análisis léxico
#define	YY_DECL						\
    Map::CParser::token_type Map::CScanner::lex(		\
				Map::CParser::semantic_type* yylval,	\
				Map::CParser::location_type* yylloc		\
				)
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer CMapFlexLexer
#include "FlexLexer.h"
#undef yyFlexLexer
#endif

#include "Parser.h"

namespace Map {

	/** 
	CScanner es una clase que dota de funcionalidades extra a la clase
	automáticamente generada por Flex CMapFlexLexer.

	@ingroup mapParserGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CScanner : public CMapFlexLexer
	{
	public:
		/** 
		Crea un nuevo objeto scanner. Los streams arg_yyin y arg_yyout
		están por defecto puestos a cin y cout.

		@param arg_yyin Entrada de donde se leerán los tokens.
		@param arg_yyout Salida donde se escribirán los errores, etc.
		*/
		CScanner(std::istream* arg_yyin = 0,
			std::ostream* arg_yyout = 0);

		/** 
		Destructor.
		*/
		virtual ~CScanner();

		/**
		Esta es la función principal usada en el análisis léxico. Es 
		generada por Flex acorde a la declaración de la macro YY_DECL.
		El parser bison generado llamará a esta función virtual para
		ir obteniendo los nuevos tokens.
		*/
		virtual CParser::token_type lex(
						CParser::semantic_type* yylval,
						CParser::location_type* yylloc
						);

		/**
		Habilita la salida de depuración (via arg_yyout).
		*/
		void set_debug(bool b);
	};

} // namespace Map

#endif // __Scanner_H
