/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

    /*! \file   eratosthene-pool.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - server module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_POOL__
    # define __LE_POOL__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "eratosthene.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_POOL_C { { 0 } }

    /* define flag masks */
    # define LE_POOL_S0 ( 0x01 )
    # define LE_POOL_C0 ( 0xfe )

    # define LE_POOL_S3 ( 0x04 )
    # define LE_POOL_C3 ( 0xfb )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_pool_struct
     *  \brief pool structure
     *
     *  mask model :
     *
     *      [01|2|34567]
     * 
     *  with :
     *
     *  0 : active
     *  2 : broadcast
     *  3-7 : message
     *  3 : reload
     */

    typedef struct le_pool_struct {

        le_byte_t pl_flag[_LE_USE_PENDING];

    } le_pool_t;

/*
    header - function prototypes
 */

    /* *** */

    le_byte_t le_pool_get( le_pool_t const * const le_pool, le_enum_t const le_tid, le_byte_t const le_flag );

    /* *** */

    le_void_t le_pool_set( le_pool_t * const le_pool, le_enum_t le_tid, le_byte_t const le_flag );

    /* *** */

    le_void_t le_pool_set_clear( le_pool_t * const le_pool, le_enum_t const le_tid, le_byte_t const le_flag );

    /* *** */

    le_void_t le_pool_set_broadcast( le_pool_t * const le_pool, le_enum_t const le_tid, le_byte_t const le_flag );

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

