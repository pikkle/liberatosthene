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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

    /*! \file   eratosthene-class.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Classes module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_CLASS__
    # define __LE_CLASS__

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

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

    # define LE_CLASS_DIGIT( n ) ( n  & 0x0F )
    # define LE_CLASS_DAUGH( n ) ( n >> 0x04 )

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct le_class_struct {

        le_byte_t cs_node;
        le_size_t cs_addr[8];
        le_real_t cs_data[3];
        le_mean_t cs_mean;

    } le_class_t;

/*
    header - function prototypes
 */

    le_byte_t le_class_get_digit( le_class_t const * const le_class );
    le_byte_t le_class_get_dcc( le_class_t const * const le_class );
    le_mean_t le_class_get_mean( le_class_t const * const le_class );
    le_void_t le_class_set_clear( le_class_t * const le_class, le_byte_t const le_digit );
    le_void_t le_class_set_digit( le_class_t * const le_class, le_byte_t const le_digit );
    le_void_t le_class_set_dcc( le_class_t * const le_class, le_byte_t const le_dcc );
    le_void_t le_class_set_node( le_class_t * const le_class, le_byte_t const le_digit, le_byte_t const le_dcc );
    le_void_t le_class_set_inject( le_class_t * const le_class, le_real_t const le_red, le_real_t const le_green, le_real_t const le_blue );
    le_enum_t le_class_io_read( le_class_t * const le_class, FILE * const le_stream );
    le_enum_t le_class_io_write( le_class_t const * const le_class, FILE * const le_stream );

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

