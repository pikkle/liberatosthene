/*
 *  liberatosthene - geodetic system
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
 *
 */

    /*! \file   eratosthene.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  General common header
     */

    /*! \mainpage liberatosthene
     *
     *  \section liberatosthene
     * 
     *  ...
     *
     *  \section Copyright and license
     *
     *  ...
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_ERATOSTHENE__
    # define __LE_ERATOSTHENE__

    # if ( __STDC_VERSION__ < 199901L )
    # error "this program requires C99 standard"
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
    # include <stdlib.h>
    # include <string.h>
    # include <sys/stat.h>

/*
    header - preprocessor definitions
 */

    /* Define boolean configuration */
    # define _LE_TRUE  1
    # define _LE_FALSE 0

    /* Define features */
    # define _LE_USE_STDIO_EXT _LE_TRUE

    /* Define feature implementation */
    # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
    # include <stdio_ext.h>
    # endif

    /* Define type implementation */
    # define _LE_VOID void
    # define _LE_BYTE uint8_t
    # define _LE_ENUM int
    # define _LE_CHAR unsigned char
    # define _LE_TIME int64_t

    /* Define class offset flags */
    # define _LE_OFFSET_32 32
    # define _LE_OFFSET_64 64
    /* Define class offset */
    # define _LE_OFFSET _LE_OFFSET_32

    /* Define class offset implementation */
    # if   ( _LE_OFFSET == _LE_OFFSET_32 )
    # define _LE_SIZE uint32_t
    # elif ( _LE_OFFSET == _LE_OFFSET_64 )
    # define _LE_SIZE uint64_t
    # else
    # error "Undefined class offset"
    # endif

    /* Define class precision flags */
    # define _LE_PRECISION_32 32
    # define _LE_PRECISION_64 64
    /* Define class precision */
    # define _LE_PRECISION _LE_PRECISION_32

    /* Define class precision implementation */
    # if   ( _LE_PRECISION == _LE_PRECISION_32 )
    # define _LE_REAL float
    # define _LE_MEAN uint32_t
    # elif ( _LE_PRECISION == _LE_PRECISION_64 )
    # define _LE_REAL double
    # define _LE_MEAN uint64_t
    # else
    # error "Undefined class precision"
    # endif

    /* Define library errors */
    # define LE_ERROR_SUCCESS   0
    # define LE_ERROR_IO_READ   1
    # define LE_ERROR_IO_WRITE  2
    # define LE_ERROR_IO_STREAM 3
    # define LE_ERROR_IO_ACCESS 4
    # define LE_ERROR_MEMORY    5

    /* Define mathematical constants */
    # define LE_PI 3.1415926535897932384626433832795029lf

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

    /* General void type */
    typedef _LE_VOID le_void_t;

    /* General byte type */
    typedef _LE_BYTE le_byte_t;

    /* General enumeration type */
    typedef _LE_ENUM le_enum_t;

    /* General char type */
    typedef _LE_CHAR le_char_t;

    /* General offset type */
    typedef _LE_SIZE le_size_t;

    /* General floating point type */
    typedef _LE_REAL le_real_t;

    /* General accumulation type */
    typedef _LE_MEAN le_mean_t;

    /* Genral time type */
    typedef _LE_TIME le_time_t;

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

