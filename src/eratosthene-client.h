/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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
     *  This function creates a client socket descriptor. It set a connection
     *  toward the provided server, specified through it ip address and service
     *  port, and returns the created socket descriptor.
     *
     *  \param le_ip   Server ip address
     *  \param le_port Server service port
     *
     *  \return Created socket descriptor on success, _LE_SOCK_NULL otherwise
     */

    le_sock_t le_client_create( le_char_t const * const le_ip, le_sock_t const le_port );

    /*! \brief constructor/destructor methods
     *
     *  This function uninitialises a socket descriptor created by the function
     *  \b le_client_create(). It closes the connection to the server and
     *  returns a null socket descriptor.
     *
     *  \param le_socket Socket descriptor
     *
     *  \return Always _LE_SOCK_NULL socket descriptor
     */

    le_sock_t le_client_delete( le_sock_t const le_socket );

    /*! \brief handshake methods
     *
     *  This function sends the query handshake to the server connected through
     *  the socket descriptor and waits the server answer. It then checks if the
     *  server allows the client to continue the query.
     *
     *  \param le_socket Socket descriptor
     *  \param le_mode   Client query mode
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_client_handshake( le_sock_t const le_socket, le_enum_t const le_mode );

    /*! \brief handshake methods
     *
     *  This function reads the expected client query handshake sent by the
     *  \b le_client_handshake() function. On success, it simply returns the
     *  query mode packed in the client query handshake.
     *
     *  \param le_socket Socket descriptor
     *
     *  \return Returns query mode on success, LE_MODE_NULL otherwise
     */

    le_byte_t le_client_switch( le_sock_t const le_socket );

    /*! \brief handshake methods
     *
     *  This function sends the sever answer to a client query handshake. It is
     *  used by the server to authorise the client to continue the query it
     *  initiated through the handshake.
     *
     *  \param le_socket Socket descriptor
     *  \param le_auth   Server authorisation mode
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_client_authorise( le_sock_t const le_socket, le_byte_t le_mode );

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

