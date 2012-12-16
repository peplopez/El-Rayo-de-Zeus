/**
@file Documentacion.h

Fichero "dummy" sin nada de código, utilizado únicamente para mantener 
documentación de Doxygen de la parte del parseador de ficheros de mapas.

@see mapParserGroup

@author David Llansó
@date Septiembre, 2010
*/

/**
@defgroup mapParserGroup Parseador de ficheros de mapas

Para la taréa del parseo de ficheros con definiciones de mapas se tiene el 
proyecto Map en el que se analizan léxica y sintácticamente los mapas descritos 
en ficheros para extraer de ellos la lista de descripciones de las entidades.
<p>
El grueso del proyecto se trata de código autogenerado por Flex y Bison 
(predecesores de Lex y Yacc), por lo que ese código <em>no</em> deberá ser
modificado y si se desea cambiar la gramática se deberán modificar los ficheros
con las descripciones de las gramáticas léxica y sintáctica (Map/Scanner.ll y 
Map/Parser.yy) y generar de nuevo a partir de ellos los ficheros. Para este fin
se encuentra información extra en Map/Readme.txt.
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

Donde los nombres de los atributos deben empezar por una letra o '_' y contener
letras, números o '_' y los valores de los atriburos pueden ser cadenas encerradas
entre dobles comillas '"', numeros enteros o reales, "true", "false" o posiciones tipo 
"{x,y,z}". No obstante todos los valores de atributos serán almacenados como strings y 
será luego, durante el spawn de las entidaes cuando los componentes deban 
saber que tipo de datos esperan de cada atributo concreto para recuperar los atributos
con su tipo correcto mediante los métodos getXXXAttribute() de Map::CEntity. Los tipos  
de las entidades deben coincidir con las declaradas en los archivos de blueprints, 
aunque no se hará ningún tipo de comprobación aquí. Eso si, toda entidad declarada 
deberá como mínimo tener el atributo de nombre "type" y los nombres de las entidades
deberán ser diferentes entre sí. Estas comprobaciones solo se realizan en modo Debug,
por optimización del código Release. Como curiosidad cabe decir que
los saltos de línea, espacios y tabulaciones son opcionales y que puede usarse ';' 
en vez de ',' como separador si se prefiere.
<p>
La clase CMapParser reune todos los componentes necesarios para el parseo
de un mapa. Es un singleton de inicialización explícita que permite parseo
de streams. No obstante incluye métodos que facilitan el parseo de un 
archivo dado su nombre, o de un string. Su uso más común será mediante la lectura
de un fichero con Map::CMapParser::parseFile() que leerá el fichero de nombre 
especificado que se encuentre en la ruta <code>Exes/media/maps</code> y creará 
una lista de Map::CEntity's con las descripciones de las entidades.
<p>
Map::CEntity no es más que una clase contenedora de atributos tipo par
<std::string,std::string> donde el primer elemento es el nombre del atributo es el
nombre del atributo y el segundo elemento es su valor codificado en un string. Además 
contiene una serie de métodos que permiten recuperar los valores de los atributos ya
transformados a un tipo concreto (int, float, double, bool, string o Vector3).
<p>
El parseador de mapas es usado por la función estática Logic::CMap::createMapFromFile(),
allí se puede ver más claramente como generar un mapa lógico a partir de las 
definiciones de entidades (Map::CEntity) devueltas por este módulo.

@author David Llansó
@date Septiembre, 2010
*/