/**
@file Documentacion.h

Fichero "dummy" sin nada de código, utilizado únicamente para mantener 
documentación de Doxygen de la parte de la lógica de juego.

@see logicGroup

@author David Llansó
@date Septiembre, 2010
*/

/**
@defgroup logicGroup Lógica de juego

La lógica de juego son las reglas del juego que se quiere implementar; 
aquí es donde se definen conceptos como que tipo de juego se va a 
realizar, sus reglas, que entidades habrá, como se crearán y organizarán,
como se comportarán, etc.
<p>
La lógica inicialmente planteada corresponde a un juego en tiempo real
en tercera persona. Inicialmente no se ha añadido ningún tipo de regla 
de juego extra y puede servir como punto de partida para diferentes 
proyectos de este tipo de juegos o de otro tipo de juegos con alguna 
modificación.
<p>
Las entidades de lógica propuestas se basan en una arquitectura de
componentes (ver @ref entityGroup) y para su creación y organización se
tiene un mapa lógico (ver @ref mapGroup) que carga un mapa de fichero
que contiene todas las entidades con sus atributos.
<p>
El acceso a la lógica del juego está centralizada en Logic::CServer, un
singleton de inicialización explícita. Éste nos permite cargar un nivel
a partir de un fichero con una definición de mapa, que deberá luego 
activarse al comienzo de la partida. Cuando la partida está activa el
servidor debe ser actualizado periodicamente para que se actualice el 
mapa y por tanto todas las entidades puedan actuar y realizar sus 
acciones. En la implementación que se ha llevado a cabo solo permitimos
tener un mapa o nivel cargado, por lo que si se desea cargar otro nivel
se borrará el anterior. Esto conlleva a que los métodos de gestión de
mapas que tiene el servidor sean triviales. Si se quisiesen realizar
implementaciones donde se permitan multiples mapas o niveles cargados
al mismo tiempo habría que revisar la implementación de los métodos de
gestion de mapas del servidor.
<p>
Falta comentar que para poder cargar correctamente las entidades
del mapa es necesario que se encuentren cargadas las definiciones 
blueprints de todas las entidades por componentes que se quieren 
permitir.
<p>
El orden cronológico de las llamadas a la lógica vendría marcado por
el siguiente marco, aunque obviamente las llamadas reales no se realizan
todas juntas sino que dependen del estado de juego de la aplicación y sus
diferentes métodos (se puede ver un ejemplo en Application::CGameState):
<p>

\code
if (!Logic::CServer::Init())
    return;

if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
	return;

Logic::CServer::getSingletonPtr()->loadLevel("map.txt");

...

Logic::CServer::getSingletonPtr()->activateMap();

while(...)
{

...

Logic::CServer::getSingletonPtr()->tick(timeSinceLastFrame);

...

}

Logic::CServer::getSingletonPtr()->deactivateMap();

...

Logic::CServer::getSingletonPtr()->unLoadLevel();

Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

Logic::CServer::Release();
\endcode

@author David Llansó
@date Septiembre, 2010
*/