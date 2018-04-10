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

    /*! \file   eratosthene-tree.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - tree module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_TREE__
    # define __LE_TREE__

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
    # include "eratosthene-tree-unit.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_TREE_C             { NULL, 0, 0, 0, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_TREE_C_CONF(r,s,t) { r, s, t, 0, NULL, LE_ERROR_SUCCESS }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /* *** */

    typedef struct le_tree_struct {

        le_char_t * tr_root;
        le_size_t   tr_scfg;
        le_time_t   tr_tcfg;

        le_size_t   tr_size;
        le_unit_t * tr_unit;

    le_enum_t _status; } le_tree_t;

/*
    header - function prototypes
 */

    /* *** */

    le_tree_t le_tree_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg );

    /* *** */

    le_void_t le_tree_delete( le_tree_t * const le_tree );

    /* *** */

    le_unit_t * le_tree_get_inject( le_tree_t * const le_tree, le_time_t const le_time );

    /* *** */

    le_unit_t * le_tree_get_query( le_tree_t const * const le_tree, le_address_t * const le_addr, le_size_t const le_addrt, le_size_t * const le_offset );

    /* *** */

    le_unit_t * le_tree_set_push( le_tree_t * const le_tree, le_time_t const le_time, le_enum_t const le_mode );

    /* *** */

    le_size_t le_tree_io_offset( le_unit_t * const le_unit, le_address_t * const le_addr );


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

