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

    /*! \file   eratosthene-switch.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - switch
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SWITCH__
    # define __LE_SWITCH__

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
    # include "eratosthene-door.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_SWITCH_C            { 0, NULL, 0, 0, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_SWITCH_C_PST(p,s,t) { 0, p, s, t, NULL, LE_ERROR_SUCCESS }

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
     *  \brief tree structure ( revoked )
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

    typedef struct le_switch_struct {

        le_time_t   sw_load;

        le_char_t * sw_path;
        le_size_t   sw_scfg;
        le_time_t   sw_tcfg;

        le_door_t * sw_door;

    le_enum_t _status; } le_switch_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods ( revoked )
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
     *  \param le_root Server storage structure path
     *  \param le_scfg Server spatial configuration value
     *  \param le_tcfg Server temporal configuration value
     *
     *  \return Returns the created structure
     */

    le_switch_t le_switch_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg );

    /*! \brief constructor/destructor methods ( revoked )
     *
     *  This function deletes the provided tree structure. It starts by parsing
     *  the tree unit structures stack to delete them using their specific
     *  method. The function then release the stack memory and clears the tree
     *  structure fields.
     *
     *  \param le_tree Tree structure
     */

    le_void_t le_switch_delete( le_switch_t * const le_switch );

    /*! \brief accessor methods ( revoked )
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

    le_door_t * le_switch_get_inject( le_switch_t * const le_switch, le_time_t const le_time, le_enum_t const le_mode );

    /*! \brief accessor methods ( revoked )
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

    le_door_t * le_switch_get_query( le_switch_t const * const le_switch, le_time_t const le_time, le_address_t const * const le_addr );

    /* *** */

    le_enum_t le_switch_set_update( le_switch_t * const le_switch, le_time_t const le_lifespan );

    /* *** */

    le_enum_t le_switch_set_build( le_switch_t * const le_switch );

    /*! \brief mutator methods ( revoked )
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

    le_door_t * le_switch_set_push( le_switch_t * const le_switch, le_time_t const le_time, le_enum_t const le_mode );

    /* *** */

    le_void_t le_switch_set_erase( le_switch_t * const le_switch );
    
    /*! \brief i/o methods ( revoked )
     *
     *  This i/o method is responsible of answering server configuration request
     *  from client. It simply packs the server configuration values, that are
     *  the number of space scale and the time equivalences classes length, in
     *  an array structure before to send it back to the remote client.
     *
     *  \param le_server Server structure
     *  \param le_tree   Tree structure
     *  \param le_stack  Socket-array stack
     *  \param le_socket Client socket descriptor
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_switch_io_auth( le_switch_t * const le_switch, le_array_t * const le_array, le_sock_t const le_socket );

    /*! \brief i/o methods ( revoked )
     *
     *  This i/o methods is responsible of data injection in the server storage
     *  structure. It expects a time packed in the first client array that is
     *  used to access the storage structure. If the corresponding unit is
     *  successfully accessed and locked, the function sends back the first
     *  client array as a confirmation.
     *
     *  It then expects a data array that contains the geographic coordinates
     *  and colours of the points to inject in the server storage structure. The
     *  revieved data are then injected in the server storage structure.
     *
     *  \param le_server Server structure
     *  \param le_tree   Tree structure
     *  \param le_stack  Socket-array stack
     *  \param le_socket Client socket descriptor
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_switch_io_inject( le_switch_t * const le_switch, le_array_t * const le_array, le_sock_t const le_socket );

    /*! \brief i/o methods ( revoked )
     *
     *  This function allows client to query a specific time storage allocation
     *  optimisation. The function reads the time packed in the socket-array and
     *  search for the corresponding storage unit. If the unit is successfully
     *  accessed and locked, the function sends the received socket-array back
     *  as a confirmation. It then executes the optimisation process on the
     *  pointed unit.
     *
     *  \param le_server Server structure
     *  \param le_tree   Tree structure
     *  \param le_stack  Socket-array stack
     *  \param le_socket Client socket descriptor
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_switch_io_optimise( le_switch_t * const le_switch, le_array_t * const le_array, le_sock_t const le_socket );

    /*! \brief i/o methods ( revoked )
     *
     *  This function reads the query addresses packed in the client array and
     *  gather the relevant data through specific tree method.
     *
     *  As the points are gathered from the storage structure, the function
     *  writes the resulting data array on the client socket.
     *
     *  \param le_server Server structure
     *  \param le_tree   Tree structure
     *  \param le_stack  Socket-array stack
     *  \param le_socket Client socket descriptor
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_switch_io_query( le_switch_t * const le_switch, le_array_t * const le_array, le_sock_t const le_socket );

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

