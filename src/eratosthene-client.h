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

    /*! \file   eratosthene-client.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - client module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_CLIENT__
    # define __LE_CLIENT__

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
    # include "eratosthene-array.h"

/*
    header - external includes
 */

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
     *
     *  This function creates a client socket descriptor. It sets a connection
     *  toward the provided remote server, specified through it ip address and
     *  service port, and returns the created socket descriptor.
     *
     *  \param le_ip   Server ip address
     *  \param le_port Server service port
     *
     *  \return Created socket descriptor on success, _LE_SOCK_NULL otherwise
     */

    le_sock_t le_client_create( le_char_t const * const le_ip, le_sock_t const le_port );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided socket descriptor created by the
     *  function \b le_client_create(). It closes the connection to the remote
     *  server and returns a null socket descriptor.
     *
     *  \param le_socket Socket descriptor
     *
     *  \return Always _LE_SOCK_NULL socket descriptor
     */

    le_sock_t le_client_delete( le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function waits client connection and accept them as they arrive. In
     *  addition to the creation of the server client socket, using the provided
     *  server socket, the function also sets the server-side socket options.
     *
     *  This function is typically used by the server to handle incoming client
     *  connections.
     *
     *  \param le_listen Socket descriptor
     *
     * \return Created socket descriptor on success, _LE_SOCK_NULL otherwise
     */

    le_sock_t le_client_io_accept( le_sock_t const le_listen );

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

