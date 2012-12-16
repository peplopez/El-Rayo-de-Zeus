/**
@file Documentacion.h

Fichero "dummy" sin nada de código, utilizado únicamente para mantener 
documentación de Doxygen de la parte de las entidades de la lógica de 
juego.

@see entityGroup

@author David Llansó
@date Septiembre, 2010
*/

/**
@defgroup entityGroup Entidades en la lógica de juego

La gestión de entidades realizada en la capa lógica (ver @ref logicGroup) utiliza
una arquitectura de componentes. Gracias a esto, hay únicamente una clase
que implementa el concepto de "entidad", la clase Logic::CEntity que no
es más que un contenedor de componentes (Logic::IComponent) que son los que definen
el comportamiento total de la entidad.

La construcción de entidades se basa en la existencia de lo que se llaman
ficheros de "blueprint" que contienen, para cada entidad, qué componentes
definen su comportamiento. Por ejemplo la entidad "Player", que representa
al jugador, está definida como:

\code
Player CAvatarController CAnimatedGraphics
\endcode

Lo que viene a indicar que la entidad es simplemente un componente de control
(para que pueda ser controlado) y uno gráfico (para que se dibuje).

El mapa después lo único que tiene que hacer es declarar la lista de
entidades que contiene y sus propiedades. En la carga del mapa se van creando
los componentes e inicializándolos con esas propiedades leídas del mapa (como
la posición inicial, o el modelo gráfico a dibujar).

Los componentes se intentan hacer lo más general posible, para que sirvan
para definir el comportamiento de distintos tipos de entidades. En muchas
ocasiones el comportamiento final se termina de configurar gracias a esos
atributos. Por ejemplo, el componente gráfico sirve para dotar a las
entidades de cuerpo en el mundo gráfico, ya sea este estático o dinámico.
Las distintas propiedades o atributos leídos del mapa configurarán el 
componente para que refleje el tipo de entidad gráfica que se desea.

El funcionamiento de los componentes se basa en la existencia del método
Logic::IComponent::tick(), donde los componentes particulares pueden
ejecutar sus operaciones. Normalmente las operaciones consistirán en el
procesado de mensajes recibidos y/o alguna otra operación necesaria.

@section grupoComponentesRegistro Registrando un componente nuevo

Para que el juego sea capaz de crear cada componente particular a partir
de su nombre se necesita una factoría de componentes y que cada clase
que implementa uno de ellos se registre en la misma.

La clase Logic::ComponentFactory es la responsable de esa creación. Es un 
singleton donde se registran todos los componentes utilizando el método 
Logic::ComponentFactory::add. Ese método recibe el nombre del componente y 
una función global (o estática) para crear un objeto de ese tipo. En el 
momento de crear una entidad, se utiliza el fichero de blueprints para saber 
qué componentes se necesitan y se utiliza la factoría para crearlos. 

Sin embargo, para facilitar el desarrollo de nuevos componentes, el fichero 
Logic/Entity/IComponent.h define una serie de macros del preprocesador que 
pueden/deben utilizarse. En conreto, para tener un componente funcionando 
basta con:

<ul>
	<li>Utilizar la macro DEC_FACTORY en la parte de declaración de la nueva
		clase para que se añadan ciertas funciones estáticas útiles para el registro
		en la factoría.
	<li>Utilizar la macro IMP_FACTORY en la parte de implementación para
		definir el código de esas funciones.
	<li>Utilizar la macro REG_FACTORY tras la declaración de la nueva clase
		para que se registre el componente en la factoría utilizando los métodos
		declarados y definidos con las macros anteriores.
	<li>Hacer una inclusión del archivo cabecera del nuevo componente en algún
		fichero .cpp del proyecto que sepamos que va a ser procesado para que la 
		macro REG_FACTORY registre el nuevo componente en la factoría.
</ul>

Como ejemplo, si queremos construir un nuevo componente <code>CMyComponent</code>, 
el inicio del fichero de cabecera sería algo así:

\code
// File MyComponent.h
#include "Logic/Entity/IComponent.h"

class CMyComponent : public Logic::IComponent 
{
   DEC_FACTORY();

public:
   ...
};
REG_FACTORY(CMyComponent);
\endcode

y en el fichero de implementación:

\code
// File MyComponent.cpp
#include "MyComponent.h"

IMP_FACTORY(IComponent);
...
\endcode

Por último, para poder utilizar el componente (mejor dicho para que durante
la carga de un mapa el motor sea capaz de crearlo), se debe registrar
en la factoría. Hacemos la inclusión de la cabecera en cualquier fichero .cpp 
del proyecto que sepamos que va a ser procesado. Las inclusiones de los 
componentes iniciales se han realizado en Logic/ComponentFactory.cpp:

\code
// Fichero XXX.cpp
#include "MyComponent.h"
...
\endcode

@section grupoComponentesImplementacion Implementando un componente nuevo

Una vez que se tiene claro cómo crear un nuevo componente para que pueda
ser utilizado dentro de una entidad, podemos pasar a implementar la
funcionalidad que deseemos que realice.

El componente más sencillo (e inútil) es el implementado en la
sección anterior: aquel que simplemente hereda de Logic::IComponent.
No sirve para mucho, pero es un comienzo: el motor del juego lo reconoce
como componente y se puede utilizar dentro de las entidades.

A continuación iremos extendiendo ese componente para ejemplificar las
características básicas que deben conocerse para implementar un componente
serio. Por comodidad el código presentado asume que se está en el fichero
de cabecera, aunque en un desarrollo serio tendríamos la mayor parte
de la implementación en un fichero .cpp.

@subsection noLabel1 Leyendo propiedades

Nuestro componente será creado cuando en el mapa se tenga una entidad
que lo necesite. Esa entidad tendrá a buen seguro una lista de atributos
que podrán ser leídos por los componentes para terminar de definir
sus comportamientos.

En el momento de la creación del componente, el motor invoca al método
Logic::IComponent::spawn() al que le pasa, entre otras cosas, un objeto
de donde leer todos los atributos de la entidad.

El método devuelve un booleano para poder notificar condiciones de error;
en caso de ser imposible la inicialización del componente, se devuelve
<code>false</code> y el motor anula la carga del mapa.

Es <em>importante</em> invocar al método spawn de la clase padre al
principio del método, para que la inicialización sea completa. El
siguiente ejemplo comprueba si existe el atributo "activateMsg" en el mapa
y si existe lo guarda en una variable. Para poder acceder a los atributos 
leídos del mapa se debe incluir el fichero Map/MapEntity.h.

\code
class CMyComponent : public Logic::IComponent 
{
   DEC_FACTORIA();

public:

   CTestComponent() : _activateMsg("Map activated or deactivated") {}

   virtual bool spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
   {
      if( !IComponent::spawn(entity,map,entityInfo) )
         return false;

      if (entity->existsKey("activateMsg"))
         _activateMsg = entity->getStringAtributo("activateMsg");

      return true;
   }

protected:

   std::string _activateMsg;
};
REG_FACTORY(CMyComponent);
\endcode

@subsection noLabel2 Activación y desactivación

Una entidad pertenece siempre a un mapa (o nivel) del juego. Un mapa al
cargarse se encuentra inactivo, no recibe atención por parte del motor
de juego y por tanto no se renderizan sus objetos ni se actualiza su lógica.

Cuando un mapa es activado y desactivado, todas sus entidades son informadas 
para que puedan realizar las tareas oportunas (como por ejemplo la entidad 
del jugador debe registrarse en el servidor lógico para que quien lo requiera 
pueda tener acceso al jugador).

Los componentes pueden también ser conscientes de esta activación/desactivación
sobreescribiendo los métodos Logic::IComponent::activate() y 
Logic::IComponent::deactivate() ya que la entidad invocará esos métodos de 
todos sus componentes. Por ejemplo Logic::CAvatarController se registra 
en los eventos de teclado para manejar al jugador si el componente pertenece
a la entidad del jugador.

El siguiente código muestra como el componente que vamos definiendo escribe la 
cadena leida en el método Logic::IComponent::spawn() cuando el mapa en el que 
se encuentra su entidad es activado y desactivado.

\code
class CMyComponent : public Logic::IComponent 
{
   DEC_FACTORIA();

public:
   ...
   
   virtual bool activate() 
   {
      std::cout << _activateMsg << std::endl;
      return true;
   }

   virtual void deactivate() 
   {
      std::cout << _activateMsg << std::endl;
   }
   ...
};
REG_FACTORY(CMyComponent);
\endcode

@subsection noLabel3 Recibiendo mensajes

El funcionamiento básico de un componente es recibir mensajes de
otros componentes y reaccionar ante ellos. Para eso el motor proporciona
al Logic::IComponent un mecanismo de recepción de mensajes para poder
hacer cosas con ellos.

La recepción está separada en dos fases. Primero el motor <em>pregunta</em>
al componente si acepta el mensaje enviado, es decir, si para él ese
mensaje tiene algún tipo de información útil. En caso afirmativo,
el motor invocará en un momento posterior al método para procesar el mensaje
y que el componente ejecute las acciones que considere oportunas en base
a la información recibida.

Por lo tanto, para ser capaces de recibir mensajes, los IComponent deben 
sobreescribir dos métodos: el Logic::IComponent::acept y 
Logic::IComponent::process.
En ambos casos se recibe como parámetro el mensaje. Para saber de qué tipo es
habrá que consultar el atributo Logic::TMessage::_type.

Para la implementación de los mensajes se ha optado conscientemente por una 
implementación sencilla que con un tipo de mensaje genérico Logic::TMessage
que tiene una serie de atributos también genéricos y en función del tipo 
(Logic::TMessage::_type) de mensaje se usarán unos u otros. Esta aproximación 
es sencilla, pero no es óptima desde el punto de vista de rendimiento.
Para llevar a cabo un proyecto serio esta implementación <b>debe</b> ser 
reconsiderada.

El siguiente código muestra la implementación de los dos métodos en
el componente CTestComponent que nos está sirviendo de prueba,
para que interprete los mensajes enviados a la entidad cuando ésta
cambia de posición, y escribe por pantalla la nueva posición (para que
funcione habrá que incluir el fichero Logic/Entity/Message.h).

\code
class CMyComponent : public Logic::IComponent 
{
   DEC_FACTORIA();

public:
   ...

   bool accept(const TMessage &message)
   {
      return message._type == Message::SET_POSITION;
   }

   void process(const TMessage &message)
   {
      switch(message._type)
      {
      case Message::SET_POSITION:
         std::cout << "Moved to(" << message._position.x << ", " 
                   << message._position.y << ", " 
                   << message._position.z << ")\n";
         break;
      }
   }
   ...
};
REG_FACTORY(CMyComponent);
\endcode

El funcionamiento interno del motor ayudará a entender los siguientes
apartados. En algún momento una entidad externa puede enviar un
mensaje a la entidad a la que pertenecemos. La entidad al recibirlos
lo que hace es preguntar uno a uno a todos sus componentes para averiguar
cuáles de ellos están interesados en el mensaje, o lo que es lo mismo,
para qué componentes el mensaje resulta útil. Si un componente <em>acepta</em>
un mensaje, éste se <em>encola</em> en la lista de mensajes a procesar
del componente.

Posteriormente, en la fase de simulación de la lógica, se concederá
tiempo de CPU al componente para que procese todos los mensajes encolados.
Eso es realizado por el método Logic::IComponent::processMessages() que
recorre la cola y va llamando sucesivamente al método Logic::IComponent::process()
sobreescrito en las implementaciones concretas de cada componente.

@subsection noLabel4 Ejecutando lógica

En general las entidades de un juego implementan la lógica usando pequeñas 
ejecuciones de un método que suele llamarse algo así como "tick()" o "update()". 
El motor del juego llama regularmente a ese método (por ejemplo en cada frame), 
y espera que ejecute una mínima parte de su comportamiento. En una arquitectura 
de componentes, la entidad lo que hace es llamar al método "tick()" o "update()"
de todos sus componentes, y estos ejecutarán ahí la parte de su comportamiento.

Aunque en el componente que hemos implementado hasta ahora no ha aparecido ningún 
método de este tipo, en realidad sí existe, declarado en Logic::IComponent::tick(). 
La implementación por defecto de ese método lo que hace es invocar al método
Logic::IComponent::processMessages() que terminará llamando al sobreescrito 
Logic::IComponent::process() con todos los mensajes aceptados por 
Logic::IComponent::accept().

\code
class IComponent 
{
public:
   ...

   bool IComponent::tick()
   {
      processMessages();
      return false;
   }
   ...
};
\endcode

Si nuestro componente ejecuta otro tipo de lógica de manera autónoma,
sobreescribremos el método Logic::IComponent::tick() con la implementación 
de nuestro nuevo comportamiento. De la explicación anterior se deduce 
que si un componente sobreescribe el método es <em>indispensable</em>
que llame a la implementación del método de la clase padre (o al menos
al método Logic::IComponent::processMessages()). Si se olvida hacer esto,
el componente <em>nunca</em> procesará los mensajes recibidos.

El siguiente podría ser un tick() que presenta un simple texto en pantalla.
Si se devuelve true implica que todo ha ido bien:

\code
class CMyComponent : public Logic::IComponent 
{
   DEC_FACTORIA();

public:
   ...

   virtual bool tick() 
   {
      // Importante no olvidar esto...
      IComponent::tick();

      std::cout << "I'm alive!" << std::endl;
      return true;
   }
   ...
};
REG_FACTORY(CMyComponent);
\endcode

\code
\endcode

Habitualmente, las entidades de los motores de juegos (y sus componentes)
tienen la posibilidad de "dormirse" de forma que el motor no pierde el 
tiempo en la invocación a su método de actualización para no consumir 
ciclos innecesarios cuando la entidad y los componentes no tienen taréas
que realizar en los siguiente ciclos. En esta aproximación no ha sido 
incluida esta habilidad de manera intencionada, pero quien pretenda 
reutilizar esta arquitectura debería plantearse incluir esta cualidad.

@author David Llansó, Marco Antonio Gómez Martín
@date Septiembre, 2010
*/