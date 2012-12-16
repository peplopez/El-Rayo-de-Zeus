/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2010 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   Maplex

/* First part of user declarations.  */

/* Line 310 of lalr1.cc  */
#line 10 "Parser.yy"
 /*** Declaraciones C++ ***/

#include <stdio.h>
#include <string>
#include <map>
#include <cassert>
#include "MapEntity.h"



/* Line 310 of lalr1.cc  */
#line 52 "Parser.cpp"


#include "Parser.h"

/* User implementation prologue.  */

/* Line 316 of lalr1.cc  */
#line 94 "Parser.yy"


#include "MapParser.h"
#include "scanner.h"

/* De esta manera conectamos el parser Bison con el scanner Flex en el mapParser.
   Define la llamada a función yylex() que devuelve el siguiente token del
   _lexer del mapParser. */
#undef yylex
#define yylex _mapParser._lexer->lex



/* Line 316 of lalr1.cc  */
#line 75 "Parser.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace Map {

/* Line 379 of lalr1.cc  */
#line 141 "Parser.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  CParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  CParser::CParser (class CMapParser& _mapParser_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      _mapParser (_mapParser_yyarg)
  {
  }

  CParser::~CParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  CParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  CParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  CParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 8: /* "TRUE" */

/* Line 479 of lalr1.cc  */
#line 85 "Parser.yy"
	{ delete (yyvaluep->string); };

/* Line 479 of lalr1.cc  */
#line 248 "Parser.cpp"
	break;
      case 9: /* "FALSE" */

/* Line 479 of lalr1.cc  */
#line 86 "Parser.yy"
	{ delete (yyvaluep->string); };

/* Line 479 of lalr1.cc  */
#line 257 "Parser.cpp"
	break;
      case 10: /* "NUMBER" */

/* Line 479 of lalr1.cc  */
#line 87 "Parser.yy"
	{ delete (yyvaluep->string); };

/* Line 479 of lalr1.cc  */
#line 266 "Parser.cpp"
	break;
      case 11: /* "STRING" */

/* Line 479 of lalr1.cc  */
#line 88 "Parser.yy"
	{ delete (yyvaluep->string); };

/* Line 479 of lalr1.cc  */
#line 275 "Parser.cpp"
	break;
      case 12: /* "ID" */

/* Line 479 of lalr1.cc  */
#line 89 "Parser.yy"
	{ delete (yyvaluep->string); };

/* Line 479 of lalr1.cc  */
#line 284 "Parser.cpp"
	break;
      case 21: /* "Feature" */

/* Line 479 of lalr1.cc  */
#line 90 "Parser.yy"
	{ delete (yyvaluep->string); };

/* Line 479 of lalr1.cc  */
#line 293 "Parser.cpp"
	break;

	default:
	  break;
      }
  }

  void
  CParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  CParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  CParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  CParser::debug_level_type
  CParser::debug_level () const
  {
    return yydebug_;
  }

  void
  CParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  CParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
/* Line 552 of lalr1.cc  */
#line 45 "Parser.yy"
{
    // Ponemos el nombre del fichero de entrada
    yylloc.begin.filename = yylloc.end.filename = &_mapParser._streamname;
}

/* Line 552 of lalr1.cc  */
#line 379 "Parser.cpp"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 677 of lalr1.cc  */
#line 115 "Parser.yy"
    {printf("done");}
    break;

  case 3:

/* Line 677 of lalr1.cc  */
#line 118 "Parser.yy"
    {}
    break;

  case 4:

/* Line 677 of lalr1.cc  */
#line 119 "Parser.yy"
    {}
    break;

  case 5:

/* Line 677 of lalr1.cc  */
#line 123 "Parser.yy"
    {
			_mapParser._entityInProgress = new Map::CEntity((yysemantic_stack_[(1) - (1)].string)->c_str());
			delete (yysemantic_stack_[(1) - (1)].string);
		}
    break;

  case 6:

/* Line 677 of lalr1.cc  */
#line 131 "Parser.yy"
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
    break;

  case 7:

/* Line 677 of lalr1.cc  */
#line 146 "Parser.yy"
    {}
    break;

  case 8:

/* Line 677 of lalr1.cc  */
#line 147 "Parser.yy"
    {}
    break;

  case 9:

/* Line 677 of lalr1.cc  */
#line 150 "Parser.yy"
    {
			assert((yysemantic_stack_[(3) - (1)].string)->compare("name") && "El atributo \"name\" no se puede definir.");
			if(!(yysemantic_stack_[(3) - (1)].string)->compare("type"))
				_mapParser._entityInProgress->setType((yysemantic_stack_[(3) - (3)].string)->c_str());
			else
				_mapParser._entityInProgress->setAttribute((yysemantic_stack_[(3) - (1)].string)->c_str(),(yysemantic_stack_[(3) - (3)].string)->c_str());
			delete (yysemantic_stack_[(3) - (1)].string);
			delete (yysemantic_stack_[(3) - (3)].string);
			}
    break;

  case 10:

/* Line 677 of lalr1.cc  */
#line 161 "Parser.yy"
    {(yyval.string) = (yysemantic_stack_[(1) - (1)].string);}
    break;

  case 11:

/* Line 677 of lalr1.cc  */
#line 162 "Parser.yy"
    {(yyval.string) = (yysemantic_stack_[(1) - (1)].string);}
    break;

  case 12:

/* Line 677 of lalr1.cc  */
#line 163 "Parser.yy"
    {(yyval.string) = (yysemantic_stack_[(1) - (1)].string);}
    break;

  case 13:

