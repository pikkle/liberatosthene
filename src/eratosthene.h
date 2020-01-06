/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

    /*! \file   eratosthene.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene
     */

    /*! \mainpage liberatosthene
     *
     *  \section overview Overview
     *
     *  The _liberatosthene_ library implements an earth scale 4D tile server
     *  through a dedicated multi-scale space-time indexation formalism. Using
     *  the theoretical framework of the index, it implements a simple and
     *  reliable 4D geographical data storage and access system. This allows
     *  software developed using this library to perform large-scale data
     *  injection and browsing with high efficiency over the network to provide
     *  remote 4D geographic services and information systems. Initially
     *  designed for point-based data, the indexation formalism and the library
     *  implementation allow now to manage polygonal representations along with
     *  large scale point-based models to create a rich 4D model of the earth.
     *
     *  \section project Eratosthene Project
     *
     *  The Eratosthene Project was initially designed to address the challenge
     *  of very large point-based models of cities and environments. Indeed,
     *  mapping a whole city at the level of the centimeter induces very large
     *  and heavy models that are difficult to manipulate, explore and exploit.
     *  Moreover, considering the time dimension implies a quick increase of the
     *  size of the models, making them impossible to handle as a single and
     *  monolithic object.
     *
     *  The Eratosthene Project is designed to provide a solution by considering
     *  a space-time indexation formalism. This indexation formalism is used to
     *  implement a server able to manage, store and broadcast over the network
     *  very large 4D models distributed all over the earth surface and covering
     *  large periods of time. In addition to the server, the Eratosthene
     *  Project also implements a graphical client able to communicate with the
     *  remote servers to offer a convenient way to browse the models in both
     *  space and time.
     *
     *  The space-time indexation formalism is designed to efficiently manage
     *  the storage of the incoming 4D models and to offer a simple and
     *  efficient way of addressing queries to the server to allow models
     *  broadcasting. The space-time indexation also allows to perform queries
     *  to the server in an intuitive way giving the possibility to both address
     *  the challenge of the earth structures level of details and time
     *  management. This allows to explore the entire earth surface in terms of
     *  4D structures from large mountain chains in 2000 to very small details
     *  of cities in 1850.
     *
     *  The subsequent indexation formalism used to drive the storage and access
     *  of the data also naturally brings a standardised discretisation of space
     *  and time. Point-based models injected in the Eratosthene information
     *  system are then standardised from both spatial and temporal point of
     *  views. This allows the implementation of binary and logical operators
     *  when considering a model at two different times. The operators allow to
     *  deduce a third model on the fly providing a differential point of view
     *  of the two models separated in time. The evolution of cities and
     *  environments can be enhanced and analysed in an efficient way, opening
     *  the way to more advanced geographic services.
     *
     *  The ability of indexation formalism to consider polygonal models in
     *  addition to large scale point-based models allows to enhance the
     *  possibilities in terms of 4D geographic services. More than the
     *  possibility to automatically manage the polygonal models level of
     *  details and large scale display, the indexation formalism also allows
     *  compositions on the fly of mixed models allowing to compare different
     *  type of models, from lidar point-based models to land registers.
     *
     *  \section license Copyright and License
     *
     *  **liberatosthene** - Nils Hamel <br >
     *  Copyright (c) 2016-2020 DHLAB, EPFL
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_ERATOSTHENE__
    # define __LE_ERATOSTHENE__

    # ifndef __cplusplus
    # if ( __STDC_VERSION__ < 201112L )
    # error "C11 standard required"
    # endif
    # endif
    # include <limits.h>
    # if ( CHAR_BIT != 8 )
    # error "8 bits bytes required"
    # endif
    # if defined ( __BYTE_ORDER__ )
    # if ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ )
    # error "little-endian architecture required"
    # endif
    # else
    # error "endianness detection required"
    # endif

    # if defined ( __linux__ )
    # define __OS_LINUX
    # else
    # if defined ( __APPLE__ ) && defined ( __MACH__ )
    # define __OS_APPLE
    # else
    # error "unsupported plateform"
    # endif
    # endif

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <stdio.h>
    # include <stdint.h>
    # include <stdlib.h>
    # include <unistd.h>
    # include <string.h>
    # include <math.h>
    # include <limits.h>
    # include <inttypes.h>
    # include <time.h>
    # include <pthread.h>
    # include <dirent.h>
    # include <sys/stat.h>
    # include <sys/socket.h>
    # include <netinet/in.h>
    # include <netinet/tcp.h>
    # include <arpa/inet.h>

/*
    header - preprocessor definitions
 */

    /* boolean values */
    # define _LE_FALSE           ( 0 )
    # define _LE_TRUE            ( 1 )

    /* i/o values */
    # define _LE_GET             ( 0 )
    # define _LE_SET             ( 1 )

    /* features configuration values */
    # define _LE_USE_BASE        ( 8 )
    # define _LE_USE_OFFSET      ( 5 )
    # if ( _LE_USE_OFFSET < 2 ) ||  ( _LE_USE_OFFSET > 7 )
    # error "_LE_USE_OFFSET range is [2,7]"
    # endif
    # define _LE_USE_DEPTH       ( 40 )
    # define _LE_USE_TIMES       ( 3 )
    # define _LE_USE_DATA        ( 4 )
    # define _LE_USE_PORT        ( 11027 )
    # define _LE_USE_PENDING     ( 16 )
    # define _LE_USE_RETRY       ( 3 )
    # define _LE_USE_ARRAY       ( 2 )
    # define _LE_USE_PATH        ( PATH_MAX + FILENAME_MAX )
    # define _LE_USE_TIMEOUT     ( CLOCKS_PER_SEC >> 2 )

    /* define types */
    # define _LE_VOID            void
    # define _LE_BYTE            uint8_t
    # define _LE_CHAR            unsigned char
    # define _LE_ENUM            int
    # define _LE_SIZE            int64_t
    # define _LE_REAL            double
    # define _LE_TIME            int64_t
    # define _LE_DATA            uint8_t
    # define _LE_SOCK            int
    # define _LE_FILE            FILE *
    # define _LE_MASK            uint64_t
    # define _LE_PROC            pthread_t
    # define _LE_MUTE            pthread_mutex_t

    /* define type boundaries */
    # define _LE_BYTE_MIN        ( 0 )
    # define _LE_BYTE_MAX        ( UINT8_MAX )
    # define _LE_SIZE_MIN        ( INT64_MIN )
    # define _LE_SIZE_MAX        ( INT64_MAX )
    # define _LE_TIME_MIN        ( INT64_MIN )
    # define _LE_TIME_MAX        ( INT64_MAX )

    /* define type nulls */
    # define _LE_SIZE_NULL       ( _LE_SIZE_MIN )
    # define _LE_TIME_NULL       ( _LE_TIME_MIN )
    # define _LE_SOCK_NULL       ( -1 )
    # define _LE_OFFS_NULL       ( ( _LE_MASK_L( 1 ) << ( _LE_USE_BASE * _LE_USE_OFFSET ) ) - _LE_MASK_L( 1 ) )

    /* define type literal suffix */
    # define _LE_REAL_L(t)       ( t )
    # define _LE_SIZE_L(t)       ( INT64_C( t ) )
    # define _LE_TIME_L(t)       ( INT64_C( t ) )
    # define _LE_MASK_L(t)       ( UINT64_C( t ) )

    /* define type i/o specifiers */
    # define _LE_BYTE_P          PRIu8
    # define _LE_BYTE_S          SCNu8
    # define _LE_CHAR_P          "u"
    # define _LE_CHAR_S          "hhu"
    # define _LE_ENUM_P          "i"
    # define _LE_ENUM_S          "i"
    # define _LE_SIZE_P          PRId64
    # define _LE_SIZE_S          SCNd64
    # define _LE_REAL_P          ".14e"
    # define _LE_REAL_S          "lf"
    # define _LE_TIME_P          PRIi64
    # define _LE_TIME_S          SCNi64
    # define _LE_DATA_P          PRIu8
    # define _LE_DATA_S          SCNu8

    /* define errors */
    # define LE_ERROR_SUCCESS    ( 0x0000 )
    # define LE_ERROR_IO_ACCESS  ( 0x0001 )
    # define LE_ERROR_IO_READ    ( 0x0002 )
    # define LE_ERROR_IO_WRITE   ( 0x0003 )
    # define LE_ERROR_IO_SEEK    ( 0x0004 )
    # define LE_ERROR_IO_SOCKET  ( 0x0005 )
    # define LE_ERROR_IO_BIND    ( 0x0006 )
    # define LE_ERROR_IO_LISTEN  ( 0x0007 )
    # define LE_ERROR_IO_ARRAY   ( 0x000b )
    # define LE_ERROR_MEMORY     ( 0x0008 )
    # define LE_ERROR_DEPTH      ( 0x0009 )
    # define LE_ERROR_TIME       ( 0x000a )
    # define LE_ERROR_OVERFLOW   ( 0x000b )

    /* define server/client modes */
    # define LE_MODE_NULL        ( 0x00 )
    # define LE_MODE_AUTH        ( 0x01 )
    # define LE_MODE_DETE        ( 0x02 )
    # define LE_MODE_QUER        ( 0x03 )
    # define LE_MODE_INJE        ( 0x04 )

    /* define mathematical constants */
    # define LE_PI               ( 3.1415926535897932384626433832795029 )
    # define LE_P2               ( LE_PI / 2.0 )
    # define LE_2P               ( LE_PI * 2.0 )
    # define LE_D2R              ( LE_PI / 180.0 )
    # define LE_R2D              ( 180.0 / LE_PI )

    /* define pseudo-constructor */
    # if defined ( __OS_APPLE )
    # define LE_ADDRIN_C         { sizeof( struct sockaddr_in ), AF_INET, 0         , { INADDR_ANY }, { 0 } }
    # define LE_ADDRIN_C_PORT(p) { sizeof( struct sockaddr_in ), AF_INET, htons( p ), { INADDR_ANY }, { 0 } }
    # endif
    # if defined ( __OS_LINUX )
    # define LE_ADDRIN_C         { AF_INET, 0         , { INADDR_ANY }, { 0 } }
    # define LE_ADDRIN_C_PORT(p) { AF_INET, htons( p ), { INADDR_ANY }, { 0 } }
    # endif
    # define LE_STAT_C           { 0 }
    # define LE_MUTE_C           PTHREAD_MUTEX_INITIALIZER

