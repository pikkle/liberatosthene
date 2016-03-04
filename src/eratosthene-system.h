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

    /*! \file   eratosthene-system.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  System module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SYSTEM__
    # define __LE_SYSTEM__

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
    # include "eratosthene-address.h"
    # include "eratosthene-array.h"
    # include "eratosthene-class.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define LE_SYSTEM_C { { 0 }, 0, 0, NULL }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_system_struct
        \brief system structure
     */

    typedef struct le_system_struct {

        char      sm_root[256];
        le_size_t sm_sdisc;
        le_time_t sm_tdisc;
        FILE **   sm_scale;

    } le_system_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     */

    le_enum_t le_system_create( le_system_t * const le_system, le_char_t const * const le_root );

    /*! \brief constructor/destructor methods
     */

    le_void_t le_system_delete( le_system_t * const le_system );

    /*! \brief injection methods
     */

    le_enum_t le_system_inject( le_system_t * const le_system, le_real_t * const le_pose, le_time_t const le_time, le_data_t const * const le_data );

    /*! \brief query methods
     */

    le_array_t le_system_query( le_system_t * const le_system, le_time_t const le_time, le_address_t * const le_addr, le_size_t const le_sdepth );

    /*! \brief query methods
     */

    le_void_t le_system_gather( le_system_t * const le_system, le_array_t * const le_qarray, le_time_t const le_time, le_address_t * const le_addr, le_class_t * const le_class, le_size_t const le_head, le_size_t const le_target );

    /*! \brief i/o methods
     */

    le_enum_t le_system_open( le_system_t * const le_system, le_time_t const le_time );

    /*! \brief i/o methods
     */

    le_void_t le_system_flush( le_system_t * const le_system );

    /*! \brief i/o methods
     */

    le_void_t le_system_close( le_system_t * const le_system );

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

