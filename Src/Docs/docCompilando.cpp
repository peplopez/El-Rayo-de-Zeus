/**

@defgroup compilando Compilacion de Galeon

@section introCompilando Introducción

La compilación de Galeon desde cero requiere la
ejecución de un gran número de pasos. Para evitar
la tarea tediosa de realizarlos manualmente,
Galeon dispone de un script para Ant que
se encarga de hacerlo automáticamente.

Antes de poder utilizarlo, no obstante, es
imprescindible comprobar que en la máquina
donde se pretende compilar Galeon están
instalados ciertos prerequisitos.

La sección siguiente detalla cuáles son
esos requisitos y cómo conseguirlos. Posteriormente
se explica cómo compilar Galeon una vez instalados.
La última sección describe cómo sería el proceso
de compilación manual sin la utilización de Ant.
Aunque está altamente desaconsejado utilizar
ese método, la sección es interesante por sí misma
porque en la práctica lo que hace es describir
todo lo que el script de Ant hace por nosotros,
algo que puede ayudar a entender mejor la estructura
de directorios de Galeon.

@section requisitos Requisitos de Galeon

Para poder trabajar con Galeon es preciso que la máquina
tenga instalado lo siguiente:

<ul>
   <li>Visual Studio 2010.</li>

   <li>DirectX: es necesario tener instaladas las SDK
   de DirectX, que incluyen los ficheros de cabecera
   y bibliotecas necesarios para poder compilar el
   motor gráfico. Las SDK se pueden obtener en la página
   de Microsoft. Es importante instalarlas <em>después</em>
   de la instalación de Visual Studio, pues el
   instalador altera (mínimamente) la configuración
   del IDE para que encuentre los ficheros al compilar.

   Por último, hay que asegurarse de que tras la
   instalación se queda establecida la variable de
   entorno <tt>DXSDK_DIR</tt> al directorio raiz
   de la instalación.</li>

   <li>Ant: esta herramienta de Apache permite la compilación
   de Galeon en un solo paso. Sólo es necesario si
   se opta por este tipo de generación (¡altamente
   recomendable!). Ant se puede descargar de http://ant.apache.org/.

   Como añadido, para poder llamar a Visual Studio
   desde los script de Ant es necesario extender la
   instalación de Ant. Para eso, hay que descargar
   <a href="http://gaia.fdi.ucm.es/people/marcoa/development/vcTasks">
   vcTask</a> y copiar el .jar al directorio lib
   de la instalación.</li>
</ul>

	Opcionalmente también puedes instalar:

<ul>
   <li>Doxygen: si planeas generar la documentación de Galeon.
   Está disponible <a href="http://www.stack.nl/~dimitri/doxygen/">aquí</a>.

   En ese caso, también puede que te interese añadir a Ant la posibilidad
   de generar la documentación directamente, instalando
   la tarea disponible <a href="http://ant-doxygen.blogspot.com/">aqui</a>.</li>

   Puedes encontrar más información en la sección \ref generarDocumentacion
</ul>

@section usoDeAnt Compilación automática con Ant

Para compilar Galeon automáticamente, basta con ejecutar Ant desde
el directorio raíz de Galeon.

<pre>
c:\Galeon> ant
Buildfile: build.xml

debug:

llamaBuilds:

debug:

llamaBuilds:

debug:

compilaSln:

windowsbuilder:
      [vc10] Command= C:\...\\IDE\devenv.com /out c:\\Galeon/vslog.txt ...
      [vc10] 
      [vc10] Microsoft (R) Visual Studio 10.0.30319.1.
      [vc10] (C) Microsoft Corp. Reservados todos los derechos.
      [vc10] 1>------ Operaci¢n Generar iniciada: proyecto: zlib, configuraci¢n: Debug Win32 ------
      [vc10] 2>------ Operaci¢n Generar iniciada: proyecto: ZLib-freeImage, configuraci¢n: Debug Win32 ------
      [vc10] 2>Compilando...
      [vc10] 1>Compilando...
      [vc10] 2>adler32.c
      [vc10] 1>compress.c
</pre>

Después de mucho rato y más de 3GB menos en el disco duro, tanto
la versión de Debug como la de Release de Galeon están generadas
en el directorio Exes.

@section compilacionManual Compilación manual

Lo primero que se debe hacer es compilar las dependencias adicionales
que tiene Galeon, que se encuentran en el directorio dependencies:

<ol>
   <li>Compilar las dependencias de Ogre: para eso, abrir
   el proyecto en Visual Studio 2010 (<tt>dependencies/OgreDependencies/OgreDependencies.VS2008.sln</tt>),
   y generar las versiones Debug y Release (lo más fácil tanto en
   este como en el resto de proyectos es usar el menú Generar - Compilación por lotes -
   Seleccionar todo - Generar
   </li>

   <li>Generar los proyectos de Ogre: la distribución
   de Ogre NO incluye los proyectos de Visual Studio, sino
   que hay que generarlos con una aplicación aparte (CMake).
   Para evitar complicaciones, la aplicación está en
   el directorio dependencies, por lo que no hay que instalarla.

   Para generarlos, se debe crear el directorio <tt>build</tt>
   dentro de la distribución de ogre, y luego invocar a
   CMake (el comando de ejecución de CMake es algo largo,
   pues se desactiva la compilación de bastantes componentes
   que Galeon no utiliza):

   <pre>
c:\Galeon\dependencies\Ogre> mkdir build
c:\Galeon\dependencies\Ogre> cd build
c:\Galeon\dependencies\Ogre\build> ..\\CMake\bin\cmake.exe -DOGRE_DEPENDENCIES_DIR=..\.. -DOGRE_BUILD_SAMPLES=FALSE -DOGRE_BUILD_TOOLS=FALSE -DOGRE_BUILD_COMPONENT_PAGING=FALSE -DOGRE_BUILD_COMPONENT_TERRAIN=FALSE -DOGRE_BUILD_PLUGIN_PCZ=FALSE -DOGRE_BUILD_PLUGIN_OCTREE=FALSE -DOGRE_BUILD_PLUGIN_BSP=FALSE -DOGRE_INSTALL_TOOLS=FALSE ..
-- Building for: Visual Studio 10 2010
-- Check for working C compiler: cl
-- Check for working C compiler: cl -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working CXX compiler: cl
...
-- Configuring done
-- Generating done
-- Build files have been written to: c:/Galeon/dependencies/Ogre/build
    </pre>
   </li>

   <li>Compilar Ogre. Para eso se abre la solución <tt>OGRE.sln</tt>
   del directorio <tt>build</tt> y se compilan todos los proyectos
   que tienen.
   </li>

   <li>Copiar los ficheros resultantes de la compilación de
   Ogre donde los espera Galeon. Los ficheros de inclusión
   de Ogre deben copiarse a <tt>dependencies/include</tt>,
   las bibliotecas estáticas a <tt>dependencies/lib</tt> y
   las DLLs y otros ficheros utilizados durante la ejecución
   irán a <tt>dependencies/bin</tt>. Más concretamente:
	<ol>
	   <li>Crear el directorio <tt>dependencies/include/OGRE</tt>
	   y copiar en él el contenido del directorio
	   <tt>dependencies/OGRE/OgreMain/include</tt> y el
	   de <tt>dependencies/OGRE/build/include</tt>.</li>

	   <li>Copiar al directorio <tt>dependencies/lib/Debug</tt> todos
	   los .lib que se hayan generado en <tt>build/lib/Debug</tt>. Hacer
	   lo mismo con los Release.</li>

	   <li>Copiar al directorio <tt>dependencies/bin/Debug</tt>
	   las DLLs que se habrán generado en <tt>build/bin/Debug</tt>, así
	   como los ficheros con extensión <tt>.cfg</tt> (en la práctica
	   ni resources.cfg ni samples.cfg ni quakemap.cfg son
	   necesarios). Hacer lo mismo con las correspondientes versiones
	   de Release.</li>
	</ol>
   </li>

   <li>Generar la solución de Visual Studio de CEGUI. Para eso, basta
   con ejecutar el fichero <tt>dependencies/CEGUI/projects/premake/build_vs2008.bat</tt>.
   </li>

   <li>Convertir a Visual Studio 2010 abriendo la solución generado con 
   Visual Studio 2010.
   </li>

   <li>Compilar todos los proyectos de la solución CEGUI.sln que
   se habrá creado en ese mismo directorio.
   </li>

   <li>Igual que con Ogre, copiar los resultados de la compilación
   a los directorios include, lib y bin:
	   <ol>
		<li>Crear el directorio <tt>cegui</tt> dentro de
		<tt>dependencies/include</tt> y copiar en él 
		el directorio <tt>dependencies/cegui/include</tt>
		(en la copia se pueden omitir los ficheros con
		extensión *.in y *.am).
		</li>

		<li>Las bibliotecas compiladas habrán quedado en
		<tt>dependencies/cegui/lib</tt>; las que terminan
		en <tt>_d</tt> son las versiones Debug que habrá
		que copiar a <tt>dependencies/lib/Debug</tt>, mientras
		que las demás son las versiones en Release. Copiar
		a cada uno de ellos los ficheros CEGUIBase,
		CEGUIExpatParser, CEGUIFalagardWRBase, CEGUIOgreRenderer
		y CEGUISILLYImageCodec, que son los componentes de CEGUI
		que utiliza Galeon.
		</li>

		<li>Por su parte, las DLLs se habrán quedado en
		<tt>dependencies/cegui/bin</tt> y siguen la misma
		nomenclatura. Hay que copiar a <tt>dependencies/bin/Debug</tt>
		las DLLs de depuración de las bibliotecas listadas anteriormente
		y a <tt>dependencies/bin/Release</tt> las versiones
		Release. También habrá que copiar una última DLL
		que CEGUI tiene de dependencia que se encuentra en
		<tt>dependencies/CEGUI/dependencies/bin</tt> llamada
		SILLY (y la correspondiente SILLY_d).
		</li>
	   </ol>
   </li>

   <li>Compilar el proyecto Visual Leak Detector de la solución vld.sln 
   que se encuentra en <tt>dependencies/VisualLeakDetector/src</tt>.
   </li>

   <li>Igual que con Ogre y CEGUI, hay que copiar los resultados de la compilación
   a los directorios include, lib y bin:
	   <ol>
		<li>Copiar los ficheros <tt>vld.h</tt> y <tt>vld_def.h</tt>, que
		se encuentran en <tt>dependencies/VisualLeakDetector/src</tt>, 
		dentro de <tt>dependencies/include</tt>
		</li>

		<li>Copiar la biblioteca <tt>vld.lib</tt>, que
		se encuentran en <tt>dependencies/VisualLeakDetector/bin</tt>, solo a 
		<tt>dependencies/lib/Debug</tt> ya que en Release no se necesita
		VLD.
		</li>

		<li>Por su parte, la DLL habrá quedado en
		<tt>dependencies/VisualLeakDetector/bin</tt> y se llamará <tt>vld_x86.dll</tt>
		o <tt>vld_x64.dll</tt> en función de la arquitectura del ordenador 
		(32 o 64 bits). Hay que copiarla a <tt>dependencies/bin/Debug</tt>.
		También habrá que copiar una última DLL (<tt>dbghelp.dll</tt>), de la que 
		Visual Leak Detector tiene dependencia, en el mismo directorio. En función 
		de la arquitectura del ordenador (32 o 64 bits) se deberá coger de 
		<tt>dependencies/VisualLeakDetector/bin/Win32</tt> o de
		<tt>dependencies/VisualLeakDetector/bin/Win64</tt>.
		</li>
	   </ol>
   </li>

   <li>El último paso, compilar Galeon. Para eso se abre la
   solución Galeon.sln del raíz, y se compilan todos sus
   proyectos. Al final, los ejecutables de la versión
   de depuración y release habrán quedado en el directorio <tt>Exes</tt>.
   No obstante, para que se puedan ejecutar las versiones Release
   y Debug de Galeón, se deberá mover el contenido de 
   <tt>dependencies/bin/Release</tt> y <tt>dependencies/bin/Debug</tt>
   al directorio <tt>Exes</tt>.
   </li>

</ol>

@section generarDocumentacion Generación de la documentación

Como ya se ha comentado previamente, la generación de la documentación
hace uso de la herramienta <a href="http://www.stack.nl/~dimitri/doxygen/">Doxygen</a>.

Es posible lanzarla directamente desde el directorio de Galeón:

<pre>
$GALEON\> doxygen
</pre>

La documentación se creará en el directorio <tt>DocumentacionDoxygen</tt>.

Si se instala en Ant la tarea <a href="http://ant-doxygen.blogspot.com/">ant-doxygen</a>
(copiando el fichero <tt>.jar</tt> en la <tt>$ANT_PATH\lib</tt>), también será
posible generarla mediante:

<pre>
$GALEON\> ant doxygen
</pre>

En este caso, la salida de doxygen no se mostrará, lo que en ocasiones
puede acelerar el proceso a costa de no ver posibles errores y avisos.

@author Marco Antonio Gómez Martín, David Llansó
@date Septiembre, 2010

*/
