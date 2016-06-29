/*
 *  liberatosthene - eratosthene indexation server
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016 EPFL CDH DHLAB
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
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Common header
     */

    /*! \mainpage liberatosthene
     *
     *  \section liberatosthene
     * 
     *  The _liberatosthene_ library implements a colorimetric and georeferenced
     *  points server. It takes advantage of an indexation of space and time
     *  based on the definition of equivalence classes, to offers convenient and
     *  efficient management of large amount of point distributed all over the
     *  earth.
     *
     *  The server is designed to offer a simple interface for points injection
     *  and query through the specific space and time indexation. Due to the
     *  simplicity of the indexation it implements, large amount of colorimetric
     *  and georeferenced points are made suitable by the server for many
     *  different applications.
     *
     *  \section Copyright and License
     *
     *  **liberatosthene** - Nils Hamel <br >
     *  Copyright (c) 2016 EPFL CDH DHLAB
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_ERATOSTHENE__
    # define __LE_ERATOSTHENE__

    # if ( __STDC_VERSION__ < 199901L )
    # error "this program requires C99 standard"
    # endif

    # include <limits.h>
    # if ( CHAR_BIT != 8 )
    # error "this program expects 8 bits byte"
    # endif

    # if defined ( __BYTE_ORDER__ )
    # if ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ )
    # error "this program expects little-endian architecture"
    # endif
    # else
    # error "this program expects endianness detection availability"
    # endif

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include <stdio.h>
    # include <stdint.h>
    # include <unistd.h>
    # include <stdlib.h>
    # include <string.h>
    # include <inttypes.h>
    # include <sys/stat.h>
    # include <sys/types.h> 
    # include <sys/socket.h>
    # include <netinet/in.h>
    # include <netinet/in.h>
    # include <arpa/inet.h>
    # include <netdb.h> 

/*
    header - preprocessor definitions
 */

    /* Boolean values */
    # define _LE_FALSE          ( 0 )
    # define _LE_TRUE           ( 1 )

    /* Features configuration */
    # define _LE_USE_SIZE_T     ( 64 )
    # define _LE_USE_DEPTH      ( 64 )
    # define _LE_USE_QUERY      ( _LE_USE_DEPTH + 24 )
    # define _LE_USE_MTU        ( 1280 )
    # define _LE_USE_RETRY      ( 3 )
    # define _LE_USE_BASE       ( 8 )
    # define _LE_USE_PORT       ( 555 )
    # define _LE_USE_PENDING    ( 5 )
    # define _LE_USE_ASCII_ITOA ( 48 )

    /* Define types */
    # define _LE_VOID           void
    # define _LE_BYTE           uint8_t
    # define _LE_CHAR           unsigned char
    # define _LE_ENUM           int
    # if   ( _LE_USE_SIZE_T == 32 )
    # define _LE_SIZE           uint32_t
    # define _LE_DIFF           int32_t
    # elif ( _LE_USE_SIZE_T == 64 )
    # define _LE_SIZE           uint64_t
    # define _LE_DIFF           int64_t
    # endif
    # define _LE_REAL           double
    # define _LE_REAR           float
    # define _LE_TIME           int64_t
    # define _LE_DATA           uint8_t
    # define _LE_SOCK           int

    /* Define boundaries */
    # define _LE_BYTE_MIN       0
    # define _LE_BYTE_MAX       UINT8_MAX
    # if   ( _LE_USE_SIZE_T == 32 )
    # define _LE_SIZE_MAX       UINT32_MAX
    # elif ( _LE_USE_SIZE_T == 64 )
    # define _LE_SIZE_MAX       UINT64_MAX
    # endif
    # define _LE_TIME_MAX       INT64_MAX
    # define _LE_TIME_MIN       INT64_MIN

    /* Define type nulls */
    # define _LE_BYTE_NULL      0xff
    # define _LE_SIZE_NULL      _LE_SIZE_MAX
    # define _LE_TIME_NULL      _LE_TIME_MIN
    # define _LE_SOCK_NULL      -1

    /* Define input specifiers */
    # define _LE_BYTE_P         PRIu8
    # define _LE_BYTE_S         SCNu8
    # define _LE_CHAR_P         "u"
    # define _LE_CHAR_S         "hhu"
    # define _LE_ENUM_P         "i"
    # define _LE_ENUM_S         "i"
    # if   ( _LE_USE_SIZE_T == 32 )
    # define _LE_SIZE_P         PRIu32
    # define _LE_SIZE_S         SCNu32
    # elif ( _LE_USE_SIZE_T == 64 )
    # define _LE_SIZE_P         PRIu64
    # define _LE_SIZE_S         SCNu64
    # endif
    # define _LE_REAL_P         ".14e"
    # define _LE_REAL_S         "lf"
    # define _LE_TIME_P         PRIi64
    # define _LE_TIME_S         SCNi64
    # define _LE_DATA_P         PRIu8
    # define _LE_DATA_S         SCNu8

    /* Define errors */
    # define LE_ERROR_SUCCESS   0x0000
    # define LE_ERROR_IO_ACCESS 0x0001
    # define LE_ERROR_IO_READ   0x0002
    # define LE_ERROR_IO_WRITE  0x0003
    # define LE_ERROR_IO_SEEK   0x0004
    # define LE_ERROR_IO_STREAM 0x0005
    # define LE_ERROR_IO_SOCKET 0x0006
    # define LE_ERROR_MEMORY    0x0007
    # define LE_ERROR_DEPTH     0x0008
    # define LE_ERROR_TIME      0x0009
    # define LE_ERROR_BASE      0x000a
    # define LE_ERROR_AUTH      0x000b
    # define LE_ERROR_FORMAT    0x000c

    /* Define constants */
    # define LE_PI              ( 3.1415926535897932384626433832795029L )
    # define LE_P2              ( LE_PI / 2.0L )
    # define LE_2P              ( LE_PI * 2.0L )

/*
    header - preprocessor macros
 */

    /* Define modulo */
    # define le_mod(i,j)        ( (i) < 0 ? ( (j) + ( (i) % (j) ) ) % (j) : (i) % (j) )

/*
    header - type definition
 */

    /* Implement types */
    typedef _LE_VOID le_void_t;
    typedef _LE_BYTE le_byte_t;
    typedef _LE_CHAR le_char_t;
    typedef _LE_ENUM le_enum_t;
    typedef _LE_SIZE le_size_t;
    typedef _LE_DIFF le_diff_t;
    typedef _LE_REAL le_real_t;
    typedef _LE_REAR le_rear_t;
    typedef _LE_TIME le_time_t;
    typedef _LE_DATA le_data_t;
    typedef _LE_SOCK le_sock_t;

/*
    header - structures
 */

/*
    header - function prototypes
 */

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

