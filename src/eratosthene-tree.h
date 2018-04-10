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

    /*! \brief constructor/destructor methods
     *
     *  This function creates and initialise a tree structure according to the
     *  provided storage structure path.
     *
     *  The function starts by enumerating the temporal unit directory and
     *  create for each of them a corresponding unit structure using their
     *  specific creation method.
     *
     *  Each unit structure is inserted in the tree stack in growing order from
     *  their respective time values. The time value of each unit is read from
     *  their directory name. As the stack is entirely filled, the function
     *  returns the created structure.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param le_path Path to the storage structure
     *  \param le_scfg Server spatial parameter
     *  \param le_tcfg Server temporal parameter
     *
     *  \return Returns the created structure
     */

    le_tree_t le_tree_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided tree structure. It starts by parsing
     *  the tree unit structures stack to delete them using their specific
     *  methods. The function then release the stack memory and clears the tree
     *  strcuture fields.
     *
     *  \param le_tree Tree structure
     */

    le_void_t le_tree_delete( le_tree_t * const le_tree );

    /*! \brief accessor methods
     * 
     *  This function search in the provided tree structure stack the unit
     *  structure that matches exactly the provided time value.
     *
     *  If such a unit structure exists in the tree stack, it is returned by the
     *  function. If no unit is found, the function ask for a new unit creation
     *  using the provided time value.
     *
     *  \param le_tree Tree structure
     *  \param le_time Unit time value
     *
     *  \return Returns the unit structure on success, NULL otherwise
     */

    le_unit_t * le_tree_get_inject( le_tree_t * const le_tree, le_time_t const le_time );

    /*! \brief accessor methods
     *
     *  This function search in the provided tree structure stack the nearest
     *  unit structure and its corresponding offset in which the class provided
     *  through the address structure is found and not empty. The proximity is
     *  understood from a temporal point of view.
     *
     *  The function starts by reading the provided address time using its index
     *  and start to search which unit structure is able to give access to the
     *  class pointed by the address structure digit. It searches, in both
     *  direction, through time for the nearest unit containing the non-empty
     *  class.
     *
     *  As a unit structure is found, it is returned by the function along with
     *  the offset of the class in the storage structure described by the unit
     *  structure. The offset is also returned in order to allow further
     *  optimisation of gathering processes.
     *
     *  \param le_tree   Tree structure
     *  \param le_addr   Address structure
     *  \param le_addrt  Index of the address time value
     *  \param le_offset Returned offset of the found class
     *
     *  \return Returns the unit structure on success, NULL otherwise
     */

    le_unit_t * le_tree_get_query( le_tree_t const * const le_tree, le_address_t * const le_addr, le_size_t const le_addrt, le_size_t * const le_offset );

    /*! \brief mutator methods
     *
     *  This function adds the unit corresponding to the provided time to the
     *  provided tree according to the specified mode value.
     *
     *  The function searches if the time value points to an existing storage
     *  structure. As the structure is found, the function creates a new unit in
     *  the tree stack and initialises it for the found storage structure.
     *
     *  If the storage structure is not found, the behaviour of the function
     *  depends on the access mode : \b LE_UNIT_READ indicates the function to
     *  fail. Providing \b LE_UNIT_WRITE tells the function to create and
     *  initialise a new storage structure corresponding to the provided time
     *  value.
     *
     *  If a new unit is pushed in the provided tree structure stack, it is
     *  always inserted in such a way the stack gives always unit in growing
     *  order accroding to their time value.
     *
     *  \param le_tree Tree structure
     *  \param le_time Unit time value
     *  \param le_mode Unit access mode
     *
     *  \return Returns unit structure on success, NULL otherwise
     */

    le_unit_t * le_tree_set_push( le_tree_t * const le_tree, le_time_t const le_time, le_enum_t const le_mode );

    /*! \brief i/o methods
     *
     *  This function searches the offset of the spatial class pointed by the
     *  spatial index provided through the address structure in the provided
     *  temporal class storage structure.
     *
     *  The spatial index digits are used to drive the progression through
     *  the structure scales. This function is mainly used to detect if the
     *  class pointed by the address exists or not.
     *
     *  \param le_unit Unit structure
     *  \param le_addr Address structure
     *
     *  \return Returns the class offset on success, _LE_OFFS_NULL otherwise
     */

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

