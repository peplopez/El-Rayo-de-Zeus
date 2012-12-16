/**
@file Documentacion.h

Fichero "dummy" sin nada de código, utilizado únicamente para mantener 
documentación de Doxygen de la parte de mapas de entidades lógicas.

@see mapGroup

@author David Llansó
@date Septiembre, 2010
*/

/**
@defgroup mapGroup Mapas de entidades lógicas

Las entidades de la capa lógica (ver @ref entityGroup) se organizan en 
mapas lógicos. Así pues, la clase Logic::CMap se encarga de almacenar 
todas las entidades del mapa, tanto aquellas que tienen representación 
gráfica y se ven como entidades puramente lógicas. 
<p>
El mapa de entidades es el responsable de gestionar la activación y
desactivación de éstas. Tiene un método de actualización Logic::CMap::tick()
que se encarga de actualizar todas las entidades y tiene también métodos 
para recuperar entidades, tanto por su nombre como por su tipo y por su 
identificador.
<p>
Para la generación y adición de entidades a un mapa se dispone de una
factoría de entidades Logic::CEntityFactory que es la única instancia que
puede crear o destruir entidades. Para poder crear entidades, en esta
factoría se cargan las definiciones blueprints de las entidades. El fichero 
que contiene la definición es muy simple. Cada línea corresponde a una 
entidad que viene definida por una serie de palabras separadas por espacios 
donde la primera palabra es el tipo de la entidad y el resto son los 
componentes que conforman dicha entidad:

\code
...
Player CAvatarController CAnimatedGraphics
...
\endcode

En la línea anterior estaríamos definiendo la entidad tipo <code>Player</code>
que consta de dos componentes <code>CAvatarController</code> y 
<code>CAnimatedGraphics</code>. 
<p>
Una vez se han cargado las definiciones de entidades del fichero o ficheros 
blueprints, se pueden crear entidades en un mapa. Para el ensamblaje de la 
entidad se usa la factoría de componentes Logic::CComponentFactory y tras 
añadir la entidad al mapa se inicializa.
<p>
Logic::CEntityFactory es también responsable de la destrucción de entidades
de un mapa, pero la destrucción de una entidad en medio de un tick() del
mapa lógico es potencialmente peligrosa, ya que no se sabe si otras entidades
tendrán acceso a la entidad, si la propia entidad se encuentra en medio de su
actualización, etc. Es por ello que se recomienda hacer uso de la función 
Logic::CEntityFactory::deferredDeleteEntity() que en vez de destruir la entidad
al instante espera a que se invoque Logic::CEntityFactory::deleteDefferedEntities()
antes del siguiente tick() del mapa para destruir todas las entidades pendientes 
de destruir.
<p>
Para simplificar la taréa de generar un nivel a partir de un fichero con una 
descripción de mapa existe la función estática Logic::CMap::createMapFromFile(). 
Para ello se ayuda del parseador de mapas (ver @ref mapParserGroup) que parsea 
un fichero generando una estructura tipo lista con la descripción de todas
las entidades del mapa. Una vez se tienen todas las descripciones de 
entidades se usa la factoría de entidaes para generar e inicializar todas las
añadiéndolas al mapa que devuelve la función.

@author David Llansó
@date Septiembre, 2010
*/