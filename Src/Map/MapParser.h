/**
@file MapParser.h

Contiene la declaración de la clase que encapsula el parseo de mapas.

@see Map::CMapParser

@author David Llansó García
@date Agosto, 2010
*/


#ifndef __MapParser_H
#define __MapParser_H

#include <string>
#include <list>

// Predeclaración de clases.
namespace Map
{
	class CEntity;
	class CParser;
	class CScanner;
}


/**
Namespace con todo lo relacionado con el parseado de mapas. Se encuentran
desde el scanner y el parser de flex y bison hasta la definición de las
entidades leidas del mapa. También hay otro tipo de clases que facilitan
la unión y el uso del parseado de mapas.
(para más información ver @ref mapParserGroup).

@author David Llansó
@date Agosto, 2010
*/
namespace Map {

	/**
	La clase CMapParser reune todos los componentes necesarios para el parseo
	de un mapa. Es un singleton de inicialización explícita que permite parseo
	de streams. No obstante incluye métodos que facilitan el parseo de un 
	archivo dado su nombre, o de un string. Al iniciar un nuevo parseo se 
	crea una instancia de CParser y otra de CScanner que se conectan.
	Luego se alimenta el scanner con la entrada que es un stream y el parser
	recibe la secuencia de tokens. CMapParser es además transferida al parser
	para que este pueda añadir las entidades leidas del mapa.

	El modo de uso más común será:
	\code
Map::CMapParser::Init();

Map::CMapParser *mapParser = Map::CMapParser::getSingletonPtr();

if(mapParser->parseFile("nombre_mapa.extension"))
	Map::CMapParser::TEntityList entitylist = 
						mapParser->getEntityList();

Map::CMapParser::Release();
	\endcode

	Hay que tener en cuenta que no se deben hacer cambios en la lista de 
	entidades y es importante llamar a Release al terminar su uso para que se 
	liberen todos los datos creados durante el parseo. Antes de un nuevo 
	parseo la propia clase se encargará de eliminar datos de un parseo previo,
	por lo que se deben procesar los datos de un mapa ANTES de parsear un
	nuevo mapa.

	<p>

	Los mapas leidos por este parseador deberán seguir la siguiente estructura:

	\code
Map = {
   [
   EntityName = {
	  [AttributeName = AttributeValue,]*
   },
   ]*
}
\endcode

Por ejemplo:

\code
Map = {
   gobling1 = {
      type = "Gobling",
      position = {1.4,0,2.66},
      life = 100.0,
      ...
   },
   orc1 = {
      type = "Orc"
      modelname = "orc.mesh",
      ...
   }
   ...
}
	\endcode

	Donde los nombres de los atributos deben empezar por una letra y contener
	letras, números o '_' y los valores de los atriburos pueden ser cadenas 
	encerradas entre dobles comillas '"', numeros enteros o reales, "true", 
	"false" o posiciones tipo "{x,y,z}". 
	No obstante todos los valores de atributos serán almacenados como strings y 
	será luego, durante el spawn de las entidaes cuando los componentes deban 
	saber que tipo de datos esperan de cada atributo concreto. Los tipos de las 
	entidades deben coincidir con las declaradas en los archivos de blueprints, 
	aunque no se hará ningún tipo de comprobación aquí. Toda entidad declarada 
	deberá como mínimo tener el atributo de nombre "type" y los nombres de las 
	entidades deberán ser diferentes entre sí. Estas comprobaciones solo se 
	realizan en modo Debug, por optimización del código Release. Como curiosidad 
	cabe decir que los saltos de línea, espacios y tabulaciones son opcionales y 
	que puede usarse ';' en vez de ',' como separador si se prefiere.

	@ingroup mapParserGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CMapParser
	{
	public:

		/**
		Devuelve la única instancia de la clase CMapParser.
		
		@return Única instancia de la clase CMapParser.
		*/
		static CMapParser* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la factoría. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/** 
		Crea un CParser y un CScanner para parsear un mapa dado en
		un stream.
		
		@param in Input stream con los datos del mapa.
		@param name Nombre del stream para mensajes de error.
		@return true si el parseo fue satisfactorio.
		*/
		bool parseStream(std::istream& in,
				  const std::string& name = "stream input");

		/** 
		Crea un CParser y un CScanner para parsear un mapa dado en
		un string.
		
		@param input String con los datos del mapa.
		@param name Nombre del stream para mensajes de error.
		@return true si el parseo fue satisfactorio.
		*/
		bool parseString(const std::string& input,
				  const std::string& name = "string stream");

		/** 
		Crea un CParser y un CScanner para parsear un mapa dado en
		un nombre de fichero.
		
		@param filename nombre del fichero con los datos del mapa.
		@param name Nombre del stream para mensajes de error.
		@return true si el parseo fue satisfactorio.
		*/
		bool parseFile(const std::string& filename);

		/** 
		Para escritura de errores en caso de fallo en el parseo. escribe
		directamente en la consola.
		*/
		void error(const class location& l, const std::string& m);

		/** 
		Para escritura de errores en caso de fallo en el parseo. escribe
		directamente en la consola.
		*/
		void error(const std::string& m);

		/**
		Tipo lista de entidades de mapa.
		*/
		typedef std::list<Map::CEntity*> TEntityList;

		/**
		Devuelve el listado de las entidades leidas del mapa.

		@return Entidades parseadas.
		*/
		const TEntityList getEntityList() {return _entityList;}

		/**
		Elimina todas las entidades que contenga la lista de entidades 
		parseadas.
		*/
		void releaseEntityList();

	private:
		/**
		Única instancia de la clase.
		*/
		static CMapParser* _instance;

		/**
		Constructor.
		*/
		CMapParser();

		/**
		Destructor.
		*/
		~CMapParser();

		/**
		Habilita la salida de depuración para el scanner de flex.
		*/
		bool _traceScanning;

		/**
		Habilita la salida de depuración para el parser de bison.
		*/
		bool _traceParsing;

		/**
		Clase amiga para poder cambiar el contexto durante el parseo.
		*/
		friend class CParser;

		/**
		Nombre del stream (fichero o input stream) usado para 
		mensajes de error.
		*/
		std::string _streamname;

		/** 
		Puntero a la actual instancia del lexer. Es usado para conectar
		el parser con el scanner.
		*/
		CScanner* _lexer;

		/**
		Entidad temporal que se encuentra en medio de su parseo. Es usado
		por la clase CParser para ir generando la lista de entidades.
		*/
		Map::CEntity *_entityInProgress;

		/**
		Lista de entidades parseada.
		*/
		TEntityList _entityList;
	};

} // namespace Map

#endif // __MapParser_H
