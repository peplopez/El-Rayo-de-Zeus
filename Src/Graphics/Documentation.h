/**
@file Documentacion.h

Fichero "dummy" sin nada de código, utilizado únicamente para mantener 
documentación de Doxygen de la parte gráfica.

@see graphicsGroup

@author David Llansó
@date Septiembre, 2010
*/

/**
@defgroup graphicsGroup Gráficos

Grupo de clases que sirven para pintar las entidades gráficas en la ventana
de reenderizado y sirven como interfaz de Ogre para el resto de los 
proyectos.
<p>
Además de ocultar el motor gráfico usado al resto de proyectos, sus clases
ofrecen un uso de más alto nivel, de manera que solucionan parte de la gestión
necesaria cuando se crean entidades, cámaras o escenas.
<p>
El eje central es el servidor, que se trata de un singleton de inicialización 
y destrucción explícita por lo que habrá que llamar a 
<code>Graphics::CServer::Init()</code> y <code>Graphics::CServer::Release()</code>
para su inicialización y liberación.
<p>
Los diferentes elementos gráficos se organizan en escenas (CScene), que contiene
listas con las entidades gráficas que se pintarán y una cámara que refleja desde
donde se desea visualizar la escena. Las entidades gráficas deben estar asociadas 
a una escena y de hecho, los recursos Ogre no se cargan hasta que la entidad no se
asocia a una escena. Diferenciamos las entidades gráficas en tres grupos distintos:
dinámicas, estáticas y animadas.
<p>
Las entidades dinámicas (<code>Graphics::CEntity</code>) son aquellas que representan
objetos que pueden cambiar de posición, orientación, escala, etc. Las estáticas
(<code>Graphics::CStaticEntity</code>) son aquellas que no van a ser alteradas 
durante toda la partida (suelos, paredes, etc.) y no pueden ser modificadas una
vez que se ha activado la escena por primera vez. El beneficio de estas entidades 
es que consumen menos recursos para ser pintadas, por lo que si se sabe que una 
entidad no va a ser alterada conviene que sea estática. Por último, las entidaes
animadas (<code>Graphics::CAnimatedEntity</code>) son entidades dinámicas que además
pueden reproducir animaciones.
<p>
El orden de uso de los diferentes elementos viene definido en el siguiente marco:
<p>

\code
if (!Graphics::CServer::Init())
    return;
...

Graphics::CScene scene = Graphics::CServer::getSingletonPtr()->createScene("escena");

...

Graphics::CEntity entity = new Graphics::CEntity("entidad","modelo.mesh");
if(!scene->addEntity(entity))
	return;

entity->setPosition(Vector3::ZERO);
entity->setYaw(Math::PI);

...

scene->removeEntity(entity);
delete entity;

...

Graphics::CServer::getSingletonPtr()->removeScene(scene);

...

Graphics::CServer::Release();
\endcode

@author David Llansó
@date Septiembre, 2010
*/