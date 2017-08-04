/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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
     *  liberatosthene - common header
     */

    /*! \mainpage liberatosthene
     *
     *  \section liberatosthene
     *
     *  The _liberatosthene_ library implements an earth scale 4D tile server
     *  through a dedicated space-time index formalism. Using the theoretical
     *  framework of the index, it implements a simple and reliable 4D data
     *  storage and access. This allows servers built using this library to
     *  perform data injection and query with high efficiency over the network
     *  to provide 4D geographic services.
     *
     *  From a theoretical point of view, any type of data, as referenced in
     *  space and time, can be considered through the index formalism. Currently,
     *  colours are considered as data. This allows servers to broadcast large
     *  amount of point cloud over the network, toward large and multiscale 4D
     *  geographic information systems at earth scale.
     *
     *  \section Copyright and License
     *
     *  **liberatosthene** - Nils Hamel <br >
     *  Copyright (c) 2016-2017 EPFL CDH DHLAB
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_ERATOSTHENE__
    # define __LE_ERATOSTHENE__

    # if ( __STDC_VERSION__ < 199901L )
    # error "C99 standard required"
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
    # include <unistd.h>
    # include <stdlib.h>
    # include <string.h>
    # include <inttypes.h>
    # include <dirent.h>
    # include <sys/stat.h>
    # include <sys/types.h>
    # include <sys/socket.h>
    # include <netinet/in.h>
    # include <netinet/in.h>
    # include <arpa/inet.h>
    # include <netdb.h>
    # include <pthread.h>

/*
    header - preprocessor definitions
 */

    /* boolean values */
    # define _LE_FALSE              ( 0 )
    # define _LE_TRUE               ( 1 )

    /* i/o values */
    # define _LE_GET                ( 0 )
    # define _LE_SET                ( 1 )

    /* features configuration value */
    # define _LE_USE_BASE           ( 8 )
    # define _LE_USE_OFFSET         ( 5 )
    # if ( _LE_USE_OFFSET < 4 ) ||  ( _LE_USE_OFFSET > 7 )
    # error "offset range is [4,7]"
    # endif
    # define _LE_USE_DEPTH          ( 40 )
    # define _LE_USE_TIMES          ( 2 )
    # define _LE_USE_DATA           ( 4 )
    # define _LE_USE_PORT           ( 11027 )
    # define _LE_USE_PENDING        ( 16 )
    # define _LE_USE_RETRY          ( 3 )
    # define _LE_USE_STRING         ( 256 )
    # define _LE_USE_ARRAY          ( 3 )

    /* define types */
    # define _LE_VOID               void
    # define _LE_BYTE               uint8_t
    # define _LE_CHAR               unsigned char
    # define _LE_ENUM               int
    # define _LE_SIZE               int64_t
    # define _LE_REAL               double
    # define _LE_TIME               int64_t
    # define _LE_DATA               uint8_t
    # define _LE_SOCK               int
    # define _LE_FILE               FILE *
    # define _LE_PROC               pthread_t
    # define _LE_MASK               uint64_t

    /* define boundaries */
    # define _LE_BYTE_MIN           ( 0 )
    # define _LE_BYTE_MAX           ( UINT8_MAX )
    # define _LE_SIZE_MIN           ( INT64_MIN )
    # define _LE_SIZE_MAX           ( INT64_MAX )
    # define _LE_TIME_MIN           ( INT64_MIN )
    # define _LE_TIME_MAX           ( INT64_MAX )

    /* define litteral suffix */
    # define _LE_REAL_L(t)          ( t )
    # define _LE_SIZE_L(t)          ( INT64_C( t ) )
    # define _LE_TIME_L(t)          ( INT64_C( t ) )

    /* define i/o specifiers */
    # define _LE_BYTE_P             PRIu8
    # define _LE_BYTE_S             SCNu8
    # define _LE_CHAR_P             "u"
    # define _LE_CHAR_S             "hhu"
    # define _LE_ENUM_P             "i"
    # define _LE_ENUM_S             "i"
    # define _LE_SIZE_P             PRId64
    # define _LE_SIZE_S             SCNd64
    # define _LE_REAL_P             ".14e"
    # define _LE_REAL_S             "lf"
    # define _LE_TIME_P             PRIi64
    # define _LE_TIME_S             SCNi64
    # define _LE_DATA_P             PRIu8
    # define _LE_DATA_S             SCNu8

    /* define type nulls */
    # define _LE_BYTE_NULL          ( _LE_BYTE_MAX )
    # define _LE_SIZE_NULL          ( _LE_SIZE_MIN )
    # define _LE_TIME_NULL          ( _LE_TIME_MIN )
    # define _LE_SOCK_NULL          ( -1 )
    # define _LE_OFFS_NULL          ( ( _LE_SIZE_L( 1 ) << ( _LE_USE_BASE * _LE_USE_OFFSET ) ) - _LE_SIZE_L( 1 ) )

    /* define errors */
    # define LE_ERROR_SUCCESS       ( 0x0000 )
    # define LE_ERROR_IO_ACCESS     ( 0x0001 )
    # define LE_ERROR_IO_READ       ( 0x0002 )
    # define LE_ERROR_IO_WRITE      ( 0x0003 )
    # define LE_ERROR_IO_SEEK       ( 0x0004 )
    # define LE_ERROR_IO_SOCKET     ( 0x0005 )
    # define LE_ERROR_IO_BIND       ( 0x0006 )
    # define LE_ERROR_IO_LISTEN     ( 0x0007 )
    # define LE_ERROR_MEMORY        ( 0x0008 )
    # define LE_ERROR_DEPTH         ( 0x0009 )
    # define LE_ERROR_TIME          ( 0x000a )
    # define LE_ERROR_BASE          ( 0x000b )
    # define LE_ERROR_AUTH          ( 0x000c )
    # define LE_ERROR_FORMAT        ( 0x000d )
    # define LE_ERROR_AGREE         ( 0x000e )

    /* define server/client agreement */
    # define LE_AUTH_NULL           ( 0x0000000000000000 )
    # define LE_AUTH_QUER           ( 0xffff0000ffff0000 )
    # define LE_AUTH_AUTH           ( 0x0000ffff0000ffff )

    /* define server/client modes */
    # define LE_MODE_NULL           ( 0x00 )
    # define LE_MODE_AUTH           ( 0x01 )
    # define LE_MODE_RESI           ( 0x02 )
    # define LE_MODE_QUER           ( 0x03 )
    # define LE_MODE_INJE           ( 0x04 )

    /* define mathematical constants */
    # define LE_PI                  ( 3.1415926535897932384626433832795029 )
    # define LE_P2                  ( LE_PI / 2.0 )
    # define LE_2P                  ( LE_PI * 2.0 )

    /* define pseudo-constructor */
    # if defined ( __APPLE__ ) && defined ( __MACH__ )
    # define LE_ADDRIN_C            { AF_INET, 0         , INADDR_ANY, { 0 } }
    # define LE_ADDRIN_C_PORT( p )  { AF_INET, htons( p ), INADDR_ANY, { 0 } }
    # else
    # define LE_ADDRIN_C            { AF_INET, 0         , { INADDR_ANY }, { 0 } }
    # define LE_ADDRIN_C_PORT( p )  { AF_INET, htons( p ), { INADDR_ANY }, { 0 } }
    # endif

/*
    header - preprocessor macros
 */

    /* define absolute values */
    # define le_time_abs(x)         ( ( le_time_t ) llabs( x ) )

    /* define string conversion */
    # define le_time_str(x)         ( ( le_time_t ) strtoimax( x, NULL, 10 ) )

/*
    header - type definition
 */

    /* internal types */
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
    typedef _LE_PROC le_proc_t;
    typedef _LE_MASK le_mask_t;

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

