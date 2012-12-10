/**
@file Documentacion.h

Fichero "dummy" sin nada de código, utilizado únicamente para mantener 
documentación de Doxygen de los subsistemas base.

@see baseSubsystemsGroup

@author David Llansó
@date Septiembre, 2010
*/

/**
@defgroup baseSubsystemsGroup Subsistemas base

Dentro del grupo de subsistemas base se encuentran la inicialización y 
gestión de los diferentes motores necesarios para correr la aplicación así
como una seríe de herramientas auxiliares usadas por el proyecto. 
<p>
Su misión principal es la de inicializar los subsistemas de Ogre, OIS, etc. 
y hacerlos accesibles al resto de proyectos de la solución. De esta manera
se ocultan los detalles engorrosos de la creación y destrucción de los 
recursos necesarios para que luego el resto de proyectos de la solución 
puedan acceder a ellos de forma limpia. Su otra función es la de ofrecer
una sería de funciones y estructuras con las que trabajar por el resto de 
proyectos.
<p>
Para activar su funcionalidad basta con inicializar el servidor central del
grupo. Se trata de un servidor de inicialización y destrucción explícita por 
lo que habrá que llamar a <code>BaseSubsystems::CServer::Init()</code> y
<code>BaseSubsystems::CServer::Release()</code>.
<p>
Es importante recalcar que este servidor debe de ser inicializado <b>antes</b>
que el resto ya que varios de ellos necesitan pedir recursos a éste en sus
inicializaciones.

@author David Llansó
@date Septiembre, 2010
*/