/*
    header - preprocessor macros
 */

    /* define structure status */
    # define le_get_status(x)    ( ( x )._status )

    /* define structure status */
    # define le_set_status(x,m)  ( ( x )._status = ( m ), ( x ) )

    /* define absolute values */
    # define le_time_abs(x)      ( ( le_time_t ) llabs( x ) )

    /* define string conversion */
    # define le_time_str(x)      ( ( le_time_t ) strtoimax( x, NULL, 10 ) )

/*
    header - type definition
 */

    /* standard types definition */
    typedef _LE_VOID le_void_t;
    typedef _LE_BYTE le_byte_t;
    typedef _LE_CHAR le_char_t;
    typedef _LE_ENUM le_enum_t;
    typedef _LE_SIZE le_size_t;
    typedef _LE_REAL le_real_t;
    typedef _LE_TIME le_time_t;
    typedef _LE_DATA le_data_t;
    typedef _LE_SOCK le_sock_t;
    typedef _LE_FILE le_file_t;
    typedef _LE_MASK le_mask_t;
    typedef _LE_PROC le_proc_t;
    typedef _LE_MUTE le_mute_t;

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief accessor methods
     *
     *  This function answers \b _LE_TRUE if the provided path points to a file
     *  or a directory that exists. The function returns \b _LE_FALSE otherwise.
     *
     *  \param le_path Entity path
     *
     *  \return Returns _LE_TRUE if element exists, _LE_FALSE otherwise
     */

    le_enum_t le_get_exist( le_char_t const * const le_path );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

