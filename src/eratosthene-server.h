/*
 *  liberatosthene - eratosthene indexation server
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
    # include "eratosthene-address.h"
    # include "eratosthene-array.h"
    # include "eratosthene-class.h"
    # include "eratosthene-network.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define LE_SERVER_C { _LE_SOCK_NULL, { 0 }, 0, 0, 0, { 0 }, { { 0 } }, LE_ERROR_SUCCESS }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct le_server_struct {
    
        le_sock_t sv_sock;

        le_char_t sv_path[_LE_USE_STRING];
        le_size_t sv_scfg;
        le_time_t sv_tcfg;

        le_size_t sv_push;
        le_time_t sv_time[_LE_USE_STREAM];
        le_file_t sv_file[_LE_USE_STREAM][_LE_USE_DEPTH];

    le_enum_t _status; } le_server_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates a server socket descriptor. The service is created
     *  on the local machine using the provided port as service port.
     *
     *  \param le_port Server service port
     *
     *  \return Created socket descriptor on success, _LE_SOCK_NULL otherwise
     */

    le_server_t le_server_create( le_size_t const le_port, le_char_t const * const le_path );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the server socket descriptor created through the
     *  \b le_server_create function. It closes the service port and returns a
     *  null socket descriptor.
     *
     *  \param le_socket Socket descriptor
     *
     *  \return Always _LE_SOCK_NULL socket descriptor
     */

    le_void_t le_server_delete( le_server_t * const le_server );

    /*! \brief server methods
     *
     *  This function is the server main idle function. It starts a infinite
     *  loop waiting for client incoming connections. It then receives the
     *  clients queries handshakes and answer them. The proper queries answers
     *  are then sent by this function.
     *
     *  \param le_socket Socket descriptor
     *  \param le_system Server system descriptor
     */

    le_void_t le_server( le_server_t * const le_server );

    /*! \brief connection methods
     *
     *  This function answer to client injection queries. It receives the
     *  injected data coming from the client and ask the server system to insert
     *  them into the storage structure.
     *
     *  \param le_socket Socket descriptor
     *  \param le_system Server system descriptor
     */

    /*! \brief handshake methods
     *
     *  This function reads and returns the client query mode sent through the
     *  handshake procedure.
     *
     *  \param le_socket Socket descriptor
     *
     *  \return Query mode descriptor on success, LE_NETWORK_MODE_NULL on error
     */

    le_enum_t le_server_handshake( le_sock_t const le_socket );

    /*! \brief handshake methods
     *
     *  This function sends the query authorisation to the client through the
     *  server handshake answer.
     *
     *  \param le_socket Socket descriptor
     *  \param le_auth   Server authorisation
     *
     *  \return Returns LE_ERROR_SUCCESS on success, LE_ERROR_IO_WRITE otherwise
     */

    le_enum_t le_server_authorise( le_sock_t const le_socket, le_enum_t const le_auth );

    le_void_t le_server_inject_client( le_server_t * const le_server, le_sock_t const le_client );

    le_void_t le_server_inject( le_server_t * const le_server, le_array_sf_t const * const le_access );

    /*! \brief connection methods
     *
     *  This function answer to client queries on classes. It ask the server
     *  system to gather the queried classes and sends them to the client.
     *
     *  \param le_socket Socket descriptor
     *  \param le_system Server system descriptor
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_server_query_client( le_server_t * const le_server, le_sock_t const le_client );

    le_void_t le_server_query( le_server_t * const le_server, le_address_t * const le_addr, le_array_t * const le_array, le_size_t const le_parse, le_size_t le_offset, le_size_t const le_stream );

    le_enum_t le_server_times_client( le_server_t const * const le_server, le_sock_t const le_client );

    le_array_t le_server_times( le_server_t const * const le_server );

    le_enum_t le_server_config_client( le_server_t const * const le_server, le_sock_t const le_client );

    le_array_t le_server_config( le_server_t const * const le_server );

    le_size_t le_server_io_stream( le_server_t * const le_server, le_time_t const le_time );

    le_void_t le_server_io_flush( le_server_t const * const le_server, le_size_t const le_stream );

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