/* Line 677 of lalr1.cc  */
#line 164 "Parser.yy"
    {(yyval.string) = (yysemantic_stack_[(1) - (1)].string);}
    break;

  case 14:

/* Line 677 of lalr1.cc  */
#line 165 "Parser.yy"
    {(yyval.string) = (yysemantic_stack_[(1) - (1)].string);}
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 175 "Parser.yy"
    {
			std::string separator = " ";
			std::string aux = *(yysemantic_stack_[(7) - (2)].string) + separator + *(yysemantic_stack_[(7) - (4)].string) + separator + *(yysemantic_stack_[(7) - (6)].string);
			delete (yysemantic_stack_[(7) - (2)].string);
			delete (yysemantic_stack_[(7) - (4)].string);
			delete (yysemantic_stack_[(7) - (6)].string);
			(yyval.string) = new std::string(aux);
		}
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 185 "Parser.yy"
    {}
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 186 "Parser.yy"
    {}
    break;



/* Line 677 of lalr1.cc  */
#line 640 "Parser.cpp"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  CParser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char CParser::yypact_ninf_ = -20;
  const signed char
  CParser::yypact_[] =
  {
       -11,     4,    17,     5,   -20,   -20,     1,   -20,   -20,    11,
      15,   -20,   -20,   -20,    13,   -20,     2,   -20,    18,    11,
      -5,   -20,    10,   -20,   -20,   -20,   -20,   -20,   -20,    11,
      12,    11,    14,    16,   -20
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  CParser::yydefact_[] =
  {
         0,     0,     0,     0,     1,     4,     0,     2,     5,     0,
       0,    16,    17,     3,     0,     8,     0,     6,     0,     0,
       0,     7,     0,    13,    14,    11,    10,     9,    12,     0,
       0,     0,     0,     0,    15
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  CParser::yypgoto_[] =
  {
       -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -19
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  CParser::yydefgoto_[] =
  {
        -1,     2,     6,     9,    10,    16,    19,    27,    28,    13
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char CParser::yytable_ninf_ = -1;
  const unsigned char
  CParser::yytable_[] =
  {
        21,    22,     1,    23,    24,    25,    26,     3,     7,    17,
      30,     5,    32,     8,    18,    11,    12,     4,    14,    15,
      29,    20,    31,    34,    33
  };

  /* YYCHECK.  */
  const unsigned char
  CParser::yycheck_[] =
  {
        19,     6,    13,     8,     9,    10,    11,     3,     7,     7,
      29,     6,    31,    12,    12,     4,     5,     0,     3,     6,
      10,     3,    10,     7,    10
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  CParser::yystos_[] =
  {
         0,    13,    15,     3,     0,     6,    16,     7,    12,    17,
      18,     4,     5,    23,     3,     6,    19,     7,    12,    20,
       3,    23,     6,     8,     9,    10,    11,    21,    22,    10,
      23,    10,    23,    10,     7
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  CParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  CParser::yyr1_[] =
  {
         0,    14,    15,    16,    16,    18,    17,    19,    19,    20,
      21,    21,    21,    21,    21,    22,    23,    23
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  CParser::yyr2_[] =
  {
         0,     2,     5,     3,     0,     0,     6,     3,     0,     3,
       1,     1,     1,     1,     1,     7,     1,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const CParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "EQUAL", "SEMICOLON", "COMMA",
  "LEFT_PAR", "RIGHT_PAR", "TRUE", "FALSE", "NUMBER", "STRING", "ID",
  "MAPBEGIN", "$accept", "Map", "Entities", "Entity", "$@1", "Attributes",
  "Attribute", "Feature", "Position", "Separator", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const CParser::rhs_number_type
  CParser::yyrhs_[] =
  {
        15,     0,    -1,    13,     3,     6,    16,     7,    -1,    16,
      17,    23,    -1,    -1,    -1,    12,    18,     3,     6,    19,
       7,    -1,    19,    20,    23,    -1,    -1,    12,     3,    21,
      -1,    11,    -1,    10,    -1,    22,    -1,     8,    -1,     9,
      -1,     6,    10,    23,    10,    23,    10,     7,    -1,     4,
      -1,     5,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  CParser::yyprhs_[] =
  {
         0,     0,     3,     9,    13,    14,    15,    22,    26,    27,
      31,    33,    35,    37,    39,    41,    49,    51
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  CParser::yyrline_[] =
  {
         0,   111,   111,   118,   119,   123,   122,   146,   147,   150,
     161,   162,   163,   164,   165,   168,   185,   186
  };

  // Print the state stack on the debug stream.
  void
  CParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  CParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  CParser::token_number_type
  CParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int CParser::yyeof_ = 0;
  const int CParser::yylast_ = 24;
  const int CParser::yynnts_ = 10;
  const int CParser::yyempty_ = -2;
  const int CParser::yyfinal_ = 4;
  const int CParser::yyterror_ = 1;
  const int CParser::yyerrcode_ = 256;
  const int CParser::yyntokens_ = 14;

  const unsigned int CParser::yyuser_token_number_max_ = 268;
  const CParser::token_number_type CParser::yyundef_token_ = 2;


} // Map

/* Line 1053 of lalr1.cc  */
#line 1070 "Parser.cpp"


/* Line 1055 of lalr1.cc  */
#line 191 "Parser.yy"
 /*** Código C++ adicional para el parser ***/

void Map::CParser::error(const CParser::location_type& l,
			    const std::string& m)
{
    _mapParser.error(l, m);
}

