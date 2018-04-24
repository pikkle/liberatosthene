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
    # define LE_TREE_C             { NULL, 0, 0, _LE_USE_COMB, 0, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_TREE_C_CONF(r,s,t) { r, s, t, _LE_USE_COMB, 0, NULL, LE_ERROR_SUCCESS }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_tree_struct
     *  \brief tree structure
     *
     *  This structure holds the required element to access the server storage
     *  structure. Usually, each client thread of the server holds its own
     *  instance of this structure that it uses for data access and export.
     *
     *  The three first field are copies of the server storage structure path
     *  and space-time configuration parameters. The next field is a statically
     *  initialised value that indicates the tree associated function to discard
     *  time units due to their temporal distance of the point of view. This
     *  allow to avoid mixing too much model that belong to too different times.
     *
     *  The next fields are the size of the unit stack and the stack itself. The
     *  stack is an array of unit structures that each contain the required data
     *  to access one time unit. It follows that any data injected in the
     *  storage structure is contained in one of those unit, distinguished
     *  through their time. It follows that any access starts by searching the
     *  unit associated to a time value.
     *
     *  A last field is used by the structure creation methods to specified the
     *  creation status of the structure. This allows to creation methods to
     *  return the structure instead of an error code.
     *  
     *  \var le_tree_struct::tr_root
     *  Server storage structure path
     *  \var le_tree_struct::tr_scfg
     *  Server spatial configuration value : number of scale
     *  \var le_tree_struct::tr_tcfg
     *  Server temporal configuration value : size of temporal classes
     *  \var le_tree_struct::tr_comb
     *  Server temporal comb size
     *  \var le_tree_struct::tr_size
     *  Tree unit stack size (number of units)
     *  \var le_tree_struct::tr_unit
     *  Tree unit stack
     *  \var le_tree_struct::_status
     *  Standard status field
     */

    typedef struct le_tree_struct {

        le_char_t * tr_root;
        le_size_t   tr_scfg;
        le_time_t   tr_tcfg;
        le_time_t   tr_comb;

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
     *  The function starts by enumerating the temporal units directory and
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
     *  \param le_path Server storage structure path
     *  \param le_scfg Server spatial configuration value
     *  \param le_tcfg Server temporal configuration value
     *
     *  \return Returns the created structure
     */

    le_tree_t le_tree_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided tree structure. It starts by parsing
     *  the tree unit structures stack to delete them using their specific
     *  method. The function then release the stack memory and clears the tree
     *  structure fields.
     *
     *  \param le_tree Tree structure
     */

    le_void_t le_tree_delete( le_tree_t * const le_tree );

    /*! \brief accessor methods
     * 
     *  This function search in the provided tree structure stack the unit
     *  structure that matches exactly the provided time value.
     *
     *  If such a unit is found in the tree stack, it is returned through its
     *  pointer. In case no unit can be found in the stack, the behaviour of the
     *  function depends on the provided mode value.
     *
     *  If the mode \b LE_UNIT_READ is provided and no unit is found, a NULL
     *  pointer is returned. If no unit is found and the \b LE_UNIT_WRITE is
     *  provided as mode value, the function ask for the creation of the unit
     *  storage allocation. The created unit structure is inserted in the tree
     *  stack and its pointer is returned.
     *
     *  \param le_tree Tree structure
     *  \param le_time Time value
     *  \param le_mode Access mode value
     *
     *  \return Returns the unit structure on success, NULL otherwise
     */

    le_unit_t * le_tree_get_unit( le_tree_t * const le_tree, le_time_t const le_time, le_enum_t const le_mode );

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
     *  optimisation of subsequent gathering processes.
     *
     *  \param le_tree   Tree structure
     *  \param le_addr   Address structure
     *  \param le_addrt  Offset of time - zero based
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
     *  If no unit storage structure is not found, the behaviour of the function
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
     *  \param le_time Time value
     *  \param le_mode Access mode value
     *
     *  \return Returns unit structure on success, NULL otherwise
     */

    le_unit_t * le_tree_set_push( le_tree_t * const le_tree, le_time_t const le_time, le_enum_t const le_mode );

    /*! \brief i/o methods
     *
     *  This function searches the offset of the spatial class pointed by the
     *  spatial index provided through the address structure in the provided
     *  unit storage structure.
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

    /*! \brief i/o methods
     *
     *  This function writes the content of the incoming array in the server
     *  storage structure.
     *
     *  The provided array is expect to have the UF3 format, that is a series
     *  of records composed of a coordinates 3-vector and a colour 3-vector.
     *
     *  For each array record, the function initiates an address structure using
     *  the record coordinates to compute the spatial index. The index is then
     *  used to drive the progression of the record injection through the scale
     *  files.
     *
     *  For a given scale, the class the record belong to is searched. If the
     *  class is found, the element is added to the class. Otherwise, a new
     *  class is created using the record to initialise it. The parent class
     *  offsets array is updated to take into account the new created class.
     *
     *  \param le_unit  Unit structure
     *  \param le_array Array structure
     *  \param le_scfg  Server spatial configuration value
     */

    le_void_t le_tree_io_inject( le_unit_t * const le_unit, le_array_t const * const le_array, le_size_t const le_scfg );

    /*! \brief i/o methods
     *
     *  This function is used to gather spatial classes positions and colours
     *  information to build a array using the UF3 format.
     *
     *  This function assume that the offset of the spatial class pointed by the
     *  address index is known (\b le_tree_io_offset()). Starting from the
     *  offsets array of this class, it starts to gather the position and
     *  colours by enumerating the class daughters and sub-daughters through a
     *  recursive process. It detect the gathering scale using the main class
     *  scale and the address additional depth (span).
     *
     *  If the provided array is not passed empty, its previous content is left
     *  unchanged, the function pushing the elements at the end of it.
     *
     *  \param le_unit   Unit structure
     *  \param le_addr   Address structure
     *  \param le_offset Class storage offset
     *  \param le_parse  Class storage scale
     *  \param le_span   Query additional depth
     *  \param le_array  Data array filled by the function
     */

    le_void_t le_tree_io_gather( le_unit_t * const le_unit, le_address_t * const le_addr, le_size_t le_offset, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /*! \brief i/o methods
     *
     *  This function implements a parallel version of \b le_tree_io_gather()
     *  function. It performs the same operations simply considering two times
     *  values.
     *
     *  As two parallel gathering processes take place in this function, it
     *  allows to consider the times comparison mode provided by the address
     *  structure. As elements for the two times are gathered at the same time,
     *  the function is able to easily implements the logical operators. The
     *  array is then filled with the results of the application of the logical
     *  operators.
     *
     *  If the provided array is not passed empty, its previous content is left
     *  unchanged, the function pushing the elements at the end of it.
     *  of it.
     *
     *  \param le_unia    Unit structure - ass. to time 1
     *  \param le_unib    Unit structure - ass. to time 2
     *  \param le_addr    Address structure
     *  \param le_mode    Address mode
     *  \param le_offseta Class storage offset - ass. to time 1
     *  \param le_offsetb Class storage offset - ass. to time 2
     *  \param le_parse   Class storage scale
     *  \param le_span    Query additional depth
     *  \param le_array   Data array filled by the function
     */

    le_void_t le_tree_io_parallel( le_unit_t * const le_unia, le_unit_t * const le_unib, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t le_offseta, le_size_t le_offsetb, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

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

