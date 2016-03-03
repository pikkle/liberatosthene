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

    /*! \file   eratosthene-server.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Server module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SERVER__
    # define __LE_SERVER__

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
    # include "eratosthene-array.h"
    # include "eratosthene-network.h"
    # include "eratosthene-system.h"

/*
    header - preprocessor definitions
 */

    /* Define the answer */
    # define LE_SERVER_ANSWER 42

    /* Define null socket */
    # define LE_SERVER_NULL  -1

    /* Define server lisenting port */
    # define LE_SERVER_PORT   555

    /* Define element length */
    # define LE_SERVER_ELEN   ( 3 * ( sizeof( le_real_t ) + sizeof( le_data_t ) ) + sizeof( le_time_t ) )

    /* Define ethernet frame maximum size */
    # define LE_SERVER_DGMAX  ( 30 * LE_SERVER_ELEN )

    /* Define connection type */
    # define LE_SERVER_CNNUL  0x00
    # define LE_SERVER_CNINJ  0x01
    # define LE_SERVER_CNQRY  0x02
    # define LE_SERVER_VALID  0xFF

    /* Define datagram length */
    # define LE_SERVER_DGRAM_LEN ( LE_ARRAY_ELEM_LEN * 30     )
    # define LE_SERVER_QUERY_LEN ( _LE_USE_DEPTH + 20 + 3 + 2 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    le_sock_t le_server_create( le_size_t le_port );
    le_void_t le_server_delete( le_sock_t const le_socket );
    le_void_t le_server2( le_sock_t const le_socket, le_system_t * const le_system );
    le_enum_t le_server_handshake_mode( le_sock_t const le_socket );
    le_enum_t le_server_handshake_auth( le_sock_t const le_socket, le_enum_t const le_auth );
    le_void_t le_server_system_inject( le_sock_t const le_socket, le_system_t * const le_system );
    le_enum_t le_server_system_query( le_sock_t const le_socket, le_system_t * const le_system );

    le_enum_t le_server_query( le_system_t * const le_system, int const le_client );

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

