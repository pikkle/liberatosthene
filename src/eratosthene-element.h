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

    /* Define pseudo-constructor */
    # define LE_ELEMENT_C { 0, 0, 0, NULL }

    /* Define element sizes */
    # define LE_ELEMENT_SLEN ( sizeof( le_real_t ) * 3 )
    # define LE_ELEMENT_DLEN ( sizeof( le_data_t ) * 3 )
    # define LE_ELEMENT_ELEN ( LE_ELEMENT_SLEN + LE_ELEMENT_DLEN )

    /* Define array allocation step */
    # define LE_ELEMENT_STEP ( LE_ELEMENT_ELEN * 4096 )

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

        le_size_t   em_size;
        le_size_t   em_head;
        le_byte_t * em_elem;

    } le_element_t;

/*
    header - function prototypes
 */

    le_void_t le_element_set_clear( le_element_t * const le_element );

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

