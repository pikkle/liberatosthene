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

    /*! \file   eratosthene-element.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Element module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SYSTEM_ELEMENT__
    # define __LE_SYSTEM_ELEMENT__

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

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct le_element_struct {

        le_real_t em_pose[3];
        le_byte_t em_data[3];
        le_time_t em_time;

    } le_element_t;

/*
    header - function prototypes
 */

    le_enum_t le_element_scan( le_element_t * const le_element, FILE * const le_stream );
    le_enum_t le_element_read( le_element_t * const le_element, FILE * const le_stream );
    le_enum_t le_element_print( le_element_t const * const le_element, FILE * const le_stream );
    le_enum_t le_element_write( le_element_t const * const le_element, FILE * const le_stream );

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

