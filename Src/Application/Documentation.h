/**
@file Documentacion.h

Fichero "dummy" sin nada de código, utilizado únicamente para mantener 
documentación de Doxygen de la aplicación.

@see applicationGroup

@author David Llansó
@date Septiembre, 2010
*/

/**
@mainpage GALEÓN

GALEON es un pequeño juego desarrollado en C++ como ejemplo inicial
de proyecto para el 
<a href="http://www.videojuegos-ucm.es/">Máster en Desarrollo de Videojuegos UCM</a>.

El objetivo del proyecto es pedagógico, pues sirve de esqueleto
para la explicación y ejemplificación de los elementos más
importantes que componen un videojuego, y donde poner en
práctica los contenidos que se van explicando durante
el Máster.

Para eso, se ha prestado especial atención a la documentación
del código, accesible desde las páginas que estás leyendo.
De esta forma los contenidos que se ven por encima durante las
clases pueden terminar de entenderse a la vista del propio
código y su documentación.

No obstante, se ha evitado de manera consciente hacer un
esqueleto de juego <em>perfectamente escalable</em>. En este
sentido, GALEON incluye muchos elementos utilizados en
videojuegos más grandes, pero su implementación dista mucho
de ser la mejor para proyectos de envergadura.

De esta forma, los alumnos tendrán al alcance el código que
pone a funcionar las técnicas explicadas en clase, pero
se verán forzados (en la mayoría de los casos) a realizar
mejores implementaciones de las mismas si quieren hacer
un uso intensivo de ellas en sus propios proyectos de fin
de master.

En \ref compilando tienes información sobre cómo compilar
la aplicación. En \ref directorios se describe la estructura
de directorios del programa. Además, hay explicaciones de su
funcionamiento a alto nivel, que podrás encontrar en la sección
"Módulos" del árbol de la izquierda de la página.

*/

/**
@defgroup applicationGroup Aplicación de estados

En GALEÓN, la aplicación es una máquina de estados, que lleva el control del
bucle del juego, especializada para trabajar con el motor gráfico Ogre. La 
aplicación debe ser inicializada de forma explícita antes de ser lanzada; luego 
debe ser también liberada de manera explícita:
<p>

\code
Application::CGaleonApplication miApp;

if (miApp.init())
    miApp.run();

miApp.release();
\endcode

<p>
En su inicialización se encarga de inicializar los diferentes motores o 
servidores: subsistemas, gráfico, interfaz de usuario y lógico. Una vez lanzada 
se comienza un bucle infinito que actualiza los citados servidores (salvo el
lógico por ser dependiente del estado de juego) y el estado activo en la máquina 
de estados. Durante la ejecución de la aplicación se irán haciendo transiciones de
estados, por lo que el estado activo irá cambiando y así el comportamiento de la
aplicación.
<p>
Los diferentes estados tienen metodos de inicialización y destrucción explícita
que se invocan al añadir el estado a la aplicación o al ser liberado. Pero a su 
vez también tienen métodos de activación y desactivación que serán invocados cada
vez que el estado pase a ser o deje de ser el estado activo de la aplicación.
Como se ha dicho antes también implementan un método de actualización 
(Application::CBaseState::tick()) que se invocará en cada ciclo siempre que el 
estado esté activo.
<p>
Por otro lado, la aplicación también escucha eventos del teclado y el ratón, y los
delega de la misma manera en el estado activo.
<p>
El estado más importante es el estado de juego, que se encarga de cargar el nivel
de juego que se va a jugar y de invocar periodicamente al tick() del motor de
lógica.

@author David Llansó
@date Septiembre, 2010
*/