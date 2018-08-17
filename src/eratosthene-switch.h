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

    /* *** */

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

    /* *** */

    le_switch_t le_switch_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg );

    /* *** */

    le_void_t le_switch_delete( le_switch_t * const le_switch );

    /* *** */
    // note // mode only required for optimisation (implicit write on injection)

    le_door_t * le_switch_get_inject( le_switch_t * const le_switch, le_time_t const le_time, le_enum_t const le_mode );

    /* *** */

    le_door_t * le_switch_get_query( le_switch_t * const le_switch, le_time_t const le_time, le_address_t const * const le_addr );

    /* *** */

    le_enum_t le_switch_set_update( le_switch_t * const le_switch, le_time_t const le_lifespan );

    /* *** */

    le_enum_t le_switch_set_build( le_switch_t * const le_switch );

    /* *** */

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

