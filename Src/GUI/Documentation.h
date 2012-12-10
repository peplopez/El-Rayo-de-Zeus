/**
@file Documentacion.h

Fichero "dummy" sin nada de código, utilizado únicamente para mantener 
documentación de Doxygen de la parte de interfaz de usuario.

@see GUIGroup

@author David Llansó
@date Septiembre, 2010
*/

/**
@defgroup GUIGroup Interfaz de usuario

Como interfaz de usuario englobamos todo aquello que sirve para informar 
de manera explícita al usuario y las herramientas con las que el usuario
se comunica con el juego.
<p>
Para mostrar al usuario información que va más allá de la partida en si
nos apoyamos en CEGui. CEGui nos permite mostrar gráficos 2D sobre el 
reenderizado de la partida y no está atado a Ogre. CEGui nos permitirá
crear menús o mostar cuadros con información necesaria para el usuario
durante la partida (vida, siguiente misión, etc.). En este módulo se 
gestionará CEGUI para que la interfaz gráfica de usuario sea correcta.
<p>
Para la comunicación del usuario con el juego usamos OIS que nos permite
gestionar la entrada de periféricos tradicionales (ratón y teclado). Se 
ha creado un gestor de periféricos de entrada que controla y captura los 
eventos que se producen en teclado y ratón basado en OIS, pero que a su
vez nos sirve para independizar al resto de la aplicación de esta 
biblioteca en concreto.
<p>
El gestor permite registrar oyentes que serán informados de los eventos 
de entrada producidos, así cualquier clase que necesite tener acceso a
los eventos deberá implementar los métodos de las clases oyentes 
(GUI::CKeyboardListener o GUI::CMouseListener) y registrarse en el gestor.
Este gestor debe ser inicializado al principio de la aplicación ya que
hasta la propia aplicación debe registrarse para escuchar eventos de
entrada.
<p>
Para la comunicación del resto de los proyectos con la parte de interfaz
de usuario hay un servidor (GUI::CServer). Además, hay una clase específica
(GUI::PlayerController) para el control del jugador mediante teclado y 
ratón. Esta clase transformará las pulsaciones de teclas y movimiento del 
ratón en acciones concretas de movimiento o giro y se comunicarán 
directamente a una instancia de Logic::CAvatarController que debe 
registrarse mediante el servidor.

@author David Llansó
@date Septiembre, 2010
*/