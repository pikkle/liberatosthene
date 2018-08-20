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

    /*! \file   eratosthene-door.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - door
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_DOOR__
    # define __LE_DOOR__

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
    # include "eratosthene-address.h"
    # include "eratosthene-array.h"
    # include "eratosthene-class.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_DOOR_C            { { 0 }, 0, 0, 0, 0, { NULL }, _LE_OFFS_NULL, { 0 }, NULL, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_DOOR_C_SCT(s,c,t) { { 0 }, 0, s, c, t, { NULL }, _LE_OFFS_NULL, { 0 }, NULL, NULL, LE_ERROR_SUCCESS }

    /* define door mode */
    # define LE_DOOR_READ         ( 0 )
    # define LE_DOOR_WRITE        ( 1 )

    /* define door state */
    # define LE_DOOR_LOCK         ( 0 )
    # define LE_DOOR_UNLOCK       ( 1 )

/*
    header - preprocessor macros
 */

    /* define stream mode */
    # define le_door_mode(m)      ( ( m ) == LE_DOOR_READ ? "r+" : "w+" )

/*
    header - type definition
 */

/*
    header - structures
 */

    /* *** */

    typedef struct le_door_struct {

        le_char_t   dr_path[_LE_USE_PATH];
        le_size_t   dr_plen;

        le_size_t   dr_scfg;
        le_time_t   dr_tcfg;
        le_time_t   dr_time;

        le_file_t   dr_sacc[_LE_USE_DEPTH];
        le_size_t   dr_soff;

        le_char_t   dr_mpth[_LE_USE_PATH];

        le_void_t * dr_prev;
        le_void_t * dr_next;

    le_enum_t _status; } le_door_t;

/*
    header - function prototypes
 */

    /* *** */

    le_door_t le_door_create( le_char_t const * const le_root, le_size_t const le_scfg, le_time_t const le_tcfg, le_time_t const le_time, le_enum_t const le_mode );

    /* *** */

    le_void_t le_door_delete( le_door_t * const le_door );

    /* *** */

    le_enum_t le_door_get_switch( le_door_t const * const le_prev, le_door_t const * const le_next, le_time_t const le_reduced );

    /* *** */

    le_size_t le_door_get_offset( le_door_t const * const le_door );

    /* *** */

    le_time_t le_door_get_interval( le_door_t const * const le_door, le_time_t const le_time );

    /* *** */

    le_enum_t le_door_get_equal( le_door_t const * const le_door, le_time_t const le_time );

    /* *** */

    le_enum_t le_door_get_anterior( le_door_t const * const le_door, le_door_t const * const le_candidate );

    /* *** */

    le_door_t * le_door_get_prev( le_door_t const * const le_door );

    /* *** */

    le_door_t * le_door_get_next( le_door_t const * const le_door );

    /* *** */

    le_enum_t le_door_set_state( le_door_t const * const le_door, le_enum_t const le_state );

    /* *** */

    le_void_t le_door_set_insert( le_door_t * const le_door, le_door_t * const le_prev, le_door_t * const le_next );

    /* *** */

    le_enum_t le_door_io_detect_monovertex( le_door_t * const le_door, le_address_t const * const le_addr );

    /* *** */

    le_enum_t le_door_io_detect_multivertex( le_door_t * const le_door, le_address_t const * const le_addr );

    /* *** */

    le_enum_t le_door_io_inject_filter( le_door_t const * const le_door, le_array_t const * const le_array );

    /* *** */

    le_enum_t le_door_io_inject_monovertex( le_door_t * const le_door );

    /* *** */

    le_enum_t le_door_io_inject_multivertex( le_door_t const * const le_door );

    /* *** */

    le_void_t le_door_io_inject_clean( le_door_t const * const le_door );

    /* *** */

    le_void_t le_door_io_optimise_monovertex( le_door_t * const le_door );

    /* *** */

    le_void_t le_door_io_optimise_proc_monovertex( le_door_t * const le_door, le_file_t const * const le_dual, le_size_t const le_offset, le_size_t * const le_head, le_size_t const le_scale );

    /* *** */

    le_void_t le_door_io_gather_monovertex( le_door_t * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /* *** */

    le_void_t le_door_io_gather_multivertex( le_door_t const * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /* *** */

    le_void_t le_door_io_parallel_monovertex( le_door_t * const le_unia, le_door_t * const le_unib, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

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

