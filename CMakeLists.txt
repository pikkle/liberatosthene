cmake_minimum_required(VERSION 3.15)
project(liberatosthene)

set(CMAKE_CXX_STANDARD 17)

add_library(common SHARED IMPORTED)
set_property(TARGET common PROPERTY IMPORTED_LOCATION ../libcommon)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS
        src/eratosthene.h
        src/eratosthene-address.h
        src/eratosthene-array.h
        src/eratosthene-class.h
        src/eratosthene-class-mono.h
        src/eratosthene-class-poly.h
        src/eratosthene-client.h
        src/eratosthene-door.h
        src/eratosthene-geodesy.h
        src/eratosthene-include.h
        src/eratosthene-server.h
        src/eratosthene-switch.h
        src/eratosthene-uv3.h
        )

set(SOURCES
        src/eratosthene.c
        src/eratosthene-address.c
        src/eratosthene-array.c
        src/eratosthene-class.c
        src/eratosthene-class-mono.c
        src/eratosthene-class-poly.c
        src/eratosthene-client.c
        src/eratosthene-door.c
        src/eratosthene-geodesy.c
        src/eratosthene-server.c
        src/eratosthene-switch.c
        src/eratosthene-uv3.c
        )

add_library(eratosthene STATIC ${SOURCES})
install(TARGETS eratosthene DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)