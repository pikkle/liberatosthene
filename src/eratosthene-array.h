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
 */

    /*! \file   eratosthene-array.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Array module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SYSTEM_ARRAY__
    # define __LE_SYSTEM_ARRAY__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include "eratosthene.h"
    # include "eratosthene-network.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor/destructor */
    # define LE_ARRAY_C        { 0, 0, NULL }

    /* Define heterogenous length */
    # define LE_ARRAY_POSE_LEN ( sizeof( le_real_t ) * 3 )
    # define LE_ARRAY_TIME_LEN ( sizeof( le_time_t )     )
    # define LE_ARRAY_DATA_LEN ( sizeof( le_data_t ) * 3 )
    # define LE_ARRAY_ELEM_LEN ( LE_ARRAY_POSE_LEN + LE_ARRAY_TIME_LEN + LE_ARRAY_DATA_LEN )

    /* Define array step */
    # define LE_ARRAY_STEP     ( 4096 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct le_array_struct {

        le_size_t   ar_vsze;
        le_size_t   ar_size;
        le_byte_t * ar_byte;

    } le_array_t;

/*
    header - function prototypes
 */

    le_array_t le_array_create( le_void_t );
    le_void_t le_array_delete( le_array_t * const le_array );
    le_size_t le_array_get_size( le_array_t * le_array );
    le_byte_t * le_array_get_byte( le_array_t * le_array );
    le_enum_t le_array_set_push( le_array_t * le_array, le_real_t const * const le_pose, le_time_t const le_time, le_data_t const * const le_data );
    le_enum_t le_array_io_write( le_array_t const * const le_array, le_sock_t const le_socket );

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

