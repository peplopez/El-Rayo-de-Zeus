Para modificar la gramática del mapa modificar los archivos scanner.ll y parser.yy
y a partir de ahí, mediante flex y bison en Linux o con CygWin, generar los archivos 
Parser.h, Parser.cpp y Scanner.cpp con:

bison --defines=Parser.h -o Parser.cpp Parser.yy
flex -oScanner.cpp Scanner.ll

También se generarán location.hh, position.hh y stack.hh. Mover los 6 ficheros
generados a ./Src y volver a generar el proyecto.