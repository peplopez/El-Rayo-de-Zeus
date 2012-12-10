#the compute nodes on BlueGene/L don't support shared libs 
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)

SET(CMAKE_SHARED_LIBRARY_C_FLAGS "")            # -pic 
SET(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "")       # -shared
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")         # +s, flag for exe link to use shared lib
SET(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG "")       # -rpath
SET(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG_SEP "")   # : or empty

SET(CMAKE_LINK_LIBRARY_SUFFIX "")
SET(CMAKE_STATIC_LIBRARY_PREFIX "lib")
SET(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
SET(CMAKE_SHARED_LIBRARY_PREFIX "lib")          # lib
SET(CMAKE_SHARED_LIBRARY_SUFFIX ".a")           # .a
SET(CMAKE_EXECUTABLE_SUFFIX "")          # .exe
SET(CMAKE_DL_LIBS "" )

SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")


INCLUDE(Platform/UnixPaths)

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_LINK_EXECUTABLE
    "<CMAKE_C_COMPILER> -Wl,-relax <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES> -Wl,-lgcc,-lc -lnss_files -lnss_dns -lresolv")
ELSE(CMAKE_COMPILER_IS_GNUCC)
  # when using IBM xlc we probably don't want to link to -lgcc
  SET(CMAKE_C_LINK_EXECUTABLE
    "<CMAKE_C_COMPILER> -Wl,-relax <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES> -Wl,-lc -lnss_files -lnss_dns -lresolv")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_LINK_EXECUTABLE
    "<CMAKE_CXX_COMPILER> -Wl,-relax <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES> -Wl,-lstdc++,-lgcc,-lc -lnss_files -lnss_dns -lresolv")
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  # when using the IBM xlC we probably don't want to link to -lgcc
  SET(CMAKE_CXX_LINK_EXECUTABLE
    "<CMAKE_CXX_COMPILER> -Wl,-relax <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES> -Wl,-lstdc++,-lc -lnss_files -lnss_dns -lresolv")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
