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

    /*! \brief constructor/destructor methods
     */

    le_sock_t le_server_create( le_size_t const le_port );

    /*! \brief constructor/destructor methods
     */

    le_void_t le_server_delete( le_sock_t const le_socket );

    /*! \brief handshake methods
     */

    le_enum_t le_server_handshake_mode( le_sock_t const le_socket );

    /*! \brief handshake methods
     */

    le_enum_t le_server_handshake_auth( le_sock_t const le_socket, le_enum_t const le_auth );

    /*! \brief server methods
     */

    le_void_t le_server( le_sock_t const le_socket, le_system_t * const le_system );

    /*! \brief connection methods
     */

    le_void_t le_server_system_inject( le_sock_t const le_socket, le_system_t * const le_system );

    /*! \brief connection methods
     */

    le_enum_t le_server_system_query( le_sock_t const le_socket, le_system_t * const le_system );

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

