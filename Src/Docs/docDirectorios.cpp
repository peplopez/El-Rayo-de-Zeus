/**

@defgroup directorios Estructura de directorios

@brief <p/>

La estructura de directorios de GALEON es la siguiente:

<ul>
   <li> Dependencies: contiene
   todas las dependencias (librerías externas) que
   se necesitan para compilar GALEON. En este directorio
   está por ejemplo Ogre.

   El directorio está pensado para contener tanto
   las dependencias que <em>se deben compilar</em>
   (como por ejemplo Ogre), como las que no
   (como podrían ser las librerías del motor de física).

   Además de un directorio por cada librería que
   se debe compilar, tiene los subdirectorios include
   (donde están los ficheros de cabecera), lib (donde
   aparecen los ficheros con las librerías estáticas
   que pueden/deben usarse en la fase de enlazado)
   y bin (con las DLLs que deben estar accesibles
   a GALEON en el momento de ejecutarse).
   </li>

   <li> Doc: directorio pensado para tener ficheros relacionados
   con la documentación que no está contenida
   directamente en el código fuente.
   </li>

   <li> Exes: directorio donde el proceso de compilación
   deposita el ejecutable de Galeon. En este directorio
   se guardan todos los recursos (gráficos, sonoros, etc.)
   que utiliza el juego.

   Tener un directorio para los ejecutables permite
   compartir los ficheros de los recursos para distintas
   versiones del juego (versión Release, Debug).
   </li>

   <li> Projects: contiene los ficheros de proyecto de
   Visual Studio.
   </li>

   <li> Src: código fuente de Galeon.
   </li>

</ul>

En el directorio raiz, además, aparece el fichero con
la solución de Visual Studio de Galeon (se ha preferido
dejar en el directorio raíz por comodidad a la
hora de abrir el proyecto) y un script para
compilar Galeon desde cero automáticamente utilizando
Ant. Puedes ver más información sobre cómo compilar
Galeon en @ref compilando.




@author Marco Antonio Gómez Martín
@date Septiembre, 2010

*/